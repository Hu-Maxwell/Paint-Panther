#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP

void handleMouseReleased(bool& isDrawingRectangle, bool& isDrawingPixel, bool& isDrawingCircle);

void handleMousePressed(sf::RenderWindow& window, bool& isDrawingRectangle, bool& isDrawingPixel, bool& isDrawingCircle,
    sf::Vector2i& startPos, int& currentTool, const sf::FloatRect& penButtonBounds,
    const sf::FloatRect& rectangleButtonBounds, const sf::FloatRect& circleButtonBounds,
    const sf::FloatRect& undoButtonBounds, const sf::FloatRect& redoButtonBounds);

void handleDrawingEvents(sf::RenderWindow& window, bool& isDrawingRectangle, bool& isDrawingPixel, bool& isDrawingCircle,
    sf::Vector2i& startPos, int& currentTool, const sf::FloatRect& penButtonBounds,
    const sf::FloatRect& rectangleButtonBounds, const sf::FloatRect& circleButtonBounds,
    const sf::FloatRect& undoButtonBounds, const sf::FloatRect& redoButtonBounds);

#endif