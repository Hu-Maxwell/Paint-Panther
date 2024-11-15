#ifndef PAINTAPP_H
#define PAINTAPP_H

#include <SFML/Graphics.hpp>
#include <stack>
#include <vector>
#include <iostream>

#include "UI.h"
#include "apiHandler.h"
#include "StartScreen.h"

class PaintApp {
public:
    PaintApp();
    void run();

private:
	// variables for canvas (draws to texture -> applied to sprite -> displayed on window) 
    sf::RenderWindow window;
    sf::RenderTexture texture;
    sf::Sprite sprite;

    // ui
    Toolbar toolbar;

    // current color and tool selected
    sf::Color currentColor;
    Tool currentTool = Tool::Pen;

    // ==== tools ==== 

    // undo / redo
    std::stack<sf::Image> undoStack, redoStack;
    sf::Texture tempTexture;

    // pen 
    sf::Vector2i lastMousePos; // for drawing a line between mouse pos last frame and cur frame
    bool isDrawing = false;

    // eraser
    bool isErasing = false; 
    
    // fill 

    // application state for start screen and pain screen
    enum class AppState {
        StartScreen,
        MainApp
    };
    AppState currentState;

    sf::Font font; // Ensure you have a font loaded
    sf::Text titleText;
    sf::RectangleShape startButton;
    sf::Text startButtonText;

    // ==== shapes =====
    sf::Vector2f shapeStartPos;

    StartScreen startScreen;

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

    // functions 
    void handleEvents();
    void handleLeftClick(sf::Event event);
    void handleRightClick(sf::Event event);
    void handleClickRelease(sf::Event event);
    void handleMouseMove(sf::Event event);

    void saveToFile(const std::string& filename);

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
