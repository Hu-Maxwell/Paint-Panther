#include "../include/UI.h"

// really long function that initializes all the UI elements
void initUI(sf::Font& font, 
    sf::RectangleShape& undoButton, sf::RectangleShape& redoButton, 
    sf::RectangleShape& penButton, sf::Sprite& penSprite, 
    sf::RectangleShape& rectButton, sf::RectangleShape& circleButton, 
    sf::RectangleShape& toolbarBackground, 
    sf::Text& undoText, sf::Text& redoText, sf::Text& penText, sf::Text& rectText, sf::Text& circleText, 
    sf::RenderWindow& window, sf::Texture& penTexture,
    sf::RectangleShape& dropdownButton, sf::Text& dropdownText, 
    sf::RectangleShape& dropButton1, sf::RectangleShape& dropButton2, sf::RectangleShape& dropButton3) {

    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Error loading font 'arial.ttf'." << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!penTexture.loadFromFile("assets/pen.png")) {
        std::cerr << "Error loading texture 'pen.png'." << std::endl;
        exit(EXIT_FAILURE);
    }

    // toolbar dimensions
    float toolbarHeight = 40.0f;
    float buttonSize = 40.0f;
	sf::Color backgroundColor = sf::Color(200, 200, 200); // gray

    // sets toolbar color to gray
    toolbarBackground.setSize(sf::Vector2f(window.getSize().x, toolbarHeight));
    toolbarBackground.setPosition(0, 0);
    toolbarBackground.setFillColor(backgroundColor);

    // undo
    undoButton.setSize(sf::Vector2f(buttonSize, buttonSize));
    undoButton.setPosition(0, 0);
    undoButton.setFillColor(backgroundColor); 
    undoText.setFont(font);
    undoText.setString("U");
    undoText.setCharacterSize(20);
    undoText.setFillColor(sf::Color::Black);
    undoText.setPosition(15, 5);

    // redo
    redoButton.setSize(sf::Vector2f(buttonSize, buttonSize));
    redoButton.setPosition(buttonSize, 0);
    redoButton.setFillColor(backgroundColor);
    redoText.setFont(font);
    redoText.setString("R");
    redoText.setCharacterSize(20);
    redoText.setFillColor(sf::Color::Black);
    redoText.setPosition(buttonSize + 15, 5);

    // pen
    penButton.setSize(sf::Vector2f(buttonSize, buttonSize));
    penButton.setPosition(2 * buttonSize, 0);
    penButton.setFillColor(backgroundColor);

    // pen sprite
    penSprite.setTexture(penTexture);
    penSprite.setScale(0.05f, 0.05f); 
	penSprite.setPosition(2 * buttonSize + 8, 8); // centers the sprite. 8 just happens to be the best number

    // rectangle 
    rectButton.setSize(sf::Vector2f(buttonSize, buttonSize));
    rectButton.setPosition(3 * buttonSize, 0);
    rectButton.setFillColor(backgroundColor);
    rectText.setFont(font);
    rectText.setString("S");
    rectText.setCharacterSize(20);
    rectText.setFillColor(sf::Color::Black);

    // rectangle sprite
	// TODO: add rectangle sprite
    rectText.setPosition(3 * buttonSize + 15, 5); 

    // circle
    circleButton.setSize(sf::Vector2f(buttonSize, buttonSize));
    circleButton.setPosition(4 * buttonSize, 0);
    circleButton.setFillColor(backgroundColor);
    circleText.setFont(font);
    circleText.setString("C");
    circleText.setCharacterSize(20);
    circleText.setFillColor(sf::Color::Black);

    // circle sprite
	// TODO: add circle sprite
    circleText.setPosition(4 * buttonSize + 15, 5);

    // dropdown
    dropdownButton.setSize(sf::Vector2f(buttonSize, buttonSize));
    dropdownButton.setPosition(5 * buttonSize, 0);
    dropdownButton.setFillColor(backgroundColor);
    dropdownText.setFont(font);
    dropdownText.setString("D");
    dropdownText.setCharacterSize(20);
    dropdownText.setFillColor(sf::Color::Black);
    dropdownText.setPosition(5 * buttonSize + 15, 5);

    // dropdown's three buttons below it 
    dropButton1.setSize(sf::Vector2f(buttonSize, buttonSize));
    dropButton1.setPosition(4 * buttonSize, 1 * buttonSize);
    dropButton1.setFillColor(backgroundColor);

    dropButton2.setSize(sf::Vector2f(buttonSize, buttonSize));
    dropButton2.setPosition(5 * buttonSize, 1 * buttonSize);
    dropButton2.setFillColor(backgroundColor);

    dropButton3.setSize(sf::Vector2f(buttonSize, buttonSize));
    dropButton3.setPosition(6 * buttonSize, 1 * buttonSize);
    dropButton3.setFillColor(backgroundColor);

	// color picker
    
}
