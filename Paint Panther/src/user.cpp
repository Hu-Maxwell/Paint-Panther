#include "../include/PaintApp.h"

void PaintApp::handleEvents() {
    sf::Event event;
    
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (currentState == AppState::StartScreen){
            // Handle events for Start Screen

            // Handle mouse movement for hover effect
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Change button color on hover
                if (startButton.getGlobalBounds().contains(mousePos)) {
                    startButton.setFillColor(sf::Color(150, 150, 250)); // Lighter blue
                }
                else {
                    startButton.setFillColor(sf::Color(100, 100, 250)); // Original color
                }
            }

            // Handle mouse button press
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Check if the mouse click is within the start button bounds
                if (startButton.getGlobalBounds().contains(mousePos)) {
                    currentState = AppState::MainApp; // Switch to MainApp state
                }
            }

            // Handle key press (e.g., Enter key to start)
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    currentState = AppState::MainApp;
                }
            }
        }
        else if (currentState == AppState::MainApp){
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    handleLeftClick(event);
                }
            }

            // though it's repeat code, it's more organized this way
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Right) {
                    handleRightClick(event);
                }
            }

            else if (event.type == sf::Event::MouseButtonReleased) {
                handleClickRelease(event);
            }

            else if (event.type == sf::Event::MouseMoved) {
                handleMouseMove(event);
            }
        }
    }
}

void PaintApp::handleLeftClick(sf::Event event) {
    // manages color
    sf::Color tempColor = toolbar.getRgbOnClick(event);
    if (tempColor != sf::Color::Black) {
        currentColor = tempColor;
    }

    // manages UI
    if (toolbar.handleUIInput(event) != Tool::Nothing) {
        Tool clickedTool = toolbar.handleUIInput(event);

        toolbar.highlightButton(clickedTool); 

        if (clickedTool == Tool::Undo) {
            undo();
        }
        else if (clickedTool == Tool::Redo) {
            redo();
        }
        else if (clickedTool == Tool::SaveFile) {
            saveToFile("output.png");
        }
        else if (clickedTool == Tool::Dropdown) { // TODO: this is really bad to have toolbar call paintapp then call toolbar
            toolbar.openDropdown();
        }
        else if (clickedTool == Tool::Color) {
            toolbar.openColorWheel();
        }
        else if (clickedTool == Tool::AI) {
            std::cout << getResponse() << std::endl;
        }
        else if (
            clickedTool == Tool::Pen || clickedTool == Tool::Eraser || clickedTool == Tool::Fill ||
            clickedTool == Tool::Rect || clickedTool == Tool::Circle ||
            clickedTool == Tool::Triangle || clickedTool == Tool::Polygon) {
            currentTool = clickedTool;
        }
        return; 
    }

    // manages tools
    if (currentTool == Tool::Pen) {
        startDrawing();
    }
    if (currentTool == Tool::Eraser) {
        startErase();
    }
    if (currentTool == Tool::Fill) {
        fill();
    }
    else if (currentTool == Tool::Rect) {
        startRect();
    }
    else if (currentTool == Tool::Circle) {
        startCircle();
    }
    else if (currentTool == Tool::Triangle) {
        startTriangle();
    }
    else if (currentTool == Tool::Polygon) {
        if (!isDrawingPolygon) {
            startPolygon();
        }
        else {
            updatePolygon();
        }
    }
}

void PaintApp::handleRightClick(sf::Event event) {
    if (currentTool == Tool::Polygon) {
        stopPolygon();
    }
}

void PaintApp::handleClickRelease(sf::Event event) {
    if (event.mouseButton.button == sf::Mouse::Left) {
        if (currentTool == Tool::Pen) {
            stopDrawing();
        }
        if (currentTool == Tool::Eraser) {
            stopErase();
        }
        else if (currentTool == Tool::Rect) {
            stopRect();
        }
        else if (currentTool == Tool::Circle) {
            stopCircle();
        }
        else if (currentTool == Tool::Triangle) {
            stopTriangle();
        }
    }
}

void PaintApp::handleMouseMove(sf::Event event) {
    if (currentTool == Tool::Pen && isDrawing) {
        draw();
    }
    if (currentTool == Tool::Eraser && isErasing) {
        erase();
    }
    else if (currentTool == Tool::Rect && isDrawingRect) {
        updateRect();
    }
    else if (currentTool == Tool::Circle && isDrawingCircle) {
        updateCircle();
    }
    else if (currentTool == Tool::Triangle && isDrawingTriangle) { 
        updateTriangle();
    }
}