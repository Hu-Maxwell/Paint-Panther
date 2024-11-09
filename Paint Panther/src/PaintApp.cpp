#include "../include/PaintApp.h"

// ====================================
// public functions
// ====================================

// constructor - initializes the window, texture, and current color
PaintApp::PaintApp()
    : window(sf::VideoMode(800, 600), "Paint2D App"),
    toolbar(window), 
    currentColor(sf::Color::Red) {
    
    texture.create(1600, 1200);
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
    bool exitLoop = false; 

    while (window.pollEvent(event) && !exitLoop) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                if (toolbar.handleUIInput(event) != Tool::Nothing) {
                    Tool clickedTool = toolbar.handleUIInput(event);
                    if (clickedTool == Tool::Undo) {
                        undo();
                    }
                    else if (clickedTool == Tool::Redo) {
                        redo();
					}
					else if (clickedTool == Tool::SaveFile) {
						saveToFile("output.png");
					}
                    else if (clickedTool == Tool::Dropdown) {
                        toolbar.openDropdown(); 
                    }
                    else if (clickedTool == Tool::Pen || clickedTool == Tool::Eraser || clickedTool == Tool::Fill || clickedTool == Tool::Rect || clickedTool == Tool::Circle) {
                        currentTool = clickedTool;
                    }
                    exitLoop = true;
                    break; 
                }

                if (currentTool == Tool::Pen) {
                    startDrawing();
                } 
                if (currentTool == Tool::Fill) {
                    fill(); 
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
            if (currentTool == Tool::Eraser) {
                erase(); 
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
// save to file 
// ====================================

void PaintApp::saveToFile(const std::string& filename) {
	texture.getTexture().copyToImage().saveToFile(filename);
    std::cout << "Saved file!" << std::endl;
}

// ====================================
// drawing 
// ====================================
void PaintApp::startDrawing() {
    saveState();
    currentLine.clear();
    lastMousePos.x = -1; 
    isDrawing = true;
}

void PaintApp::stopDrawing() {
    isDrawing = false;
    currentLine.clear();
}

// curMousePos.x - lastMousePos.x
// curMousePos.y - lastMousePos.y
// sf::RectangleShape rect;
// rect.setPosition(lastMousePos.x, lastMousePos.y);
// it's ok to make a line thickness variable

void PaintApp::draw() {
    sf::Vector2i curMousePos = sf::Mouse::getPosition(window);
    
    if (lastMousePos != curMousePos && lastMousePos.x != -1) {
        sf::VertexArray line(sf::Lines, 2);
        line[0].position = sf::Vector2f(static_cast<float>(lastMousePos.x), static_cast<float>(lastMousePos.y));
        line[1].position = sf::Vector2f(static_cast<float>(curMousePos.x), static_cast<float>(curMousePos.y));

        line[0].color = currentColor;
        line[1].color = currentColor;

        texture.draw(line);
    }
    lastMousePos = curMousePos;

    texture.display();
}

// ====================================
// erase tool 
// ====================================

// draw white lines
// make the line thicker
void PaintApp::erase() {
    
}

// ====================================
// fill tool 
// ====================================

void PaintApp::fill() {
    saveState();

    sf::Image image = texture.getTexture().copyToImage();

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    int x = static_cast<int>(worldPos.x);
    int y = static_cast<int>(worldPos.y);

    if (x < 0 || x >= static_cast<int>(image.getSize().x) || y < 0 || y >= static_cast<int>(image.getSize().y))
        return;

    sf::Color targetColor = image.getPixel(x, y);

    if (targetColor == currentColor)
        return;

    floodFill(image, x, y, targetColor, currentColor);

    tempTexture.loadFromImage(image);
    sf::Sprite tempSprite(tempTexture);

    texture.clear(sf::Color::Transparent);
    texture.draw(tempSprite);
    texture.display();
}

void PaintApp::floodFill(sf::Image& image, int x, int y, const sf::Color& targetColor, const sf::Color& replacementColor) {
    std::stack<sf::Vector2i> pixels;
    pixels.push(sf::Vector2i(x, y));

    while (!pixels.empty()) {
        sf::Vector2i p = pixels.top();
        pixels.pop();

        int px = p.x;
        int py = p.y;

        if (px < 0 || px >= static_cast<int>(image.getSize().x) || py < 0 || py >= static_cast<int>(image.getSize().y))
            continue;

        if (image.getPixel(px, py) != targetColor)
            continue;

        image.setPixel(px, py, replacementColor);

        pixels.push(sf::Vector2i(px + 1, py));
        pixels.push(sf::Vector2i(px - 1, py));
        pixels.push(sf::Vector2i(px, py + 1));
        pixels.push(sf::Vector2i(px, py - 1));
    }
}

// ====================================
// color wheel
// ====================================

void PaintApp::openColorWheel() {
    // 
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
// polygon  
// ====================================


// ====================================
// triangle  
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