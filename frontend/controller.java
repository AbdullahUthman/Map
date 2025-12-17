import javafx.fxml.FXML;
import javafx.scene.paint.Color;
import javafx.scene.shape.Line;

import java.io.BufferedReader;
import java.io.InputStreamReader;

public class Controller {

    @FXML private Line A_B;
    @FXML private Line B_C;
    @FXML private Line C_D;

    @FXML
    public void initialize() {
        runCppAndHighlight();
    }

    private void runCppAndHighlight() {
        try {
            ProcessBuilder pb =
                new ProcessBuilder("../backend/dijkstra", "A", "D");

            Process process = pb.start();

            BufferedReader reader =
                new BufferedReader(
                    new InputStreamReader(process.getInputStream())
                );

            String output = reader.readLine(); // A-B,B-C,C-D

            String[] edges = output.split(",");

            for (String edge : edges) {
                highlight(edge);
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void highlight(String edgeId) {
        Line line = null;

        if (edgeId.equals("A-B")) line = A_B;
        if (edgeId.equals("B-C")) line = B_C;
        if (edgeId.equals("C-D")) line = C_D;

        if (line != null) {
            line.setStroke(Color.RED);
            line.setStrokeWidth(4);
        }
    }
}
