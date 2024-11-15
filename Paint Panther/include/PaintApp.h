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
    sf::RenderWindow saveWindow;
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
    //this stuff isn't doing anything
    //sf::RectangleShape baseBox;
    //std::string inputString;
    //size_t cursorPos;
    //bool isOpen;

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
class Dropdown {
public:
    Dropdown(/*const sf::Font& font, */const sf::Vector2f& position) //ive removed the note for the font because it isn't working rn
        : isOpen(true), cursorPos(0) { //opened by default
        //this is for the piece behind the input box
        background.setSize(sf::Vector2f(420, 170)); // Slightly larger than the input box
        background.setFillColor(sf::Color(200,200,200)); // backgroundColor
        background.setPosition(position.x - 10, position.y + 10); // Offset to surround the input box

        //inputbox always visible?
        inputBox.setSize(sf::Vector2f(400, 50));
        inputBox.setFillColor(sf::Color::White);
        inputBox.setOutlineThickness(2);
        inputBox.setOutlineColor(sf::Color::Black);
        inputBox.setPosition(position.x, position.y + 110);

        //font
        inputText.setFont(font);
        inputText.setCharacterSize(20);
        inputText.setFillColor(sf::Color::Red);
        inputText.setPosition(position.x + 10, position.y + 60);

        //initialize string
        inputString = "";
        

    }

    void handleEvents(const sf::Event& event, const sf::Vector2i& mousePos) {
        if (event.type == sf::Event::MouseButtonPressed) {
            //close dropdown when clicking outside
            if (isOpen && !background.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                isOpen = false;
            }
        }

        if (isOpen && event.type == sf::Event::TextEntered) {
            if (event.text.unicode == '\b') {
                //for backspace
                if (!inputString.empty()) {
                    inputString.pop_back();
                    cursorPos = std::max(cursorPos - 1, static_cast<size_t>(0));

                }
            }

            else if (event.text.unicode == '\r') {
                //for enter key

                std::cout << inputString << std::endl;
                //void setText(std::string grah) {

                //}
                std::string inputGetter = inputString;
                
                //std::cout << userInput << std::endl;
                isOpen = false;
            }
            else if (event.text.unicode < 128) {
                inputString += static_cast<char>(event.text.unicode);
                cursorPos++;
            }
            //userInput = inputString;
            //std::string userInput = inputString;
            //std::cout << userInput << std::endl;
           
            inputText.setString(inputString);
        }
        
    }

    std::string getSalary() {
        return inputGetter;
        std::cout << inputGetter << std::endl;
    }
    
    
    void draw(sf::RenderWindow& window) {

        if (isOpen) {
            window.draw(background);
            window.draw(inputBox);
            window.draw(inputText);

            //input text as rectangles (lines) or something
            for (size_t i = 0; i < inputString.size(); ++i) {
                sf::RectangleShape charRect(sf::Vector2f(10, 30));
                charRect.setFillColor(sf::Color::Black);
                charRect.setPosition(inputBox.getPosition().x + 10 + (i * 12), inputBox.getPosition().y + 10); //spacing in box
                window.draw(charRect);
            }

            //blinking line for cursor cause im so cool (idk why it doesnt appear)
            if (isOpen && cursorPos < inputString.size()) {
                sf::RectangleShape cursor(sf::Vector2f(2, 30)); //THE LINE
                cursor.setFillColor(sf::Color::Black);
                cursor.setPosition(inputBox.getPosition().x + 10 + (cursorPos * 12), inputBox.getPosition().y + 10);
                window.draw(cursor);
            }
        }
        else {
            window.
        }
    }

    const std::string& getInput() const { return inputString; }

    // ====================================
    // private functions
    // ====================================
private:
    bool isOpen;

    sf::Font font; //font
    sf::RectangleShape background; //back box
    sf::RectangleShape inputBox; //input box
    std::string inputString; //user's typing
    sf::Text inputText; //displayed text?
    std::string inputGetter; //probably gonna work
    size_t cursorPos; //cursor pos
};

#endif 
