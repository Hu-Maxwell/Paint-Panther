#ifndef PAINTAPP_H
#define PAINTAPP_H

#include <SFML/Graphics.hpp>
#include <stack>
#include <vector>
#include <iostream>
#include "UI.h"

class PaintApp {
public:
    PaintApp();
    void run();

private:
    sf::RenderWindow window;
    sf::RenderTexture texture;
    sf::Sprite sprite;
    sf::Color currentColor;
    std::vector<sf::Vertex> currentLine;
    std::stack<sf::Image> undoStack, redoStack;
    bool isDrawing = false;
    bool dropdownIsOpen = false; 
    sf::Texture penTexture;
    sf::Sprite penSprite; 

    // UI elements
    sf::Font font;
    sf::RectangleShape undoButton, redoButton, penButton, shapeButton, circleButton, toolbarBackground;
    sf::RectangleShape dropdownButton, dropButton1, dropButton2, dropButton3;
    sf::Text undoText, redoText, penText, shapeText, circleText, dropdownText;

    sf::Texture tempTexture; // member variable to keep texture within scope

	// tool enum
    enum class Tool {
        Pen,
        Shape, // rename this to rect later
        Circle
    };

    Tool currentTool = Tool::Pen;

    // rect drawing var 
    // TODO: rename this to rect later
    bool isDrawingShape = false;
    sf::Vector2f shapeStartPos;
    sf::RectangleShape currentRectangle;

	// circle drawing var
    bool isDrawingCircle = false;
    sf::CircleShape currentCircle;

    void handleEvents();
    void startDrawing();
    void stopDrawing();
    void draw();
    void startShape();
    void updateShape();
    void stopShape();
    void startCircle();
    void openDropdown(); 
    void drawDropdown(); 
    void updateCircle();
    void stopCircle();
    void undo();
    void redo();
    void saveState();
    void highlightButton(); 
    void render();
};

#endif 
