package com.example.demo;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;

public class HelloApplication extends Application {
    @Override
    public void start(Stage stage) throws IOException {
        // Note: No leading slash. This looks in the same package as this class.
        FXMLLoader fxmlLoader = new FXMLLoader(HelloApplication.class.getResource("hello-view.fxml"));

        if (fxmlLoader.getLocation() == null) {
            throw new RuntimeException("FXML file not found! Check target/classes/com/example/demo/");
        }

        Scene scene = new Scene(fxmlLoader.load(), 320, 240);
        stage.setTitle("Frankenstein Map App");
        stage.setScene(scene);
        stage.show();
    }

    public static void main(String[] args) {
        launch();
    }
}

//changes
