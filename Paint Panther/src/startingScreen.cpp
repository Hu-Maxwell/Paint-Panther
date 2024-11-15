#include "../include/startingScreen.h"

StartingScreen::StartingScreen(sf::RenderWindow& _window) : window(_window) {
	if (!font.loadFromFile("assets/Zain-Regular.ttf")) {
		std::cerr << "Couldn't load 'arial.ttf'." << std::endl;
		exit(EXIT_FAILURE);
	}

	text1.setFont(font);
	text1.setString("Paint");
	text1.setCharacterSize(200); 
	text1.setFillColor(sf::Color::Black);

	text2.setFont(font);
	text2.setString("Panther");
	text2.setCharacterSize(200);
	text2.setFillColor(sf::Color::Black);

	trail1.setPosition(0, 243);
	trail1.setFillColor(sf::Color::Black);

	trail2.setPosition(0, 443);
	trail2.setFillColor(sf::Color::Black);

	button.setSize(sf::Vector2f(500, 150));
	buttonText.setFont(font); 
	buttonText.setString("Start"); 
	buttonText.setCharacterSize(150);
	buttonText.setFillColor(sf::Color::Black); 

	button2.setSize(sf::Vector2f(1000, 200));
	button2.setPosition(window.getSize().x * 2, window.getSize().y * 2);
	button2.setFillColor(sf::Color::Black); 
	buttonText2.setPosition(window.getSize().x * 2, window.getSize().y * 2);
	buttonText2.setFont(font);
	buttonText2.setString("Resize window?");
	buttonText2.setCharacterSize(150);
	buttonText2.setFillColor(sf::Color::White);
}

int StartingScreen::mainLoop() {
	while (window.isOpen()) {
		window.clear(sf::Color(255, 174, 174, 100)); // web theme sf::Color(255, 174, 174, 100)

		int res = pollForInput();
		if (res == -1) {
			return res; 
			// close window
		}
		else if (res == 1) {
			// resize window
			return res; 
		}

		if (moveOut) {
			moveLogoOut();
			moveDownAnimation();
			moveResizeButtonRight(); 
		}
		else {
			moveRightAnimation();
		}

		window.draw(text1);
		window.draw(text2);

		window.draw(trail1);
		window.draw(trail2);

		window.draw(button); 
		window.draw(buttonText); 

		window.draw(button2);
		window.draw(buttonText2); 

		window.display();
	}
}

// screen 1 (video)
// screen 2 (resolution settings)

// std::pair<int, int> resolution 

int StartingScreen::pollForInput() {
	sf::Event event;

	float deltaTime = clock.getElapsedTime().asMilliseconds(); 
	clock.restart(); 

	timer += deltaTime;

	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			return -1; 
		}

		if (event.type == sf::Event::MouseButtonPressed) {
			if (event.mouseButton.button == sf::Mouse::Left) {
				if (timer >= 1500 && !moveOut) {
					moveOut = true;
					timer = 0;
				} 
				if (moveOut && timer >= 1500) {
					sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					if (button2.getGlobalBounds().contains(mousePos)) {
						return 1; 
					}
				}
			}
		}
	}
}

void StartingScreen::moveResizeButtonRight() {
	float targetX = window.getSize().x * 0.75f;

	static float speed = 10.0f;
	float decelerationFactor = 0.990f;

	if (updatedX2 < targetX) {
		updatedX2 += speed;

		speed *= decelerationFactor;

		if (updatedX2 >= targetX - 1) {
			updatedX2 = targetX;
			speed = 0.0f;
		}
	}

	button2.setPosition(updatedX2 - 1000, 243);
	buttonText2.setPosition(button2.getPosition().x + (button2.getSize().x / 8), button2.getPosition().y + (button2.getSize().y / 8));
}

void StartingScreen::moveLogoOut() {
	float compoundX = 0;
	float targetX = window.getSize().x;

	static float speed = 11.0f;
	float decelerationFactor = 0.990f;

	if (updatedX < targetX) {
		updatedX += speed;

		speed *= decelerationFactor;

		if (updatedX >= targetX - 1) {
			updatedX = targetX;
			speed = 0.0f;
		}
	}

	compoundX += updatedX;

	text1.setPosition(updatedX, 100);
	text2.setPosition(updatedX, 300);

	float lastTrailLen = trail1.getSize().x; 
	trail1.setSize(sf::Vector2f(compoundX + 25, 17));
	trail2.setSize(sf::Vector2f(compoundX + 25, 17));
}

void StartingScreen::moveDownAnimation() {
	float targetY = window.getSize().y;

	static float speed = 0.5f;
	float decelerationFactor = 0.980f;

	if (updatedY < targetY) {
		updatedY += speed;

		speed *= decelerationFactor;

		if (updatedY >= targetY - 1) {
			updatedY = targetY;
			speed = 0.0f;
		}
	}

	button.setPosition(button.getPosition().x, button.getPosition().y + updatedY);
	buttonText.setPosition(button.getPosition().x + (button.getSize().x / 4), button.getPosition().y - (button.getSize().y / 4));
}

void StartingScreen::moveRightAnimation() {
	float compoundX = 0;

	float targetX = window.getSize().x / 2.0f;
	float distance = targetX - updatedX;

	static float speed = 7.0f;  

	float decelerationFactor = 0.990f;  

	if (updatedX < targetX) {
		updatedX += speed;

		speed *= decelerationFactor;

		if (updatedX >= targetX - 1) {  
			updatedX = targetX;  
			speed = 0.0f;        
		}
	}

	compoundX += updatedX; 

	text1.setPosition(updatedX, 100);
	text2.setPosition(updatedX, 300);

	trail1.setSize(sf::Vector2f(compoundX + 25, 17));
	trail2.setSize(sf::Vector2f(compoundX + 25, 17));

	button.setPosition(window.getSize().x - updatedX, 700); 
	buttonText.setPosition(button.getPosition().x + (button.getSize().x / 4), button.getPosition().y - (button.getSize().y / 4));
}

// function that paintapp calls to get the resolution 
void StartingScreen::getResolution() {

}