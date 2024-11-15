#include "../include/PaintApp.h"
#include <future>

void PaintApp::handleEvents() {
    sf::Event event;
    
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (aiResponsePending && aiResponseFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            std::string response = aiResponseFuture.get();
            std::cout << response << std::endl;
            aiResponsePending = false;
        }

        else if (event.type == sf::Event::MouseButtonPressed) {
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

void PaintApp::handleLeftClick(sf::Event event) {
    // manages color
    sf::Color tempColor = toolbar.getRgbOnClick(event);
    if (tempColor != sf::Color::Black) {
        currentColor = tempColor;
    }

    // checks if click is in dropdown
    toolbar.checkIfDropdownClicked(event); 

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
            std::cout << "Starting AI request..." << std::endl;
            aiResponseFuture = getResponseAsync();  
            aiResponsePending = true; 
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
    if (currentTool == Tool::Eraser) {
        if (isErasing) {
            erase(); 
        }
        updateEraserOutline();
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