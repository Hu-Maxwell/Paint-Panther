#include "../include/paint.hpp"

int main() {
    // creates the window

    int width = 800; 
    int height = 600; 

    sf::RenderWindow window(sf::VideoMode(width, height), "Paint Panther");
    window.setFramerateLimit(144); 
    
    sf::RenderTexture canvas;
    canvas.create(width, height);
    canvas.clear(sf::Color::White);  

    // game loop!
    while (window.isOpen()) {
        paintLoop(window, canvas); 
    }

    return 0;
}