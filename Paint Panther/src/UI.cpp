#include "../include/UI.h"

sf::Font Toolbar::Button::font; 
Toolbar::Toolbar(sf::RenderWindow& _window) : window(_window) {
    if (!Button::font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Error loading font 'arial.ttf'." << std::endl;
        exit(EXIT_FAILURE);
    }

    dropdownIsOpen = false;

    toolbarHeight = 40.0f;
    backgroundColor = sf::Color(200, 200, 200);

    toolbarBackground.setSize(sf::Vector2f(window.getSize().x, toolbarHeight));
    toolbarBackground.setPosition(0, 0);
    toolbarBackground.setFillColor(backgroundColor);

    // ui buttons
    buttonSize.x = 40.0f;
    buttonSize.y = toolbarHeight; 

    sf::Vector2f buttonPos; 
    buttonGap = 5.0f;

    buttons.emplace_back("U", Tool::Undo); 
    buttons.emplace_back("R", Tool::Redo);
    buttons.emplace_back("P", Tool::Pen);
    buttons.emplace_back("F", Tool::Fill); 
    buttons.emplace_back("D", Tool::Dropdown);
    initButtons(buttons, 0, 0);

    // dropdown
    dropdownWidth = (3 * buttonSize.x) + (2 * buttonGap);
    dropdownHeight = buttonSize.x;

    dropdownRect.setSize(sf::Vector2f(dropdownWidth, dropdownHeight));
    dropdownRect.setFillColor(backgroundColor);
    dropdownRect.setPosition(3 * buttonSize.x + 3 * buttonGap, toolbarHeight);

    float dropdownPosX = (3 * buttonSize.x) + (3 * buttonGap);
    float dropdownPosY = toolbarHeight;

    dropdownButtons.emplace_back("R", Tool::Rect);
    dropdownButtons.emplace_back("C", Tool::Circle);

    initButtons(dropdownButtons, dropdownPosX, dropdownPosY);

}

void Toolbar::initButtons(std::vector<Button>& buttonVector, float startingPosX, float startingPosY) {
    float buttonPosX = startingPosX;
    float buttonPosY = startingPosY; 

    for (Button& button : buttonVector) {
        sf::RectangleShape& buttonRect = button.rect; 

        buttonRect.setSize(sf::Vector2f(buttonSize.x, toolbarHeight)); 
        buttonRect.setPosition(buttonPosX, buttonPosY); 

        // center the button's text
        sf::FloatRect textBounds = button.text.getLocalBounds();
        button.text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f); 
        button.text.setPosition(buttonRect.getPosition().x + buttonRect.getSize().x / 2, buttonRect.getPosition().y + buttonRect.getSize().y / 2);
    
        buttonPosX += buttonSize.x + buttonGap; 
    }
}


Tool Toolbar::handleUIInput(sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        for (int i = 0; i < buttons.size(); i++) {
            if (buttons[i].rect.getGlobalBounds().contains(mousePos)) {
                return buttons[i].tool; 
            }
        }
        
        for (int i = 0; i < dropdownButtons.size(); i++) {
            if (dropdownButtons[i].rect.getGlobalBounds().contains(mousePos)) {
                return dropdownButtons[i].tool;
            }
        }
    }
    
    // if user doesn't click on any buttons 
    return Tool::Nothing; 
}

void Toolbar::openDropdown() {
    dropdownIsOpen = true;
}

void Toolbar::highlightButton() {
    // stuff
}

void Toolbar::renderUI() {
    window.draw(toolbarBackground);

    for (const Button& btn : buttons) {
        window.draw(btn.rect);
        window.draw(btn.text);
    }

    if (dropdownIsOpen) {
        window.draw(dropdownRect);
        for (const Button& btn : dropdownButtons) {
            window.draw(btn.rect);
            window.draw(btn.text);
        }
    }
}