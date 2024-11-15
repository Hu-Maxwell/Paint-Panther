#include "../include/PaintApp.h"

// ====================================
// public functions
// ====================================

// constructor - initializes the window, texture, and current color
PaintApp::PaintApp()
    : window(sf::VideoMode(1600, 1000), "Paint2D App"), 
    toolbar(window), 
    currentColor(sf::Color::Red) {

    window.setFramerateLimit(144); 

    // eraser outline 
    eraserOutline.setRadius(eraserRadius);
    eraserOutline.setFillColor(sf::Color::White);
    eraserOutline.setOutlineColor(sf::Color::Black);
    eraserOutline.setOutlineThickness(1.0f);
    
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    texture.create(desktopMode.width, desktopMode.height);
    texture.clear(sf::Color::White);
    texture.display();
    sprite.setTexture(texture.getTexture());
    sprite.setPosition(0, 0);
}

// run - main loop of the application
void PaintApp::run() {
    int res = 0;  
    if (startingScreen) {
        StartingScreen screen(window);
        res = screen.mainLoop(); 
    }

    if (res == -1) {
        window.close(); 
    }
    if (res == 1) {
        sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
        window.create(sf::VideoMode(desktopMode.width, desktopMode.height), "Paint2D App");
    }

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
    window.draw(sprite);

    if (currentTool == Tool::Eraser) {
        window.draw(eraserOutline);
    }

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

    // render UI 
    toolbar.renderUI();
    window.display();
}