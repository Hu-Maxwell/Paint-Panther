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

void PaintApp::draw() {
    float lineThickness = 10.0f;
    sf::Vector2i curMousePos = sf::Mouse::getPosition(window);

    if (lastMousePos.x != -1) {
        float dx = static_cast<float>(curMousePos.x - lastMousePos.x);
        float dy = static_cast<float>(curMousePos.y - lastMousePos.y);
        float distance = std::sqrt(dx * dx + dy * dy);

        // amount of circles drawn each frame depends on how far the last mouse pos is 
        // (distance / radius) approximates the amount of circles that need to be drawn
        // + 1 because, without it, if the distance is really small (1 pix), nothing will be drawn 
        int amountOfCircles = static_cast<int>(distance / (lineThickness / 2)) + 1;
        float stepsX = dx / amountOfCircles;
        float stepsY = dy / amountOfCircles;

        for (int i = 0; i <= amountOfCircles; i++) {
            // calculate pos of circles between
            float x = lastMousePos.x + (stepsX * i);
            float y = lastMousePos.y + (stepsY * i);

            sf::CircleShape circle(lineThickness / 2.0f);
            circle.setFillColor(currentColor);
            circle.setOrigin(lineThickness / 2.0f, lineThickness / 2.0f);
            circle.setPosition(x, y);

            texture.draw(circle);
        }
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

// TODO: move curMousePos so it's global
void PaintApp::startErase() {
    saveState();
    isErasing = true;
}

void PaintApp::erase() {
    sf::Vector2i curMousePos = sf::Mouse::getPosition(window);

    // eraser
    sf::CircleShape eraser(eraserRadius);
    eraser.setFillColor(sf::Color::White);
    eraser.setPosition(static_cast<float>(curMousePos.x - eraserRadius),
        static_cast<float>(curMousePos.y - eraserRadius));

    texture.draw(eraser);
    texture.display();
}

void PaintApp::updateEraserOutline() {
    sf::Vector2i curMousePos = sf::Mouse::getPosition(window);
    sf::Vector2f floatMousePos = sf::Vector2f(static_cast<float>(curMousePos.x - eraserRadius), static_cast<float>(curMousePos.y - eraserRadius));

    eraserOutline.setPosition(floatMousePos);
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

void PaintApp::displayAiResponse() {


    // std font 
    // contentAi
}