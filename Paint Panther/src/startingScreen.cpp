#include "../include/StartScreen.h"
#include <iostream>

// Constructor
StartScreen::StartScreen(sf::RenderWindow& windowRef)
    : window(windowRef), startClicked(false)
{
    // Load the font
    if (!font.loadFromFile("assets/arial.ttf")) { // Ensure the path is correct
        std::cerr << "Error: Could not load font 'assets/arial.ttf'." << std::endl;
        // Handle error appropriately, e.g., set a default font or exit
    }

    // Initialize Title Text
    titleText.setFont(font);
    titleText.setString("Paint Panther");
    titleText.setCharacterSize(72); // Adjust size as needed
    titleText.setFillColor(sf::Color::Black);
    // Center the title
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    titleText.setPosition(window.getSize().x / 2, window.getSize().y / 3); // Positioned at one-third height

    // Initialize Start Button
    startButton.setSize(sf::Vector2f(200, 60)); // Width: 200px, Height: 60px
    startButton.setFillColor(sf::Color(100, 100, 250)); // Blueish color
    startButton.setOutlineColor(sf::Color::Black);
    startButton.setOutlineThickness(2.0f);
    // Center the button
    startButton.setOrigin(startButton.getSize().x / 2, startButton.getSize().y / 2);
    startButton.setPosition(window.getSize().x / 2, window.getSize().y / 2); // Center of window

    // Initialize Start Button Text
    startButtonText.setFont(font);
    startButtonText.setString("Start");
    startButtonText.setCharacterSize(32); // Adjust size as needed
    startButtonText.setFillColor(sf::Color::White);
    // Center the text within the button
    sf::FloatRect buttonTextBounds = startButtonText.getLocalBounds();
    startButtonText.setOrigin(buttonTextBounds.width / 2, buttonTextBounds.height / 2);
    startButtonText.setPosition(startButton.getPosition());
}

// Handle events specific to the start screen
void StartScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        // Change button color on hover
        if (startButton.getGlobalBounds().contains(mousePos)) {
            startButton.setFillColor(sf::Color(150, 150, 250)); // Lighter blue
        }
        else {
            startButton.setFillColor(sf::Color(100, 100, 250)); // Original color
        }
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (startButton.getGlobalBounds().contains(mousePos)) {
            startClicked = true;
        }
    }

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter) {
            startClicked = true;
        }
    }
}

// Render the start screen elements
void StartScreen::draw() {
    window.draw(titleText);
    window.draw(startButton);
    window.draw(startButtonText);
}

// Check if the start button has been clicked
bool StartScreen::isStartClicked() const {
    return startClicked;
}

// Reset the startClicked flag
void StartScreen::reset() {
    startClicked = false;
}