#include "../include/paint.hpp"

// draws the UI
void drawUI(sf::RenderWindow& window, sf::FloatRect& penButtonBounds, sf::FloatRect& rectangleButtonBounds, sf::FloatRect& circleButtonBounds, sf::FloatRect& undoButtonBounds, sf::FloatRect& redoButtonBounds) {
    sf::RectangleShape toolbar(sf::Vector2f(window.getSize().x, 50));
    toolbar.setFillColor(sf::Color(200, 200, 200));

    sf::RectangleShape penButton(sf::Vector2f(100, 30));
    penButton.setPosition(10, 10);
    penButton.setFillColor(sf::Color(150, 150, 150));
    penButtonBounds = penButton.getGlobalBounds();

    sf::RectangleShape rectangleButton(sf::Vector2f(100, 30));
    rectangleButton.setPosition(120, 10);
    rectangleButton.setFillColor(sf::Color(150, 150, 150));
    rectangleButtonBounds = rectangleButton.getGlobalBounds();

    // circle button
    sf::RectangleShape circleButton(sf::Vector2f(100, 30));
    circleButton.setPosition(230, 10);
    circleButton.setFillColor(sf::Color(150, 150, 150));
    circleButtonBounds = circleButton.getGlobalBounds();

    // undo button
    sf::RectangleShape undoButton(sf::Vector2f(100, 30));
    undoButton.setPosition(340, 10);
    undoButton.setFillColor(sf::Color(150, 150, 150));
    undoButtonBounds = undoButton.getGlobalBounds();

    // redo button
    sf::RectangleShape redoButton(sf::Vector2f(100, 30));
    redoButton.setPosition(450, 10);
    redoButton.setFillColor(sf::Color(150, 150, 150));
    redoButtonBounds = redoButton.getGlobalBounds();

    window.draw(toolbar);
    window.draw(penButton);
    window.draw(rectangleButton);
    window.draw(circleButton);
    window.draw(undoButton);
    window.draw(redoButton);
}