#include "../include/paint.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

// draws a rectangle at the given mouse pos
void drawRectangle(sf::RenderTexture& canvas, const sf::Vector2i& startPos, const sf::Vector2i& endPos) {
    float width = endPos.x - startPos.x;
    float height = endPos.y - startPos.y;

    sf::RectangleShape rectangle(sf::Vector2f(std::abs(width), std::abs(height)));
    rectangle.setPosition(
        width < 0 ? endPos.x : startPos.x,
        height < 0 ? endPos.y : startPos.y
    );
    rectangle.setFillColor(sf::Color::Black);
    canvas.draw(rectangle);
}

// draws a pixel at the given mouse pos
void drawPixel(sf::RenderTexture& canvas, const sf::Vector2f& mousePosition) {
    sf::CircleShape pixel(2);
    pixel.setPosition(mousePosition);
    pixel.setFillColor(sf::Color::Black);
    canvas.draw(pixel);
}

// handles window events
void handleDrawingEvents(sf::RenderWindow& window, bool& isDrawingSquare, bool& isDrawingPixel,
    sf::Vector2i& startPos, int& currentTool, const sf::FloatRect& penButtonBounds,
    const sf::FloatRect& rectangleButtonBounds) {
    sf::Event event;
    if (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;

        case sf::Event::MouseButtonPressed: {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (event.mouseButton.button == sf::Mouse::Left) {
                if (penButtonBounds.contains(mousePos)) {
                    currentTool = 1; // Pen
                    std::cout << "Pen tool selected" << std::endl;
                }
                else if (rectangleButtonBounds.contains(mousePos)) {
                    currentTool = 2; // Rectangle
                    std::cout << "Rectangle tool selected" << std::endl;
                }
                else if (currentTool == 1) {
                    isDrawingPixel = true;
                }
            }

            if (event.mouseButton.button == sf::Mouse::Left && currentTool == 2) {
                isDrawingSquare = true;
                startPos = sf::Mouse::getPosition(window);
            }
            break;
        }

        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Left) {
                isDrawingPixel = false;
                isDrawingSquare = false;
            }
            break;

        default:
            break;
        }
    }
}

// draws the UI
void drawUI(sf::RenderWindow& window, sf::FloatRect& penButtonBounds, sf::FloatRect& rectangleButtonBounds) {
    sf::RectangleShape toolbar(sf::Vector2f(window.getSize().x, 50));
    toolbar.setFillColor(sf::Color(200, 200, 200));

    sf::RectangleShape penButton(sf::Vector2f(100, 30));
    penButton.setPosition(10, 10);
    penButton.setFillColor(sf::Color(150, 150, 150));
    penButtonBounds = penButton.getGlobalBounds();

    sf::RectangleShape rectangleButton(sf::Vector2f(100, 30));
    rectangleButton.setPosition(120, 10);
    rectangleButton.setFillColor(sf::Color(150, 150, 150));
    rectangleButtonBounds = rectangleButton.getGlobalBounds();

    window.draw(toolbar);
    window.draw(penButton);
    window.draw(rectangleButton);
}

// main drawing loop
void paintLoop(sf::RenderWindow& window, sf::RenderTexture& canvas) {
    bool isDrawingSquare = false;
    bool isDrawingPixel = false;

    sf::Vector2i startPos;
    int currentTool = 0; // 0 = nothing, 1 = pen, 2 = rectangle

    sf::FloatRect penButtonBounds;
    sf::FloatRect rectangleButtonBounds;

    canvas.clear(sf::Color::White);
    canvas.display();

    while (window.isOpen()) {
        handleDrawingEvents(window, isDrawingSquare, isDrawingPixel, startPos, currentTool, penButtonBounds, rectangleButtonBounds);

        if (isDrawingSquare) {
            drawRectangle(canvas, startPos, sf::Mouse::getPosition(window));
        }

        if (isDrawingPixel) {
            sf::Vector2f mouseCoords = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            drawPixel(canvas, mouseCoords);
        }

        window.clear();
        sf::Sprite canvasSprite(canvas.getTexture());
        window.draw(canvasSprite);

        drawUI(window, penButtonBounds, rectangleButtonBounds);

        window.display();
    }
}