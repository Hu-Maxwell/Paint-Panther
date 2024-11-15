#include "../include/startingScreen.h"

StartingScreen::StartingScreen(sf::RenderWindow& _window) : window(_window) {
	if (!font.loadFromFile("assets/Zain-Regular.ttf")) {
		std::cerr << "Couldn't load 'arial.ttf'." << std::endl;
		exit(EXIT_FAILURE);
	}

	text1.setFont(font);
	text1.setString("Paint");
	text1.setCharacterSize(200); 

	text2.setFont(font);
	text2.setString("Panther");
	text2.setCharacterSize(200);

	trail1.setSize(sf::Vector2f(0, 19));
	trail1.setPosition(0, 243);

	trail2.setSize(sf::Vector2f(0, 19));
	trail2.setPosition(0, 443);
}

void StartingScreen::mainLoop() {
	while (window.isOpen()) {
		window.clear(); 

		pollForInput(); 

		moveRightAnimation();

		window.draw(text1);
		window.draw(text2);

		window.draw(trail1);
		window.draw(trail2);

		window.display();
	}
}

// screen 1 (video)
// screen 2 (resolution settings)

// std::pair<int, int> resolution 

void StartingScreen::pollForInput() {
	sf::Event event;

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}

		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				std::cout << "clicked"; 
			}
		}
	}
}

void StartingScreen::moveRightAnimation() {
	// Define the target x-position (middle of the screen)
	float compoundX = 0;

	float targetX = window.getSize().x / 2.0f;
	float distance = targetX - updatedX;

	// Set a lower initial speed
	static float speed = 7.0f;  // Starting speed, adjust as needed

	// Apply a smaller deceleration factor
	float decelerationFactor = 0.990f;  // Closer to 1.0 means slower deceleration

	// Update the position if it's not at the target position
	if (updatedX < targetX) {
		updatedX += speed;

		// Gradually reduce the speed for a slow deceleration effect
		speed *= decelerationFactor;

		// Stop moving if position is close enough to target
		if (updatedX >= targetX - 1) {  // Adding a tolerance to snap to the target
			updatedX = targetX;  // Snap to the target position for precision
			speed = 0.0f;        // Stop the movement
		}
	}

	compoundX += updatedX; 

	// Set the new position of the text
	text1.setPosition(updatedX, 100);
	text2.setPosition(updatedX, 300);
	trail1.setSize(sf::Vector2f(compoundX + 25, 17));
	trail2.setSize(sf::Vector2f(compoundX + 25, 17));
}

void StartingScreen::displayScreenOne() {

}

void StartingScreen::displayScreenTwo() {

}

// function that paintapp calls to get the resolution 
void StartingScreen::getResolution() {

}