#include "../include/PaintApp.h"

// ====================================
// public functions
// ====================================

// constructor - initializes the window, texture, and current color
PaintApp::PaintApp()
    : window(sf::VideoMode(800, 600), "Paint2D App"),
    toolbar(window), 
    currentColor(sf::Color::Black) {
    
    texture.create(800, 600);
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

// handleEvents - handles all events in the application
void PaintApp::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                // TODO: still bugged if you click in the toolbar
                if (toolbar.handleUIInput(event) != Tool::Nothing) {
                    Tool clickedTool = toolbar.handleUIInput(event);
                    if (clickedTool == Tool::Undo) {
                        undo();
                    }
                    else if (clickedTool == Tool::Redo) {
                        redo();
                    }
                    else if (clickedTool == Tool::Pen) {
                        currentTool = clickedTool;
                    }
                    else if (clickedTool == Tool::Rect) {
                        currentTool = clickedTool;
                    }
                    break;
                }
                if (currentTool == Tool::Pen) {
                    startDrawing();
                }
                else if (currentTool == Tool::Rect) {
                    startRect();
                }
                else if (currentTool == Tool::Circle) {
                    startCircle();
                }
            }
        }

        else if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (currentTool == Tool::Pen) {
                    stopDrawing();
                }
                else if (currentTool == Tool::Rect) {
                    stopRect();
                }
                else if (currentTool == Tool::Circle) {
                    stopCircle();
                }
            }
        }

        else if (event.type == sf::Event::MouseMoved) {
            if (currentTool == Tool::Pen && isDrawing) {
                draw();
            }
            else if (currentTool == Tool::Rect && isDrawingRect) {
                updateRect();
            }
            else if (currentTool == Tool::Circle && isDrawingCircle) {
                updateCircle();
            }
        }
    }
}

// ====================================
// drawing 
// ====================================
void PaintApp::startDrawing() {
    saveState();
    currentLine.clear();
    isDrawing = true;
}

void PaintApp::stopDrawing() {
    isDrawing = false;
    currentLine.clear();
}

void PaintApp::draw() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vertex vertex(sf::Vector2f(mousePos), currentColor);
    currentLine.push_back(vertex);

    sf::CircleShape circle(2);
    circle.setPosition(vertex.position - sf::Vector2f(1, 1)); // Center the circle
    circle.setFillColor(currentColor);
    texture.draw(circle);
    texture.display();
}

// ====================================
// rectangle 
// ====================================
void PaintApp::startRect() {
    saveState();
    shapeStartPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    currentRectangle.setPosition(shapeStartPos);
    currentRectangle.setFillColor(sf::Color::Transparent);
    currentRectangle.setOutlineColor(currentColor);
    currentRectangle.setOutlineThickness(1);
    isDrawingRect = true;
}

void PaintApp::updateRect() {
    sf::Vector2f currentPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sf::Vector2f size = currentPos - shapeStartPos;

    sf::Vector2f position = shapeStartPos;

    // Adjust for negative width/height
    if (size.x < 0) {
        position.x += size.x;
        size.x = -size.x;
    }
    if (size.y < 0) {
        position.y += size.y;
        size.y = -size.y;
    }

    currentRectangle.setPosition(position);
    currentRectangle.setSize(size);
}

void PaintApp::stopRect() {
    isDrawingRect = false;
    texture.draw(currentRectangle);
    texture.display();
    currentRectangle.setSize(sf::Vector2f(0, 0));
}

// ====================================
// circle 
// ====================================
void PaintApp::startCircle() {
    saveState();
    shapeStartPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    currentCircle.setPosition(shapeStartPos);
    currentCircle.setFillColor(sf::Color::Transparent);
    currentCircle.setOutlineColor(currentColor);
    currentCircle.setOutlineThickness(1);
    isDrawingCircle = true;
}

void PaintApp::updateCircle() {
    sf::Vector2f currentPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    float radius = std::sqrt(std::pow(currentPos.x - shapeStartPos.x, 2) + std::pow(currentPos.y - shapeStartPos.y, 2));
    currentCircle.setRadius(radius);
    currentCircle.setOrigin(radius, radius);
}

void PaintApp::stopCircle() {
    isDrawingCircle = false;
    texture.draw(currentCircle);
    texture.display();
    currentCircle.setRadius(0);
}

// ====================================
// dropdown 
// ====================================




// ====================================
// undo/redo
// ====================================
void PaintApp::undo() {
    if (!undoStack.empty()) {
        redoStack.push(texture.getTexture().copyToImage());

        tempTexture.loadFromImage(undoStack.top());

        texture.clear(sf::Color::White);
        sf::Sprite tempSprite(tempTexture);
        texture.draw(tempSprite);
        texture.display();

        undoStack.pop();
    }
}

void PaintApp::redo() {
    if (!redoStack.empty()) {
        undoStack.push(texture.getTexture().copyToImage());

        tempTexture.loadFromImage(redoStack.top());

        texture.clear(sf::Color::White);
        sf::Sprite tempSprite(tempTexture);
        texture.draw(tempSprite);
        texture.display();

        redoStack.pop();
    }
}

void PaintApp::saveState() {
    undoStack.push(texture.getTexture().copyToImage());
    while (!redoStack.empty()) {
        redoStack.pop();
    }
}

// render - draws all elements to the window
void PaintApp::render() {
    window.clear();
    window.draw(sprite);

    if (isDrawingRect) {
        window.draw(currentRectangle);
    }

    if (isDrawingCircle) {
        window.draw(currentCircle);
    }

    // render UI 
    toolbar.renderUI();
    window.display();
}