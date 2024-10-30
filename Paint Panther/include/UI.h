#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <iostream>

enum class Tool {
    Undo,
    Redo,
    SaveFile,
    Pen,
    Fill, 
    Dropdown,
    Rect, 
    Circle, 
    Nothing
};

class Toolbar {
public:
    struct Button {
        sf::RectangleShape rect;
        sf::Text text;
        Tool tool;
        static sf::Font font; 
        static sf::Color color; 
        
        Button(const std::string& label, Tool _tool) {
            text.setFont(font);
            text.setString(label);
            text.setCharacterSize(14);
            text.setFillColor(sf::Color::White);

            tool = _tool;

            rect.setSize(sf::Vector2f(40.0f, 40.0f)); // TODO: change this to buttonSize
            rect.setFillColor(sf::Color(100, 100, 250)); // TODO: change this to buttonColor
        }
    };

    Toolbar(sf::RenderWindow& _window);
    void initButtons(std::vector<Button>& buttonVector, float startingPosX, float startingPosY);
    Tool handleUIInput(sf::Event event);
    void openDropdown();
    void highlightButton();
    void renderUI();

    sf::RenderWindow& window;
    sf::Font font;
    
    sf::RectangleShape toolbarBackground;
    std::vector<Button> buttons;

    float toolbarHeight; 
    sf::Vector2f buttonSize; 
    float buttonGap; 
    sf::Color backgroundColor; 

    sf::RectangleShape dropdownRect; 
    std::vector<Button> dropdownButtons; 
    float dropdownWidth;
    float dropdownHeight; // change to vector2 later
    bool dropdownIsOpen; 
};

#endif 