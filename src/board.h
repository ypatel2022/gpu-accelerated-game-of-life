#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Board {

public:
    Board(sf::RenderWindow& window, int32_t gridSize, bool fillRandom);
    ~Board();

    void drawBoard();
    void processInput(sf::Event& event);
    void updateBoard();

private:
    sf::RenderWindow& window;
    sf::VertexArray vertexArray;

    sf::Color dark = sf::Color(26, 30, 35);
    sf::Color light = sf::Color(140, 143, 188);
    sf::Color highlighted = sf::Color(252, 186, 3, 200);

    int32_t gridSize;

    std::vector<bool> cells;
    float scaling;
};