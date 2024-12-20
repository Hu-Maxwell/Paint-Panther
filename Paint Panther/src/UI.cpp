#include "../include/UI.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


Toolbar::Toolbar(sf::RenderWindow& _window) : window(_window) {
    dropdownIsOpen = false;

    toolbarHeight = 40.0f;
    backgroundColor = sf::Color(200, 200, 200);

    toolbarBackground.setSize(sf::Vector2f(window.getSize().x * 3, toolbarHeight));
    toolbarBackground.setPosition(0, 0);
    toolbarBackground.setFillColor(backgroundColor);

    // ui buttons
    buttonSize.x = 40.0f;
    buttonSize.y = toolbarHeight; 

    sf::Vector2f buttonPos; 
    buttonGap = 5.0f;

    buttons.emplace_back("assets/undo.png", Tool::Undo); 
    buttons.emplace_back("assets/redo.png", Tool::Redo);
    buttons.emplace_back("assets/save.png", Tool::SaveFile); 
    buttons.emplace_back("assets/pen.png", Tool::Pen);
    buttons.emplace_back("assets/eraser.png", Tool::Eraser); 
    buttons.emplace_back("assets/fill.png", Tool::Fill); 
    buttons.emplace_back("assets/color.png", Tool::Color); 
    buttons.emplace_back("assets/dropdown.png", Tool::Dropdown);
    buttons.emplace_back("assets/ai.png", Tool::AI);
    initButtons(buttons, 0, 0);


    // dropdown ======================================================
    dropdownWidth = (4 * buttonSize.x) + (3 * buttonGap);
    dropdownHeight = buttonSize.x;

    float dropdownPosX = (7 * buttonSize.x) + (7 * buttonGap);
    float dropdownPosY = toolbarHeight;

    dropdownRect.setSize(sf::Vector2f(dropdownWidth, dropdownHeight));
    dropdownRect.setFillColor(backgroundColor);
    dropdownRect.setPosition(dropdownPosX, dropdownPosY);

    dropdownButtons.emplace_back("assets/rectangle.png", Tool::Rect);
    dropdownButtons.emplace_back("assets/circle.png", Tool::Circle);
    dropdownButtons.emplace_back("assets/triangle.png", Tool::Triangle); 
    dropdownButtons.emplace_back("assets/polygon.png", Tool::Polygon); 

    initButtons(dropdownButtons, dropdownPosX, dropdownPosY);

}

void Toolbar::initButtons(std::vector<Button>& buttonVector, float startingPosX, float startingPosY) {
    float buttonPosX = startingPosX;
    float buttonPosY = startingPosY; 

    for (Button& button : buttonVector) {
        sf::RectangleShape& buttonRect = button.rect; 

        buttonRect.setSize(sf::Vector2f(buttonSize.x, toolbarHeight)); 
        button.setPosition(buttonPosX, buttonPosY); 
        buttonPosX += buttonSize.x + buttonGap; 
    }
}

Tool Toolbar::handleUIInput(sf::Event event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        for (int i = 0; i < buttons.size(); i++) {
            if (buttons[i].rect.getGlobalBounds().contains(mousePos)) {
                return buttons[i].tool; 
            }
        }
        
        for (int i = 0; i < dropdownButtons.size(); i++) {
            if (dropdownButtons[i].rect.getGlobalBounds().contains(mousePos)) {
                return dropdownButtons[i].tool;
            }
        }
    }
    
    // if user doesn't click on any buttons 
    return Tool::Nothing; 
}

void Toolbar::openDropdown() { // referenced by user.cpp
    dropdownIsOpen = true;
}

void Toolbar::checkIfDropdownClicked(sf::Event event) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (dropdownIsOpen) {
        if (!dropdownRect.getGlobalBounds().contains(mousePos)) {
            dropdownIsOpen = false;
        }
    }
}

// hue shift down (darker) to mark selected buttons (excluding undo and save)
void Toolbar::highlightButton(Tool currentButton) { 

    // Undo Highlighting
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i].background.setFillColor(sf::Color(255, 174, 174));
    }
    for (int i = 0; i < dropdownButtons.size(); i++) {
        dropdownButtons[i].background.setFillColor(sf::Color(255, 174, 174));
    }

    // Finds button by comparing against button[i].tool
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].tool == currentButton && currentButton !=  Tool::SaveFile && currentButton != Tool::Undo) {
            buttons[i].background.setFillColor(sf::Color(255, 120, 120));
        }
    }
    for (int i = 0; i < dropdownButtons.size(); i++) {
        if (dropdownButtons[i].tool == currentButton) {
            dropdownButtons[i].background.setFillColor(sf::Color(255, 120, 120));
        }
    }

}

// h is hue, s is saturation, v is value
sf::Color Toolbar::HSVtoRGB(float h, float s, float v) {
    float r, g, b;
    r = g = b = v;

    float hf = h / 60.0f; // fraction of a hue sector 
    int i = static_cast<int>(floor(hf)); // integer of each sector - refer back to the chart 
    float f = hf - i; // fraction of a hue sector - like for example the ratio between red and yellow of each sector
    float pv = v * (1 - s); // partial val
    float qv = v * (1 - s * f); // quarter val
    float tv = v * (1 - s * (1 - f)); // tertiary val

    switch (i) {
    case 0:
        r = v;
        g = tv;
        b = pv;
        break;
    case 1:
        r = qv;
        g = v;
        b = pv;
        break;
    case 2:
        r = pv;
        g = v;
        b = tv;
        break;
    case 3:
        r = pv;
        g = qv;
        b = v;
        break;
    case 4:
        r = tv;
        g = pv;
        b = v;
        break;
    case 5:
    default:
        r = v;
        g = pv;
        b = qv;
        break;
    }

    return sf::Color((r * 255.0f), (g * 255.0f), (b * 255.0f));
}


void Toolbar::openColorWheel() {
    int colorWheelSize = 216;
    cwImage.create(colorWheelSize, colorWheelSize, sf::Color::Black);

    int cx = colorWheelSize / 2; // x of circle
    int cy = colorWheelSize / 2; // y of circle
    int radius = colorWheelSize / 2; // radius of circle

    // for each pixel
    for (int y = 0; y < colorWheelSize; y++) {
        for (int x = 0; x < colorWheelSize; x++) {
            // dx and dy is the distance from the center of the circle
            int dx = x - cx;
            int dy = y - cy;
            double distance = sqrt(dx * dx + dy * dy); // dist formula -> distance of pixel to center

            // if the pixel is inside the circle
            if (distance <= radius) {
                double angle = atan2(dy, dx);

                // ensure angle is always between [0, 2pi] 
                if (angle < 0)
                    angle += 2 * M_PI;

                // each pixel of a color wheel is made of 3 aspects: hue, saturation and value
                double hue = angle * 180 / M_PI;
                double saturation = distance / radius;
                double value = 1.0;

                sf::Color color = HSVtoRGB(hue, saturation, value);
                cwImage.setPixel(x, y, color);
            }
        }
    }

    if (!cwTexture.loadFromImage(cwImage)) {
        std::cerr << "Failed to load color wheel texture." << std::endl;
        return;
    }

    cwSprite.setTexture(cwTexture);
    sf::Vector2u windowSize = window.getSize();

    float cwX = (6 * buttonSize.x) + (6 * buttonGap);
    float cwY = toolbarHeight;
    cwSprite.setPosition(cwX, cwY);

    colorWheelOpen = true;
}

sf::Color Toolbar::getRgbOnClick(sf::Event event) {
    sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);

	// gets local pos of mouse click based off of the color wheel
    sf::Vector2f localPos = window.mapPixelToCoords(mousePos);
    localPos.x -= cwSprite.getPosition().x;
    localPos.y -= cwSprite.getPosition().y;

    // if not in circle, return placeholder color (here, it's black)
    // TODO: change it from black to some random color the user will never select
    if (localPos.x < 0 || localPos.x >= cwImage.getSize().x ||
        localPos.y < 0 || localPos.y >= cwImage.getSize().y) {
        colorWheelOpen = false;
        return sf::Color::Black;
    }
    else {
        return cwImage.getPixel(localPos.x, localPos.y);
    }
}

void Toolbar::renderUI() {
    window.draw(toolbarBackground);

    for (const Button& btn : buttons) {
        window.draw(btn.background);
        window.draw(btn.rect);
    }

    if (dropdownIsOpen) {
        window.draw(dropdownRect);
        for (const Button& btn : dropdownButtons) {
            window.draw(btn.background);
            window.draw(btn.rect);
        }
    }

    if (colorWheelOpen) {
        window.draw(cwSprite);
    }
}