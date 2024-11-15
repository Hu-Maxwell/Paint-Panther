#include "../include/PaintApp.h"

// ====================================
// public functions
// ====================================

// constructor - initializes the window, texture, and current color
PaintApp::PaintApp()
    : window(sf::VideoMode(1600, 1000), "Paint2D App"),
    toolbar(window), 
    currentColor(sf::Color::Red) {
    
    texture.create(1600, 1200); // TODO: change so it fits inside window
    texture.clear(sf::Color::White);
    texture.display();
    sprite.setTexture(texture.getTexture());
}

// run - main loop of the application
void PaintApp::run() {
    while (window.isOpen()) {
        handleEvents();
        render();
    }
}

// ====================================
// private functions
// ====================================

// draws all elements to the window
void PaintApp::render() {
    window.clear();
     if (!startScreen.isStartClicked()) {
        // Render Start Screen
        startScreen.draw();
    }
    else {
        // Draw the main texture
        window.draw(sprite);

        // Draw ongoing shapes if any
        if (isDrawingRect) {
            window.draw(currentRectangle);
        }

        if (isDrawingCircle) {
            window.draw(currentCircle);
        }

        if (isDrawingPolygon && polygonPoints.size() > 1) {
            sf::VertexArray lines(sf::LinesStrip, polygonPoints.size());
            for (size_t i = 0; i < polygonPoints.size(); ++i) {
                lines[i].position = polygonPoints[i];
                lines[i].color = currentColor;
            }
            window.draw(lines);
        }

        if (isDrawingTriangle) {
            window.draw(currentTriangle);
        }

        // Render UI
        toolbar.renderUI();
    }

    window.display();
}