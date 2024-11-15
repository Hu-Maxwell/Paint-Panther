#ifndef PAINTAPP_H
#define PAINTAPP_H

#include <SFML/Graphics.hpp>
#include <string>
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
    bool dropdownSaveIsOpen = false;

    Tool currentTool = Tool::Pen;

    // ==== drawing tools ==== 

    // pen 
    sf::Vector2i lastMousePos; // for drawing a line between mouse pos last frame and cur frame

    // eraser
    bool isErasing; 
    
    // fill 


    // ==== shapes =====
    sf::Vector2f shapeStartPos;

    // rect
    bool isDrawingRect = false;
    sf::RectangleShape currentRectangle;

	// circle 
    bool isDrawingCircle = false;
    sf::CircleShape currentCircle;

    // triangle
    bool isDrawingTriangle = false;
    sf::ConvexShape currentTriangle;

    // polygon
    bool isDrawingPolygon = false;
    sf::ConvexShape currentPolygon;

    void handleEvents();
    void handleLeftClick(sf::Event event);
    void handleRightClick(sf::Event event);
    void handleClickRelease(sf::Event event);
    void handleMouseMove(sf::Event event);

    // save file and its dropdown
    void saveToFile(const std::string& filename);

    sf::RectangleShape saveFileButton; //this is the button that opens the menu.  find a way to make this the save button
    bool saveIsOpen;
    sf::RectangleShape saveFileInputBox; //input box
    std::string saveFileInputString; //user's typing
    size_t saveFileCursorPos; //cursor pos
    //end of save to file stuff
    //this stuff isn't doing anything ^^^

    void startDrawing();
    void draw();
    void stopDrawing();

    void startErase(); 
    void erase();
    void stopErase(); 

    void fill(); 
    void floodFill(sf::Image& image, int x, int y, const sf::Color& targetColor, const sf::Color& replacementColor);

    void startRect();
    void updateRect();
    void stopRect();

    void startCircle();
    void updateCircle();
    void stopCircle();

    void startTriangle();
    void updateTriangle();
    void stopTriangle();

    std::vector<sf::Vector2f> polygonPoints;
    void startPolygon();
    void updatePolygon();
    void stopPolygon();

    void undo();
    void redo();
    void saveState();

    void render();
};

#endif 
