#include "../include/paint.hpp"

int main() {
    // creates the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Paint Panther");

    // game loop!
    while (window.isOpen()) {
        paintLoop(window); 
    }

    return 0;
}
