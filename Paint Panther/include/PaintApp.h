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

    Toolbar toolbar; 

    std::stack<sf::Image> undoStack, redoStack;
    sf::Texture tempTexture; 

    bool isDrawing = false;
    bool dropdownIsOpen = false;

	// tool enum
    enum class Tool {
        Pen,
        Shape, // rename this to rect later
        Circle
    };

    Tool currentTool = Tool::Pen;

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
    void updateCircle();
    void stopCircle();

    void undo();
    void redo();
    void saveState();

    void render();
};

#endif 
