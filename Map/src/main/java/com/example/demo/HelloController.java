package com.example.demo;

import javafx.fxml.FXML;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.*;
import javafx.scene.shape.*;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.scene.text.Text;
import javafx.scene.Group;
import java.io.*;
import java.util.ArrayList;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;

public class HelloController {

    private ImageView backgroundView;
    private double scale = 1.0;
    private static final double CELL_W = 220;
    private static final double CELL_H = 160;
    private double lastMouseX;
    private double lastMouseY;
    private ArrayList<Integer> lastPath = new ArrayList<>();




    @FXML
    private void zoomIn() {
        scale += 0.1;
        mapGroup.setScaleX(scale);
        mapGroup.setScaleY(scale);
    }

    @FXML
    private void zoomOut() {
        scale = Math.max(0.5, scale - 0.1);
        mapGroup.setScaleX(scale);
        mapGroup.setScaleY(scale);
    }

    @FXML
    private Pane mapPane;

    @FXML
    private Group mapGroup;

    @FXML
    private Label sourceLabel;

    @FXML
    private Label destLabel;

    @FXML
    private TextField searchField;

    @FXML
    private Label distanceLabel;
    @FXML
    private Label stepsLabel;

    private Integer searchedId = null;
    private Integer sourceId = null;
    private Integer destId = null;

    private final double[][] POSITIONS = {
            cell(2, 0),  // 0 Entrance 3
            cell(0, 1),  // 1 Parking Area
            cell(0, 2),  // 2 Entrance 2

            cell(2, 1),  // 3 Sports Complex (big)
            cell(1, 2),  // 4 CAFE 1
            cell(3, 2),  // 5 Green Area 1

            cell(1, 3),  // 6 ATM
            cell(3, 3),  // 7 Business Inc Center

            cell(0, 4),  // 8 Library
            cell(1, 4),  // 9 Green Area 2
            cell(2, 4),  // 10 Green Area 3

            cell(4, 4),  // 11 Auditorium
            cell(3, 4),  // 12 Admission Office

            cell(1, 5),  // 13 Main Entrance
            cell(2, 5),  // 14 Admin Office
            cell(3, 5),  // 15 Student Affairs

            cell(2, 2),  // 16 Cafe 2
            cell(1, 1),  // 17 Gym

            cell(2, 0),  // 18 A-Block
            cell(3, 0),  // 19 B-Block
            cell(4, 0),  // 20 C-Block
            cell(5, 0),  // 21 Mosque
            cell(6, 1),  // 22 China Block
            cell(6, 2),  // 23 Arena

            cell(4, 5)   // 24 Green Area 4
    };

    private double[] cell(int col, int row) {
        return new double[] {
                100 + col * CELL_W,
                80 + row * CELL_H
        };
    }




    private final String[] NAMES = {
            "Entrance 3", "Parking Area", "Entrance 2", "Sports Complex",
            "CAFE 1", "Green Area 1", "ATM", "Business Inc Center",
            "Library", "Green Area 2", "Green Area 3", "Auditorium",
            "Admission Office", "Main Entrance", "Admin Office", "Student Affairs",
            "Cafe 2", "Gym", "A-Block", "B-Block", "C-Block", "Mosque",
            "China Block", "Arena", "Green Area 4"
    };

    private boolean isGreenArea(int id) {
        return id == 5 || id == 9 || id == 10 || id == 24;
    }

    @FXML
    private void onSearch() {
        String query = searchField.getText().trim();
        if (query.isEmpty()) {
            searchedId = null;
            drawMap(new ArrayList<>());
            return;
        }

        try {
            Process p = new ProcessBuilder("backend.exe").start();
            BufferedWriter w = new BufferedWriter(new OutputStreamWriter(p.getOutputStream()));
            w.write("F " + query + "\n");
            w.flush();
            w.close();

            BufferedReader r = new BufferedReader(new InputStreamReader(p.getInputStream()));
            String line = r.readLine();
            if (line == null) return;

            int id = Integer.parseInt(line.trim());
            searchedId = (id >= 0) ? id : null;
            drawMap(new ArrayList<>());

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void initialize() {
        mapGroup = new Group();
        mapPane.getChildren().add(mapGroup);

        // ---- BACKGROUND IMAGE ----
        Image bgImage = new Image(
                getClass().getResource("/images/picture.png").toExternalForm()
        );

        backgroundView = new ImageView(bgImage);
        backgroundView.setFitWidth(1600);
        backgroundView.setFitHeight(1200);
        backgroundView.setPreserveRatio(false);

        // Background MUST be first
        mapGroup.getChildren().add(backgroundView);

        distanceLabel.setText("— meters");
        stepsLabel.setText("— steps");

        enablePan();
        enableScrollZoom();

        drawMap(lastPath);
    }


    private void drawMap(ArrayList<Integer> pathNodes) {
        mapGroup.getChildren().clear();

        // Always add background first
        mapGroup.getChildren().add(backgroundView);

        int[][] roads = {
                {0,3}, {0,1}, {0,18},
                {1,2}, {1,3}, {1,17},
                {2,4}, {2,16},
                {3,4}, {3,5}, {3,23},
                {4,5}, {4,6}, {4,16},
                {5,7}, {5,11},
                {6,7}, {6,10}, {6,12}, {6,14},
                {7,11}, {7,12}, {7,15},
                {8,9}, {8,10}, {8,13}, {8,14},
                {9,10}, {9,13},
                {10,12}, {10,13},
                {11,12}, {11,23},
                {12,13}, {12,24},
                {13,14}, {13,15},
                {14,15}, {14,24},
                {15,24},
                {16,17}, {16,24},
                {17,18}, {17,23},
                {18,19}, {18,20},
                {19,20}, {19,21},
                {20,21},
                {21,22},
                {22,23},
                {23,24}
        };

        // Roads second
        for (int[] road : roads) {
            drawLine(road[0], road[1], pathNodes);
        }

        // Nodes last
        for (int i = 0; i < NAMES.length; i++) {
            drawBlock(i, pathNodes);
        }
    }


    private void drawLine(int u, int v, ArrayList<Integer> path) {
        Line line = new Line(POSITIONS[u][0], POSITIONS[u][1], POSITIONS[v][0], POSITIONS[v][1]);
        line.setStroke(Color.DIMGRAY);
        line.setStrokeWidth(2.5);

        boolean isPath = false;
        if (path.contains(u) && path.contains(v)) {
            int idxU = path.indexOf(u);
            int idxV = path.indexOf(v);
            if (Math.abs(idxU - idxV) == 1) isPath = true;
        }

        if (isPath) {
            line.setStroke(Color.RED);
            line.setStrokeWidth(4.0);
        }
        mapGroup.getChildren().add(line);
    }

    private void drawBlock(int id, ArrayList<Integer> path) {
        double x = POSITIONS[id][0];
        double y = POSITIONS[id][1];

        Color fillColor = isGreenArea(id) ? Color.LIGHTGREEN : Color.DODGERBLUE;

        if (sourceId != null && sourceId == id) {
            fillColor = Color.LIMEGREEN;
        } else if (destId != null && destId == id) {
            fillColor = Color.ORANGE;
        } else if (!path.isEmpty() && id != path.get(0) && id != path.get(path.size() - 1) && path.contains(id)) {
            fillColor = Color.TOMATO;
        }

        if (searchedId != null && searchedId == id) {
            fillColor = Color.GOLD;
        }

        Shape shape;
        if (id == 3) {
            shape = new Rectangle(x - 80, y - 50, 160, 100);
        } else if (isGreenArea(id)) {
            shape = new Circle(x, y, 30);
        } else {
            shape = new Rectangle(x - 60, y - 25, 120, 50);
        }

        shape.setFill(fillColor);
        shape.setStroke(Color.BLACK);
        shape.setStrokeWidth(1.5);

        int finalId = id;
        shape.setOnMouseClicked(e -> {
            if (e.getClickCount() == 2) {
                if (isGreenArea(finalId)) {
                    System.out.println("Cannot select green area");
                    return;
                }

                if (sourceId == null) {
                    sourceId = finalId;
                    sourceLabel.setText(NAMES[finalId]);
                    destLabel.setText("None");
                    drawMap(new ArrayList<>());
                } else {
                    destId = finalId;
                    destLabel.setText(NAMES[finalId]);
                    runBackend();
                    sourceId = null;
                    destId = null;
                }
            }
        });

        Text text = new Text(NAMES[id]);
        text.setFont(Font.font("Arial", FontWeight.BOLD, 10));
        text.setFill(isGreenArea(id) ? Color.DARKGREEN : Color.WHITE);
        text.setX(x - text.getLayoutBounds().getWidth() / 2);
        text.setY(y + 3);

        mapGroup.getChildren().addAll(shape, text);
    }

    private void runBackend() {
        try {
            Process p = new ProcessBuilder("backend.exe").start();

            BufferedWriter writer =
                    new BufferedWriter(new OutputStreamWriter(p.getOutputStream()));
            writer.write("S " + sourceId + " " + destId + "\n");
            writer.flush();
            writer.close();

            BufferedReader reader =
                    new BufferedReader(new InputStreamReader(p.getInputStream()));

            ArrayList<Integer> path = new ArrayList<>();
            int distance = -1;

            String line;
            while ((line = reader.readLine()) != null) {

                line = line.trim();
                if (line.isEmpty()) continue;

                // DEBUG (keep this for now)
                System.out.println("RAW BACKEND: [" + line + "]");

                // Distance (optional)
                if (line.startsWith("DIST")) {
                    distance = Integer.parseInt(line.replaceAll("[^0-9]", ""));
                    continue;
                }

                // 🔥 PATH: any line containing node IDs
                if (Character.isDigit(line.charAt(0))) {
                    String[] parts = line.split("\\s+");
                    for (String s : parts) {
                        if (s.matches("\\d+")) {
                            path.add(Integer.parseInt(s));
                        }
                    }
                }
            }


            // 🔴 update UI
            if (distance >= 0) {
                distanceLabel.setText(distance + " meters");
                stepsLabel.setText((int)(distance * 0.76) + " steps");
            }

            // 🔥 THIS is what re-enables highlighting
            lastPath = path;
            drawMap(lastPath);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }


    private void enablePan() {
        mapPane.setOnMousePressed(e -> {
            lastMouseX = e.getSceneX();
            lastMouseY = e.getSceneY();
        });

        mapPane.setOnMouseDragged(e -> {
            double dx = e.getSceneX() - lastMouseX;
            double dy = e.getSceneY() - lastMouseY;

            mapGroup.setTranslateX(mapGroup.getTranslateX() + dx);
            mapGroup.setTranslateY(mapGroup.getTranslateY() + dy);

            lastMouseX = e.getSceneX();
            lastMouseY = e.getSceneY();
        });
    }



    private void enableScrollZoom() {
        mapPane.setOnScroll(e -> {
            double zoomFactor = (e.getDeltaY() > 0) ? 1.1 : 0.9;

            scale *= zoomFactor;
            scale = Math.max(0.5, Math.min(scale, 3.0)); // clamp

            mapGroup.setScaleX(scale);
            mapGroup.setScaleY(scale);

            e.consume();
        });
    }


}
