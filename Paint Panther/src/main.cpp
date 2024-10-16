#include "../include/paint.hpp"

int main() {
    // creates the window
    sf::RenderWindow window(sf::VideoMode(2400, 1800), "Paint Panther");
    sf::RenderTexture canvas;
    canvas.create(2400, 1800);
    canvas.clear(sf::Color::White);  

    // game loop!
    while (window.isOpen()) {
        paintLoop(window, canvas); 
    }

    return 0;
}
