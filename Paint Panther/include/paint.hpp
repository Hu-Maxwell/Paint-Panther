#ifndef PAINT_HPP
#define PAINT_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

void drawRectangle(sf::RenderTexture& canvas, const sf::Vector2i& startPos, const sf::Vector2i& endPos);
void drawPixel(sf::RenderTexture& canvas, const sf::Vector2f& mousePosition);
void handleDrawingEvents(sf::RenderWindow& window, bool& isDrawingSquare, bool& isDrawingPixel,
                        sf::Vector2i& startPos, int& currentTool, const sf::FloatRect& penButtonBounds,
                        const sf::FloatRect& rectangleButtonBounds);
void drawUI(sf::RenderWindow& window, sf::FloatRect& penButtonBounds, sf::FloatRect& rectangleButtonBounds);
void paintLoop(sf::RenderWindow& window, sf::RenderTexture& canvas);

#endif 