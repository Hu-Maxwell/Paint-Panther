#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <iostream>

struct button {
    sf::RectangleShape rect; 
    sf::Text text;

    button(sf::RectangleShape& _rect, sf::Text _text) {
        rect = _rect;
        text = _text; 
    }
    // sf::Texture; (add this later) 
};

class Toolbar {
public:
    Toolbar(sf::RenderWindow& _window);
    void initUI();
    void handleUIClick();
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