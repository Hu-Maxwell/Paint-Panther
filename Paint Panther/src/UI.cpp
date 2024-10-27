#include "../include/UI.h"

Toolbar::Toolbar(sf::RenderWindow& _window) : window(_window) {
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Error loading font 'arial.ttf'." << std::endl;
        exit(EXIT_FAILURE);
    }

    sf::RectangleShape undoRect;
    sf::Text undoText;
    button undoButton{ undoRect, undoText };
    buttons.push_back(undoButton);

    toolbarHeight = 40.0f;
    buttonSize = 40.0f;
    backgroundColor = sf::Color(200, 200, 200);

    toolbarBackground.setSize(sf::Vector2f(window.getSize().x, toolbarHeight));
    toolbarBackground.setPosition(0, 0);
    toolbarBackground.setFillColor(backgroundColor);

    initUI(); 
}


void Toolbar::initUI() {
    float xPosition = 0.0f;

    for (button& btn : buttons) { 
        btn.rect.setSize(sf::Vector2f(buttonSize, toolbarHeight));
        btn.rect.setPosition(xPosition, 0); 
        btn.rect.setFillColor(sf::Color(100, 100, 250));  

        btn.text.setFont(font);  
        btn.text.setString("U");  
        btn.text.setCharacterSize(14);  
        btn.text.setFillColor(sf::Color::White);  

        sf::FloatRect textBounds = btn.text.getLocalBounds();
        btn.text.setOrigin(textBounds.left + textBounds.width / 2.0f,
            textBounds.top + textBounds.height / 2.0f);
        btn.text.setPosition(
            btn.rect.getPosition().x + btn.rect.getSize().x / 2.0f,
            btn.rect.getPosition().y + btn.rect.getSize().y / 2.0f);

        xPosition += buttonSize + 5.0f;  
    }
}

void Toolbar::handleUIClick() {
	
}

void Toolbar::openDropdown() {
	
}

void Toolbar::highlightButton() {

}

void Toolbar::renderUI() {
    window.draw(toolbarBackground);

    for (const button& btn : buttons) {
        window.draw(btn.rect);    
        window.draw(btn.text);   
    }
}
