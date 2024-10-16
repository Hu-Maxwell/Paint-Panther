#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP


void handleMouseReleased(bool& isDrawingSquare, bool& isDrawingPixel);

void handleMousePressed(sf::RenderWindow& window, bool& isDrawingrectangle, bool& isDrawingPixel,
    sf::Vector2i& startPos, int& currentTool, const sf::FloatRect& penButtonBounds,
    const sf::FloatRect& rectangleButtonBounds);

void handleDrawingEvents(sf::RenderWindow& window, bool& isDrawingrectangle, bool& isDrawingPixel,
    sf::Vector2i& startPos, int& currentTool, const sf::FloatRect& penButtonBounds,
    const sf::FloatRect& rectangleButtonBounds);

#endif