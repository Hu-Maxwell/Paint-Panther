#include "../include/paint.hpp"

int main() {
    // creates the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Paint Panther");
    sf::RenderTexture canvas;
    canvas.create(800, 600);
    canvas.clear(sf::Color::White);  

    // game loop!
    while (window.isOpen()) {
        paintLoop(window, canvas); 
    }

    return 0;
}