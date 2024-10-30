#ifndef PAINTAPP_H
#define PAINTAPP_H

#include <SFML/Graphics.hpp>
#include <stack>
#include <vector>
#include <iostream>

#include "../include/UI.h"

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

    Tool currentTool = Tool::Pen;

    // ==== drawing tools ==== 

    // pen 
    sf::Vector2i lastMousePos; // for drawing a line between mouse pos last frame and cur frame

    // eraser

    
    // fill 


    // ==== shapes =====
    sf::Vector2f shapeStartPos;

    // rect
    bool isDrawingRect = false;
    sf::RectangleShape currentRectangle;

	// circle 
    bool isDrawingCircle = false;
    sf::CircleShape currentCircle;


    void handleEvents();

    void saveToFile(const std::string& filename);

    void startDrawing();
    void stopDrawing();
    void draw();

    void fill(); 
    void floodFill(sf::Image& image, int x, int y, const sf::Color& targetColor, const sf::Color& replacementColor);

    void startRect();
    void updateRect();
    void stopRect();

    void startCircle();
    void updateCircle();
    void stopCircle();

    void undo();
    void redo();
    void saveState();

    void render();
};

#endif 
