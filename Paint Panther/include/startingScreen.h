#include <iostream>
#include <SFML/Graphics.hpp>

class StartingScreen {
public:
	StartingScreen(sf::RenderWindow& _window);
	int mainLoop();
	
	sf::RenderWindow& window; 

	sf::Clock clock;
	float timer = 0;


	sf::Font font;

	sf::Text text1;
	sf::Text text2;

	sf::RectangleShape trail1;
	sf::RectangleShape trail2;

	sf::Text buttonText;
	sf::RectangleShape button; 

	sf::Text buttonText2; 
	sf::RectangleShape button2; 

	bool moveOut = false;

	float updatedX = 0; 
	float updatedX2 = 0;
	float updatedY = 0;

	int pollForInput();
	void moveLogoOut(); 

	void moveResizeButtonRight(); 
	void moveDownAnimation();
	void moveRightAnimation();

	// function that paintapp calls to get the resolution 
	void getResolution();
};