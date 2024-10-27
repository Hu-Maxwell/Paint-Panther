#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <iostream>

struct button {
    sf::RectangleShape rect;
    sf::Text text;

    button(const std::string& label, sf::Font& font) {
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::White);

        rect.setSize(sf::Vector2f(40.0f, 40.0f)); // Default size
        rect.setFillColor(sf::Color(100, 100, 250)); // Default color
    }
};

class Toolbar {
public:
    Toolbar(sf::RenderWindow& _window);
    void initUI();
    void handleUIInput();
    void openDropdown();
    void highlightButton();
    void renderUI();

    sf::RenderWindow& window;
    sf::Font font;
    
    sf::RectangleShape toolbarBackground;
    std::vector<button> buttons;

    float toolbarHeight; // 40.0f
    float buttonSize; // 40.0f
    sf::Color backgroundColor; // sf::Color(200, 200, 200);  gray
};

#endif 