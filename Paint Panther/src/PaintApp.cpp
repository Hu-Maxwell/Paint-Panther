#include "../include/PaintApp.h"

// ====================================
// public functions
// ====================================

// constructor - initializes the window, texture, and current color
PaintApp::PaintApp()
    : window(sf::VideoMode(1600, 1000), "Paint2D App"), 
    toolbar(window), 
    currentColor(sf::Color::Black) {

    window.setFramerateLimit(144); 

    if (!font.loadFromFile("assets/Zain-Regular.ttf")) {
        std::cerr << "Couldn't load 'arial.ttf'." << std::endl;
        exit(EXIT_FAILURE);
    }

    container.setPosition(window.getSize().x * .85, 100);
    container.setSize(sf::Vector2f(600, 200));
    container.setFillColor(sf::Color(150, 150, 150));

    text.setFont(font);
    text.setPosition(window.getSize().x * .85 + 10, 100);
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::Black);
    text.setString("Loading...");


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

    if (isAiResponse) {
        window.draw(container);
        window.draw(text);
    }//ai

    // render UI 
    toolbar.renderUI();
    window.display();
}