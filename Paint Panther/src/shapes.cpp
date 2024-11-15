#include "../include/PaintApp.h"

// ====================================
// rectangle 
// ====================================
void PaintApp::startRect() {
    saveState();
    shapeStartPos = window.mapPixelToCoords(sf::Mouse::getPosition(window)); // cannot use Vector2i, so it converts w/ window
    currentRectangle.setPosition(shapeStartPos);
    currentRectangle.setFillColor(sf::Color::Transparent); 
    currentRectangle.setOutlineColor(currentColor);
    currentRectangle.setOutlineThickness(5.0f);
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
    currentCircle.setFillColor(sf::Color::Transparent); 
    currentCircle.setOutlineColor(currentColor);
    currentCircle.setOutlineThickness(5.0f);
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
// triangle  
// ====================================

void PaintApp::startTriangle() {
    saveState();
    shapeStartPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    isDrawingTriangle = true;

    currentTriangle.setPointCount(3);
    currentTriangle.setFillColor(sf::Color::Transparent);
    currentTriangle.setOutlineColor(currentColor);
    currentTriangle.setOutlineThickness(5.0f);

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
}

void PaintApp::stopTriangle() {
    isDrawingTriangle = false;

    texture.draw(currentTriangle);
    texture.display();
}

// ====================================
// polygon  
// ====================================
void PaintApp::startPolygon() {
    saveState();
    isDrawingPolygon = true;

    polygonPoints.clear();

    polygonPoints.push_back(window.mapPixelToCoords(sf::Mouse::getPosition(window)));

    std::cout << "Polygon drawing started." << std::endl;
}

void PaintApp::updatePolygon() {
    if (!isDrawingPolygon) return;

    sf::Vector2f currentPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    polygonPoints.push_back(currentPos);

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

        polygon.setFillColor(currentColor);
        polygon.setOutlineColor(currentColor);
        polygon.setOutlineThickness(1);

        texture.draw(polygon);
        texture.display();
    }

    polygonPoints.clear();
}