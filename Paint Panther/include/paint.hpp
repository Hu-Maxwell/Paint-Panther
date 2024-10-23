#ifndef PAINT_HPP
#define PAINT_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stack>

#include "../include/shape.hpp"
#include "../include/uiux.hpp"
#include "../include/undo_redo.hpp"
#include "../include/user_input.hpp"

void paintLoop(sf::RenderWindow& window, sf::RenderTexture& canvas);

#endif 