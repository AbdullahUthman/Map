package com.example.demo;

import javafx.fxml.FXML;
import javafx.scene.layout.Pane;
import javafx.scene.shape.*;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.scene.text.Text;
import java.io.*;
import java.util.ArrayList;

public class HelloController {

    @FXML
    private Pane mapPane;

    private Integer sourceId = null;
    private Integer destId = null;

    // --- POSITIONS ---
    private final double C1 = 150, C2 = 350, C3 = 550, C4 = 750;
    private final double R1 = 60, R2 = 180, R3 = 300, R4 = 420, R5 = 540;

    private final double[][] POSITIONS = {
            {C2, R1}, {C3, R1}, {C4, R1},           // 0-2
            {C1, R2}, {C2, R2}, {C3, R2}, {C4, R2}, // 3-6
            {C1, R3}, {C2, R3}, {C3, R3}, {C4, R3}, // 7-10 (8=Park)
            {C1, R4}, {C2, R4}, {C3, R4}, {C4, R4}, // 11-14
            {C1, R5}, {C2, R5}, {C3, R5}, {C4, R5}, {C4+140, R5} // 15-19
    };

    private final String[] NAMES = {
            "Entrance", "Fire Stn", "Gaming",
            "Res Block 1", "Police Stn", "Commercial", "Food Court",
            "Res Block 2", "Public Park", "Mall", "Cinema",
            "Sports Cmplx", "Hospital", "School", "Library",
            "Parking", "Pharmacy", "Clinic", "Playground", "Exit"
    };

    public void initialize() {
        drawMap(new ArrayList<>());
    }

    private void drawMap(ArrayList<Integer> pathNodes) {
        mapPane.getChildren().clear();
        mapPane.setStyle("-fx-background-color: #f4f4f4;");

        // --- ROADS (Matches C++ Exactly) ---
        int[][] roads = {
                // Local
                {0,1}, {1,2}, {3,4}, {4,5}, {5,6}, {7,8}, {8,9}, {9,10},
                {11,12}, {12,13}, {13,14}, {15,16}, {16,17}, {17,18}, {18,19},
                {0,4}, {4,8}, {8,12}, {12,16}, {1,5}, {5,9}, {9,13}, {13,17},
                {3,7}, {7,11}, {11,15}, {2,6}, {6,10}, {10,14}, {14,18},
                // Express (Park 8)
                {8,0}, {8,1}, {8,2}, {8,3}, {8,6}, {8,11}, {8,14}, {8,15}, {8,19}, {8,5}, {8,12},
                // Express (Comm 5)
                {5,0}, {5,2}, {5,7}, {5,10}, {5,12}, {5,14}, {5,17}
        };

        for (int[] road : roads) {
            drawLine(road[0], road[1], pathNodes);
        }

        for (int i = 0; i < 20; i++) {
            drawBlock(i, pathNodes);
        }
    }

    private void drawLine(int u, int v, ArrayList<Integer> path) {
        Line line = new Line(POSITIONS[u][0], POSITIONS[u][1], POSITIONS[v][0], POSITIONS[v][1]);

        // Style
        double dist = Math.hypot(POSITIONS[u][0]-POSITIONS[v][0], POSITIONS[u][1]-POSITIONS[v][1]);
        if (dist > 220) {
            line.setStroke(Color.LIGHTBLUE);
            line.setStrokeWidth(3.0);
            line.getStrokeDashArray().addAll(10d, 5d);
            line.setOpacity(0.5);
        } else {
            line.setStroke(Color.GRAY);
            line.setStrokeWidth(2.0);
        }

        // --- HIGHLIGHTING LOGIC ---
        // If nodes are neighbors in the path list, color red
        boolean isPath = false;
        if (path.contains(u) && path.contains(v)) {
            int idxU = path.indexOf(u);
            int idxV = path.indexOf(v);
            if (Math.abs(idxU - idxV) == 1) isPath = true;
        }

        if (isPath) {
            line.setStroke(Color.RED);
            line.setStrokeWidth(5.0);
            line.setOpacity(1.0);
            line.getStrokeDashArray().clear();
            line.toFront();
        }
        mapPane.getChildren().add(line);
    }

    private void drawBlock(int id, ArrayList<Integer> path) {
        double x = POSITIONS[id][0];
        double y = POSITIONS[id][1];

        Color fillColor = Color.DODGERBLUE;
        if (sourceId != null && sourceId == id) fillColor = Color.LIMEGREEN;
        else if (destId != null && destId == id) fillColor = Color.ORANGE;
        else if (path.contains(id)) fillColor = Color.TOMATO;

        Shape shape;
        if (id == 8) shape = new Circle(x, y, 60);
        else shape = new Rectangle(x - 65, y - 25, 130, 50);

        shape.setFill(fillColor);
        shape.setStroke(Color.BLACK);
        shape.setStrokeWidth(1.5);
        shape.toFront();

        int finalId = id;
        shape.setOnMouseClicked(e -> {
            if (e.getClickCount() == 2) { // DOUBLE CLICK
                if (sourceId == null) {
                    sourceId = finalId;
                    System.out.println("Source Set: " + sourceId);
                    drawMap(new ArrayList<>());
                } else {
                    destId = finalId;
                    System.out.println("Dest Set: " + destId + ". Running Backend...");
                    runBackend();
                    sourceId = null; destId = null; // Reset for next time
                }
            }
        });

        Text text = new Text(NAMES[id]);
        text.setFont(Font.font("Arial", FontWeight.BOLD, 12));
        text.setFill(Color.WHITE);
        text.setX(x - text.getLayoutBounds().getWidth() / 2);
        text.setY(y + 4);
        text.toFront();

        mapPane.getChildren().addAll(shape, text);
    }

    private void runBackend() {
        try {
            // ProcessBuilder
            ProcessBuilder pb = new ProcessBuilder("backend.exe");
            Process p = pb.start();

            // SEND INPUT (0 19)
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(p.getOutputStream()));
            writer.write(sourceId + " " + destId + "\n");
            writer.flush();
            writer.close();

            // READ OUTPUT
            BufferedReader reader = new BufferedReader(new InputStreamReader(p.getInputStream()));
            String line = reader.readLine();

            System.out.println("C++ OUTPUT: " + line); // CHECK THIS IN CONSOLE

            ArrayList<Integer> path = new ArrayList<>();
            if (line != null && !line.startsWith("No")) {
                String[] parts = line.trim().split("\\s+");
                for (String s : parts) path.add(Integer.parseInt(s));
            }
            drawMap(path);

        } catch (Exception e) {
            e.printStackTrace(); // THIS WILL PRINT IF FILE NOT FOUND
        }
    }
}