#include "../include/PaintApp.h"

// ====================================
// public functions
// ====================================

// constructor - initializes the window, texture, and current color
PaintApp::PaintApp() : window(sf::VideoMode(800, 600), "Paint2D App"), currentColor(sf::Color::Black) {
    texture.create(800, 600);
    texture.clear(sf::Color::White);
    texture.display();
    sprite.setTexture(texture.getTexture());

    // initialize UI
    initUI(font,
        undoButton, redoButton,
        penButton, penSprite,
        shapeButton, circleButton,
        toolbarBackground,
        undoText, redoText, penText, shapeText, circleText,
        window, penTexture,
        dropdownButton, dropdownText, 
        dropButton1, dropButton2, dropButton3);
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
                if (undoButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    undo();
                }
                else if (redoButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    redo();
                }
                else if (penButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    currentTool = Tool::Pen;
                }
                else if (shapeButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    currentTool = Tool::Shape;
                }
                else if (circleButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    currentTool = Tool::Circle;
                } 
                else if (dropdownButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    openDropdown(); 
                }
                else if (dropButton1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && dropdownIsOpen) {
                    std::cout << "dropdown1 open";
                }
                else if (dropButton2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && dropdownIsOpen) {
                    std::cout << "dropdown2 open";
                }
                else if (dropButton3.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && dropdownIsOpen) {
                    std::cout << "dropdown3 open";
                }
                else {
                    if (currentTool == Tool::Pen) {
                        startDrawing();
                    }
                    else if (currentTool == Tool::Shape) {
                        startShape();
                    }
                    else if (currentTool == Tool::Circle) {
                        startCircle();
                    }
                }
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (currentTool == Tool::Pen) {
                    stopDrawing();
                }
                else if (currentTool == Tool::Shape) {
                    stopShape();
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
            else if (currentTool == Tool::Shape && isDrawingShape) {
                updateShape();
            }
            else if (currentTool == Tool::Circle && isDrawingCircle) {
                updateCircle();
            }
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Z && event.key.control) {
                undo();
            }
            else if (event.key.code == sf::Keyboard::Y && event.key.control) {
                redo();
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
void PaintApp::startShape() {
    saveState();
    shapeStartPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    currentRectangle.setPosition(shapeStartPos);
    currentRectangle.setFillColor(sf::Color::Transparent);
    currentRectangle.setOutlineColor(currentColor);
    currentRectangle.setOutlineThickness(1);
    isDrawingShape = true;
}

void PaintApp::updateShape() {
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

void PaintApp::stopShape() {
    isDrawingShape = false;
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

void PaintApp::openDropdown() {
    if (!dropdownIsOpen) {
        dropdownIsOpen = true;
    }
    else {
        dropdownIsOpen = false; 
    }
}

void PaintApp::drawDropdown() {
    // three more buttons appear 
    window.draw(dropButton1);
    window.draw(dropButton2);
    window.draw(dropButton3); 
}

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

// highlights current selected button
void PaintApp::highlightButton() {
    if (currentTool == Tool::Pen) {
        penButton.setFillColor(sf::Color::Yellow);
        shapeButton.setFillColor(sf::Color(200, 200, 200));
        circleButton.setFillColor(sf::Color(200, 200, 200));
    }
    else if (currentTool == Tool::Shape) {
        penButton.setFillColor(sf::Color(200, 200, 200));
        shapeButton.setFillColor(sf::Color::Yellow);
        circleButton.setFillColor(sf::Color(200, 200, 200));
    }
    else if (currentTool == Tool::Circle) {
        penButton.setFillColor(sf::Color(200, 200, 200));
        shapeButton.setFillColor(sf::Color(200, 200, 200));
        circleButton.setFillColor(sf::Color::Yellow);
    }
}

// render - draws all elements to the window
void PaintApp::render() {
    window.clear();
    window.draw(sprite);

    if (isDrawingShape) {
        window.draw(currentRectangle);
    }

    if (isDrawingCircle) {
        window.draw(currentCircle);
    }

    if (dropdownIsOpen) {
        drawDropdown(); 
    }

    highlightButton();

    window.draw(toolbarBackground);
    window.draw(undoButton);
    window.draw(undoText);
    window.draw(redoButton);
    window.draw(redoText);
    window.draw(penButton);
    window.draw(penSprite); 
    window.draw(shapeButton);
    window.draw(shapeText);
    window.draw(circleButton);
    window.draw(circleText);
	window.draw(dropdownButton);
    window.draw(dropdownText); 
  
    window.display();
}