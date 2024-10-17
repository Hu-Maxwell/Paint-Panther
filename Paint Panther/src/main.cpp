#include "../include/paint.hpp"

int main() {
    // creates the window

    int width = 500; 
    int height = 500; 

    sf::RenderWindow window(sf::VideoMode(width, height), "Paint Panther");
    sf::RenderTexture canvas;
    canvas.create(width, height);
    canvas.clear(sf::Color::White);  

    // game loop!
    while (window.isOpen()) {
        paintLoop(window, canvas); 
    }

    return 0;
}