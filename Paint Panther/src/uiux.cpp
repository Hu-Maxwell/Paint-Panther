#include "../include/paint.hpp"

// draws the UI
void drawUI(sf::RenderWindow& window, sf::FloatRect& penButtonBounds, sf::FloatRect& rectangleButtonBounds) {
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
    

    // undo / redo button


    // 



    window.draw(toolbar);
    window.draw(penButton);
    window.draw(rectangleButton);
}