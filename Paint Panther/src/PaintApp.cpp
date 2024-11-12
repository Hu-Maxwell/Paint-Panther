#include "../include/PaintApp.h"

// ====================================
// public functions
// ====================================

// constructor - initializes the window, texture, and current color
PaintApp::PaintApp()
    : window(sf::VideoMode(1600, 1000), "Paint2D App"),
    toolbar(window), 
    currentColor(sf::Color::Red) {
    
    texture.create(1600, 1000);
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
                    else if (clickedTool == Tool::Pen || clickedTool == Tool::Eraser || clickedTool == Tool::Fill || clickedTool == Tool::Rect || clickedTool == Tool::Circle || clickedTool == Tool::Polygon) {
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
                else if (currentTool == Tool::Polygon) {
                    if (!isDrawingPolygon) {
                        startPolygon();
                    }
                    else {
                        updatePolygon();
                    }             
                }
                else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) { //CANNOT GET THIS TO WORK
                    stopPolygon();
                }
            }
        }

        else if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
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
    shapeStartPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
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
    // Draw lines between each consecutive point for visual feedback
    if (polygonPoints.size() > 1) {
        sf::VertexArray lines(sf::LinesStrip, polygonPoints.size());
        for (size_t i = 0; i < polygonPoints.size(); ++i) {
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
        for (size_t i = 0; i < polygonPoints.size(); ++i) {
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

/* 
take the starting mouse position of the click, and the current mouse position
there are three points of a triangle, the tip, and the bottom 2 feet
the tip of the triangle should be at the (startMousePos.x + curMousePos.x) / y, startMousePos.y 
the left base of the triangle should be at startingMousePos.x, curMousePos.y
the right base of the trinagle should be at curMousePos.x and curMousePos.y
*/
void PaintApp::startTriangle() {
    saveState();
    shapeStartPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    isDrawingTriangle = true; 
}

void PaintApp::updateTriangle() {
    sf::Vector2f currentPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
}

void PaintApp::stopTriangle() {
    isDrawingTriangle = false; 
    texture.display();
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

    if (isDrawingPolygon && polygonPoints.size() > 1) {
        sf::VertexArray lines(sf::LinesStrip, polygonPoints.size());
        for (size_t i = 0; i < polygonPoints.size(); ++i) {
            lines[i].position = polygonPoints[i];
            lines[i].color = currentColor;
        }
        window.draw(lines);
    }

    // render UI 
    toolbar.renderUI();
    window.display();
}