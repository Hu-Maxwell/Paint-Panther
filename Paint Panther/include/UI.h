#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <iostream>

void initUI(sf::Font& font,
    sf::RectangleShape& undoButton, sf::RectangleShape& redoButton,
    sf::RectangleShape& penButton, sf::Sprite& penSprite,
    sf::RectangleShape& rectButton, sf::RectangleShape& circleButton,
    sf::RectangleShape& toolbarBackground,
    sf::Text& undoText, sf::Text& redoText, sf::Text& penText, sf::Text& rectText, sf::Text& circleText,
    sf::RenderWindow& window, sf::Texture& penTexture,
    sf::RectangleShape& dropdownButton, sf::Text& dropdownText,
    sf::RectangleShape& dropButton1, sf::RectangleShape& dropButton2, sf::RectangleShape& dropButton3);

#endif 