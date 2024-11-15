#include <iostream>
#include <SFML/Graphics.hpp>

class StartingScreen {
public:
	StartingScreen(sf::RenderWindow& _window);
	void mainLoop();
	
	sf::RenderWindow& window; 

	sf::Font font;
	sf::Text text1;
	sf::Text text2;
	sf::RectangleShape trail1;
	sf::RectangleShape trail2;

	float updatedX = 0; 

	void pollForInput();

	void moveRightAnimation();

	void displayScreenOne();
	void displayScreenTwo();

	// function that paintapp calls to get the resolution 
	void getResolution();
};