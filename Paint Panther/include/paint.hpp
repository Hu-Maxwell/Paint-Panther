#ifndef PAINT_HPP
#define PAINT_HPP

#include <SFML/Graphics.hpp>

void drawSquare(sf::RenderWindow& window, const sf::Vector2i& startPos, const sf::Vector2i& endPos);
void drawPixel(sf::RenderWindow& window, const sf::Vector2i& mousePosition);
void handleDrawingEvents(sf::RenderWindow& window, bool& isDrawing, sf::Vector2i& startPos);
void paintLoop(sf::RenderWindow& window);

#endif 