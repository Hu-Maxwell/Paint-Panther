#include "../include/paint.hpp"

void UndoRedoManager::saveState(const sf::RenderTexture& canvas) {
    sf::Texture texture;
    texture = canvas.getTexture();
    undoStack.push(texture);
    // Clear the redo stack when a new state is saved
    while (!redoStack.empty()) {
        redoStack.pop();
    }
}

void UndoRedoManager::undo(sf::RenderTexture& canvas) {
    if (!undoStack.empty()) {
        sf::Texture texture = undoStack.top();
        undoStack.pop();
        redoStack.push(canvas.getTexture());
        canvas.clear();
        sf::Sprite sprite(texture);
        canvas.draw(sprite);
        canvas.display();
    }
}

void UndoRedoManager::redo(sf::RenderTexture& canvas) {
    if (!redoStack.empty()) {
        sf::Texture texture = redoStack.top();
        redoStack.pop();
        undoStack.push(canvas.getTexture());
        canvas.clear();
        sf::Sprite sprite(texture);
        canvas.draw(sprite);
        canvas.display();
    }
}
