#include "../include/UI.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

sf::Font Toolbar::Button::font; 
Toolbar::Toolbar(sf::RenderWindow& _window) : window(_window) {
    // filepath very buggy atm, feel free to comment out this part 
    if (!Button::font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Error loading font 'arial.ttf'." << std::endl;
        exit(EXIT_FAILURE);
    }

    // start screen ui specs ---------------------------------------

     // Initialize Start Screen Title Text
    titleText.setFont(font);
    titleText.setString("Paint Panther");
    titleText.setCharacterSize(72); // Adjust size as needed
    titleText.setFillColor(sf::Color::Black);
    // Center the title
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    titleText.setPosition(window.getSize().x / 2, window.getSize().y / 3); // Positioned at one-third height

    // Initialize Start Button
    startButton.setSize(sf::Vector2f(200, 60)); // Width: 200px, Height: 60px
    startButton.setFillColor(sf::Color(100, 100, 250)); // Blueish color
    startButton.setOutlineColor(sf::Color::Black);
    startButton.setOutlineThickness(2.0f);
    // Center the button
    startButton.setOrigin(startButton.getSize().x / 2, startButton.getSize().y / 2);
    startButton.setPosition(window.getSize().x / 2, window.getSize().y / 2); // Center of the window

    // Initialize Start Button Text
    startButtonText.setFont(font);
    startButtonText.setString("Start");
    startButtonText.setCharacterSize(32); // Adjust size as needed
    startButtonText.setFillColor(sf::Color::White);
    // Center the text within the button
    sf::FloatRect buttonTextBounds = startButtonText.getLocalBounds();
    startButtonText.setOrigin(buttonTextBounds.width / 2, buttonTextBounds.height / 2);
    startButtonText.setPosition(startButton.getPosition());
    
    // --------------------------------------------------------------

    dropdownIsOpen = false;

    toolbarHeight = 40.0f;
    backgroundColor = sf::Color(200, 200, 200);

    toolbarBackground.setSize(sf::Vector2f(window.getSize().x, toolbarHeight));
    toolbarBackground.setPosition(0, 0);
    toolbarBackground.setFillColor(backgroundColor);

    // ui buttons
    buttonSize.x = 40.0f;
    buttonSize.y = toolbarHeight; 

    sf::Vector2f buttonPos; 
    buttonGap = 5.0f;

    buttons.emplace_back("Undo", Tool::Undo); 
    buttons.emplace_back("Redo", Tool::Redo);
    buttons.emplace_back("Save", Tool::SaveFile); 
    buttons.emplace_back("Pen", Tool::Pen);
    buttons.emplace_back("Ersr", Tool::Eraser); 
    buttons.emplace_back("Fill", Tool::Fill); 
    buttons.emplace_back("Color", Tool::Color); 
    buttons.emplace_back("Drop", Tool::Dropdown);
    buttons.emplace_back("AI", Tool::AI);
    initButtons(buttons, 0, 0);

    // dropdown
    dropdownWidth = (3 * buttonSize.x) + (2 * buttonGap);
    dropdownHeight = buttonSize.x;

    dropdownRect.setSize(sf::Vector2f(dropdownWidth, dropdownHeight));
    dropdownRect.setFillColor(backgroundColor);
    dropdownRect.setPosition(3 * buttonSize.x + 3 * buttonGap, toolbarHeight);

    float dropdownPosX = (3 * buttonSize.x) + (3 * buttonGap);
    float dropdownPosY = toolbarHeight;

    dropdownButtons.emplace_back("Rect", Tool::Rect);
    dropdownButtons.emplace_back("Circle", Tool::Circle);
    dropdownButtons.emplace_back("Tri", Tool::Triangle); 
    dropdownButtons.emplace_back("Po", Tool::Polygon); // TODO: change the UI dropdown so it fits

    initButtons(dropdownButtons, dropdownPosX, dropdownPosY);

}// End toolbar

void Toolbar::initButtons(std::vector<Button>& buttonVector, float startingPosX, float startingPosY) {
    float buttonPosX = startingPosX;
    float buttonPosY = startingPosY; 

    for (Button& button : buttonVector) {
        sf::RectangleShape& buttonRect = button.rect; 

        buttonRect.setSize(sf::Vector2f(buttonSize.x, toolbarHeight)); 
        buttonRect.setPosition(buttonPosX, buttonPosY); 

        // center the button's text
        sf::FloatRect textBounds = button.text.getLocalBounds();
        button.text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f); 
        button.text.setPosition(buttonRect.getPosition().x + buttonRect.getSize().x / 2, buttonRect.getPosition().y + buttonRect.getSize().y / 2);
    
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

void Toolbar::openDropdown() {
    if (dropdownIsOpen) {dropdownIsOpen = false;}
    dropdownIsOpen = true;
}

// hue shift down (darker) to mark selected buttons (excluding undo and save)
void Toolbar::highlightButton(Tool currentButton) { 

    // Undo Highlighting
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i].rect.setFillColor(sf::Color(100, 100, 250));
    }
    for (int i = 0; i < dropdownButtons.size(); i++) {
        dropdownButtons[i].rect.setFillColor(sf::Color(100, 100, 250));
    }

    // Finds button by comparing against button[i].tool
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].tool == currentButton && currentButton !=  Tool::SaveFile && currentButton != Tool::Undo) {
            buttons[i].rect.setFillColor(sf::Color(50, 50, 210));
        }
    }
    for (int i = 0; i < dropdownButtons.size(); i++) {
        if (dropdownButtons[i].tool == currentButton) {
            dropdownButtons[i].rect.setFillColor(sf::Color(80, 80, 230));
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
    int colorWheelSize = 512;
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

// vomit code dont even try to understand cuz i dont 
sf::Color Toolbar::getRgbOnClick(sf::Event event) {
    sf::Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);

	// gets local pos of mouse click based off of the color wheel
    sf::Vector2f localPos = window.mapPixelToCoords(mousePos);
    localPos.x -= cwSprite.getPosition().x;
    localPos.y -= cwSprite.getPosition().y;

    // if not in circle, return debug color (here, it's black)
    // TODO: change it from black to some random color the user will never select
    if (localPos.x < 0 || localPos.x >= cwImage.getSize().x ||
        localPos.y < 0 || localPos.y >= cwImage.getSize().y) {
        return sf::Color::Black; 
    }

    sf::Color color = cwImage.getPixel(localPos.x, localPos.y);

    return color; 
}

void Toolbar::renderUI() {
    window.draw(toolbarBackground);

    for (const Button& btn : buttons) {
        window.draw(btn.rect);
        window.draw(btn.text);
    }

    if (dropdownIsOpen) {
        window.draw(dropdownRect);
        for (const Button& btn : dropdownButtons) {
            window.draw(btn.rect);
            window.draw(btn.text);
        }
    }

    if (colorWheelOpen) {
        window.draw(cwSprite);
    }
}