#ifndef PAINT_HPP
#define PAINT_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

#include "../include/shape.hpp"
#include "../include/uiux.hpp"
#include "../include/user_input.hpp"

void drawPixel(sf::RenderTexture& canvas, const sf::Vector2f& mousePosition);

void paintLoop(sf::RenderWindow& window, sf::RenderTexture& canvas);

#endif 