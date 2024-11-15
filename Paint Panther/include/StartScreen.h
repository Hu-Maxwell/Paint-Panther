// StartScreen.h
#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <SFML/Graphics.hpp>

class StartScreen {
private:
    sf::Font font;
    sf::Text titleText;
    sf::RectangleShape startButton;
    sf::Text startButtonText;
    sf::RenderWindow& window;
    bool startClicked;

public:
    // Constructor
    StartScreen(sf::RenderWindow& windowRef);

    // Handle events specific to the start screen
    void handleEvent(const sf::Event& event);

    // Render the start screen elements
    void draw();

    // Check if the start button has been clicked
    bool isStartClicked() const;

    // Reset the startClicked flag
    void reset();
};

#endif // STARTSCREEN_H