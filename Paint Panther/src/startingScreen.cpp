#include <iostream>
#include <SFML/Graphics.hpp>

class StartingScreen {
public:
	StartingScreen() {
		if (font.loadFromFile("assets/arial.ttf")) {
			std::cerr << "Couldn't load 'arial.ttf ddasd'." << std::endl;
			exit(EXIT_FAILURE);
		}

		text.setFont(font);
		text.setString("test");
		text.setPosition(100, 100);
	}

	void mainLoop(sf::RenderWindow& window) {
		while (window.isOpen()) {
			window.draw(text);
			window.display();
		}
	}

private:
	// screen 1 (video)
	// screen 2 (resolution settings)

	// std::pair<int, int> resolution 

	sf::Font font;
	sf::Text text;

	void pollForClick() {

	}

	void displayScreenOne() {

	}

	void displayScreenTwo() {

	}

	// function that paintapp calls to get the resolution 
	void getResolution() {

	}

};