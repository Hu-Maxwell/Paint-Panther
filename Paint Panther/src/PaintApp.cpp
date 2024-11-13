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

// handles all user input
void PaintApp::handleEvents() {
    sf::Event event;
    bool exitLoop = false;

    while (window.pollEvent(event) && !exitLoop) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Color tempColor = toolbar.getRgbOnClick(event); 
                if (tempColor != sf::Color::Black) {
                    currentColor = tempColor; 
                }

                if (toolbar.handleUIInput(event) != Tool::Nothing) {
                    Tool clickedTool = toolbar.handleUIInput(event);

                    toolbar.highlightButton(clickedTool); // Testing this (clicked tool returns the "tool" in the enumerated toolbar class - Use to find the button[i].tool
                    
                    if (clickedTool == Tool::Undo) {
                        undo();
                    }
                    else if (clickedTool == Tool::Redo) {
                        redo();
                    }
                    else if (clickedTool == Tool::SaveFile) {
                        saveToFile("output.png");
                    }
                    else if (clickedTool == Tool::Dropdown) { // thi s is really bad to have toolbar call paintapp then call toolbar
                        toolbar.openDropdown();
                    }
                    else if (clickedTool == Tool::Color) {
                        toolbar.openColorWheel(); 
                    }
                    else if (
                        clickedTool == Tool::Pen || clickedTool == Tool::Eraser || clickedTool == Tool::Fill 
                        || clickedTool == Tool::Rect || clickedTool == Tool::Circle || clickedTool == Tool::Triangle) {
                        currentTool = clickedTool;
                    }
                    exitLoop = true;
                    break;
                }

                if (currentTool == Tool::Pen) {
                    startDrawing();
                }
                if (currentTool == Tool::Eraser) {
                    startErase(); 
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
                else if (currentTool == Tool::Triangle) {
                    startTriangle(); 
                }
            }
            
        }

                else if (currentTool == Tool::Polygon) {
                    if (!isDrawingPolygon) {
                        startPolygon();
                    }
                    else {
                        updatePolygon();
                    }             
                }
                //else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) { //CANNOT GET THIS TO WORK
                //    stopPolygon();
                //}
            }
            if (event.mouseButton.button == sf::Mouse::Right) {
                if (currentTool == Tool::Polygon) {
                    stopPolygon();
                }
            }
        }

        else if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) { // Add the unhighlight case Here - UI/UX
                if (currentTool == Tool::Pen) {
                    stopDrawing();
                }
                if (currentTool == Tool::Eraser) {
                    stopErase(); 
                }
                else if (currentTool == Tool::Rect) {
                    stopRect();
                }
                else if (currentTool == Tool::Circle) {
                    stopCircle();
                }
                else if (currentTool == Tool::Triangle) {
                    stopTriangle();
                }
            }
        }

        else if (event.type == sf::Event::MouseMoved) {
            if (currentTool == Tool::Pen && isDrawing) {
                draw();
            }
            if (currentTool == Tool::Eraser && isErasing) {
                erase();
            }
            else if (currentTool == Tool::Rect && isDrawingRect) {
                updateRect();
            }
            else if (currentTool == Tool::Circle && isDrawingCircle) {
                updateCircle();
            }
            else if (currentTool == Tool::Triangle && isDrawingTriangle) { // isDrawingTriangle
                updateTriangle();
            }
        }
    }
}

// ====================================
// save to file 
// ====================================

// TODO: make it so that the file saved changes depending on what the user inputs
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

// is glitchy here - skips a bit because of rectangle angle changing 
void PaintApp::draw() {
    float lineThickness = 10.0f;
    sf::Vector2i curMousePos = sf::Mouse::getPosition(window);

    if (lastMousePos != curMousePos && lastMousePos.x != -1) {
        // Calculate line length and angle
        float dx = static_cast<float>(curMousePos.x - lastMousePos.x);
        float dy = static_cast<float>(curMousePos.y - lastMousePos.y);
        float length = sqrt(dx * dx + dy * dy);
        float angle = atan2(dy, dx) * 180.0f / 3.14159f;

        // Create a rectangle shape
        sf::RectangleShape rectangle(sf::Vector2f(length, lineThickness)); // 5.0f is the desired thickness
        rectangle.setFillColor(currentColor);

        // Set position and rotation
        rectangle.setPosition(sf::Vector2f(lastMousePos.x + dx / 2.0f, lastMousePos.y + dy / 2.0f));
        rectangle.setRotation(angle);

        // Draw the rectangle
        texture.draw(rectangle);
    }
    lastMousePos = curMousePos;

    texture.display();
}

void PaintApp::stopDrawing() {
    isDrawing = false;
    currentLine.clear();
}

// ====================================
// erase tool 
// ====================================

// TODO: add a circle around cursor that indicates erase radius
void PaintApp::startErase() {
    saveState(); 
    isErasing = true; 
}

void PaintApp::erase() {
    sf::Vector2i curMousePos = sf::Mouse::getPosition(window);

    float eraserRadius = 20.f; 
    sf::CircleShape eraser(eraserRadius);
    eraser.setFillColor(sf::Color::White);
    eraser.setPosition(static_cast<float>(curMousePos.x - eraserRadius),
        static_cast<float>(curMousePos.y - eraserRadius));

    texture.draw(eraser);
    texture.display();
}

void PaintApp::stopErase() {
    isErasing = false; 
}
       

// ====================================
// fill tool 
// ====================================

// TODO: optimize it and fix it glitching when toolbar is clicked
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
// rectangle 
// ====================================
void PaintApp::startRect() {
    saveState();
    shapeStartPos = window.mapPixelToCoords(sf::Mouse::getPosition(window)); // cannot use Vector2i, so it converts w/ window
    currentRectangle.setPosition(shapeStartPos);
    currentRectangle.setFillColor(sf::Color::Red); // was "sf::Color::Transparent" now changed to "sf::Color::Red"
    currentRectangle.setOutlineColor(currentColor);
    currentRectangle.setOutlineThickness(1);
    isDrawingRect = true;
}

void PaintApp::updateRect() {
    sf::Vector2f currentPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sf::Vector2f size = currentPos - shapeStartPos;

    sf::Vector2f position = shapeStartPos;

    // adjusts for negative width/height
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
    currentCircle.setFillColor(sf::Color::Red); // was "sf::Color::Transparent" now changed to "sf::Color::Red"
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
void PaintApp::startPolygon() {
    saveState();
    polygonPoints.clear();
    polygonPoints.push_back(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
    isDrawingPolygon = true;
    std::cout << "Polygon drawing started." << std::endl;
}

void PaintApp::updatePolygon() {
    if (!isDrawingPolygon) return;
    sf::Vector2f currentPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    polygonPoints.push_back(currentPos);
    std::cout << polygonPoints.size() << std::endl;
    // Draw lines between each point
    if (polygonPoints.size() > 1) {
        sf::VertexArray lines(sf::LinesStrip, polygonPoints.size());
        for (int i = 0; i < polygonPoints.size(); ++i) {
            lines[i].position = polygonPoints[i];
            lines[i].color = currentColor;
        }
        window.draw(lines);
    }
}

void PaintApp::stopPolygon() {
    isDrawingPolygon = false;

    if (polygonPoints.size() >= 3) { // Ensure it's a valid polygon
        sf::ConvexShape polygon;
        polygon.setPointCount(polygonPoints.size());
        for (int i = 0; i < polygonPoints.size(); ++i) {
            polygon.setPoint(i, polygonPoints[i]);
        }
        polygon.setFillColor(sf::Color::Red); // Set fill color
        polygon.setOutlineColor(currentColor);
        polygon.setOutlineThickness(1);

        texture.draw(polygon);
        texture.display();
        std::cout << "Polygon drawing completed." << std::endl;
    }
    polygonPoints.clear();
}


// ====================================
// triangle  
// ====================================

void PaintApp::startTriangle() {
    saveState();
    shapeStartPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    isDrawingTriangle = true; 

    currentTriangle.setPointCount(3);
    currentTriangle.setFillColor(sf::Color::Transparent);
    currentTriangle.setOutlineColor(currentColor);
    currentTriangle.setOutlineThickness(2.0f);

    currentTriangle.setPoint(0, shapeStartPos);
    currentTriangle.setPoint(1, shapeStartPos);
    currentTriangle.setPoint(2, shapeStartPos); 
}

void PaintApp::updateTriangle() {
    sf::Vector2f currentPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    sf::Vector2f tipPoint((shapeStartPos.x + currentPos.x) / 2.0f, shapeStartPos.y);
    sf::Vector2f leftBasePoint(shapeStartPos.x, currentPos.y);
    sf::Vector2f rightBasePoint(currentPos.x, currentPos.y);

    currentTriangle.setPoint(0, tipPoint);
    currentTriangle.setPoint(1, leftBasePoint);
    currentTriangle.setPoint(2, rightBasePoint);

    window.clear(sf::Color::White);

    sf::Sprite sprite(texture.getTexture());
    window.draw(sprite);

    window.draw(currentTriangle);

    toolbar.renderUI();

    window.display();
}

void PaintApp::stopTriangle() {
    isDrawingTriangle = false;
    texture.draw(currentTriangle);
    texture.display();

    window.clear(sf::Color::White);

    sf::Sprite finalizedSprite(texture.getTexture());
    window.draw(finalizedSprite);

    toolbar.renderUI();
    window.display();
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

// draws all elements to the window
void PaintApp::render() {
    window.clear();
    window.draw(sprite);

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