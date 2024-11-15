#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <iostream>

enum class Tool {
    Undo,
    Redo,
    SaveFile,
    Pen,
    Eraser, 
    Fill, 
    Color, 
    Dropdown,
    Rect, 
    Circle, 
    Polygon, 
    Triangle,
    AI, 
    Nothing
};

class Toolbar {
public:
    struct Button {
        sf::RectangleShape rect;
        sf::RectangleShape background;
        std::shared_ptr<sf::Texture> texture; 
        Tool tool;
        static sf::Color color;

        Button(const std::string filepath, Tool _tool) : tool(_tool) {
            background.setSize(sf::Vector2f(40.0f, 40.0f));
            background.setFillColor(sf::Color(255, 174, 174));

            rect.setSize(sf::Vector2f(40.0f, 40.0f));

            texture = std::make_shared<sf::Texture>();
            if (!texture->loadFromFile(filepath)) {
                std::cerr << "Failed to load texture: " << filepath << std::endl;
                exit(EXIT_FAILURE);
            }
            rect.setTexture(texture.get());  
        }

        void setPosition(float x, float y) {
            background.setPosition(x, y);
            rect.setPosition(x, y);
        }
    };

    Toolbar(sf::RenderWindow& _window);
    void initButtons(std::vector<Button>& buttonVector, float startingPosX, float startingPosY);
    Tool handleUIInput(sf::Event event);
    void openDropdown();
    void checkIfDropdownClicked(sf::Event event);
    void highlightButton(Tool);
    void renderUI();

    sf::Color HSVtoRGB(float h, float s, float v);
    void openColorWheel(); 
    sf::Color getRgbOnClick(sf::Event event);

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

	// cwImage -> cwTexture -> cwSprite cuz sfml is weird
    sf::Image cwImage;
    sf::Texture cwTexture;
	sf::Sprite cwSprite;
    bool colorWheelOpen; 
};

#endif 