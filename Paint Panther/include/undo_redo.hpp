#ifndef UNDO_REDO_HPP
#define UNDO_REDO_HPP

class UndoRedoManager {
public:
    void saveState(const sf::RenderTexture& canvas);
    void undo(sf::RenderTexture& canvas);
    void redo(sf::RenderTexture& canvas);

private:
    std::stack<sf::Texture> undoStack;
    std::stack<sf::Texture> redoStack;
};

#endif