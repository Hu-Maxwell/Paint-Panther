#ifndef PAINT_HPP
#define PAINT_HPP

#include <SFML/Graphics.hpp>

void drawSquare(sf::RenderTexture& canvas, sf::Vector2i& startPos, sf::Vector2i& endPos, sf::Vector2u windowSize);
void drawPixel(sf::RenderTexture& canvas, const sf::Vector2i& mousePosition);
void handleDrawingEvents(sf::RenderWindow& window, bool& isDrawing, sf::Vector2i& startPos);
void paintLoop(sf::RenderWindow& window, sf::RenderTexture& canvas); 

#endif 