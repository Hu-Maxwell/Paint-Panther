#include "../include/PaintApp.h"

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

