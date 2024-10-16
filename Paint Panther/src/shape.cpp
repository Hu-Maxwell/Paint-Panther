#include "../include/paint.hpp"

// draws a rectangle
void drawRectangle(sf::RenderTexture& canvas, const sf::Vector2i& startPos, const sf::Vector2i& endPos) {
    float width = endPos.x - startPos.x;
    float height = endPos.y - startPos.y;

    sf::RectangleShape rectangle(sf::Vector2f(std::abs(width), std::abs(height)));
    rectangle.setPosition(
        width < 0 ? endPos.x : startPos.x,
        height < 0 ? endPos.y : startPos.y
    );
    rectangle.setFillColor(sf::Color::Black);
    canvas.draw(rectangle);
}

// draws a circle
void drawCircle() {

}

// draws a trapezoid (this is hard btw!) 
void drawTrapezoid() {

}