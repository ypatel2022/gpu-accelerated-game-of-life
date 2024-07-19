#include "board.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>

Board::Board(sf::RenderWindow& window, int32_t gridSize, bool fillRandom)
    : window(window)
    , gridSize(gridSize)
{
    srand((unsigned)time(NULL));

    scaling = window.getSize().x / gridSize;

    sf::VertexArray gridCellVA(sf::Quads);

    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {

            int x = j * scaling;
            int y = i * scaling;

            sf::Vertex tl(sf::Vector2f(x, y));
            sf::Vertex bl(sf::Vector2f(x, y + scaling));
            sf::Vertex br(sf::Vector2f(x + scaling, y + scaling));
            sf::Vertex tr(sf::Vector2f(x + scaling, y));

            if (fillRandom) {
                // fill random
                if (rand() < RAND_MAX / 15) {
                    tl.color = bl.color = br.color = tr.color = light;
                    cells.push_back(true);
                } else {
                    tl.color = bl.color = br.color = tr.color = dark;
                    cells.push_back(false);
                }

            } else {
                // set to off by default
                tl.color = bl.color = br.color = tr.color = dark;
                cells.push_back(false);
            }

            // add to vertex array
            gridCellVA.append(tl);
            gridCellVA.append(bl);
            gridCellVA.append(br);
            gridCellVA.append(tr);
        }
    }

    // set vertex array
    vertexArray = gridCellVA;
}

Board::~Board()
{
}

void Board::drawBoard()
{
    // update vertex array with highlighted squares
    for (int32_t i = 0; i < cells.size(); i++) {

        float s = scaling;
        int x = i % gridSize * s;
        int y = i / gridSize * s;

        sf::Vertex tl(sf::Vector2f(x, y));
        sf::Vertex bl(sf::Vector2f(x, y + s));
        sf::Vertex br(sf::Vector2f(x + s, y + s));
        sf::Vertex tr(sf::Vector2f(x + s, y));

        if (cells[i]) {
            tl.color = bl.color = br.color = tr.color = light;
        } else {
            tl.color = bl.color = br.color = tr.color = dark;
        }

        // update vertex array
        vertexArray[i * 4] = tl;
        vertexArray[i * 4 + 1] = bl;
        vertexArray[i * 4 + 2] = br;
        vertexArray[i * 4 + 3] = tr;
    }

    window.draw(vertexArray);
}

bool outOfRange(int n, int min, int max)
{
    return n < min || n > max;
}

void Board::processInput(sf::Event& event)
{
    // get mouse position

    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    if (outOfRange(pixelPos.x, 0, window.getSize().x) || outOfRange(pixelPos.y, 0, window.getSize().y)) {
        return;
    }

    int index = int(pixelPos.x / scaling) + int(pixelPos.y / scaling) * gridSize;

    // std::cout << int(pixelPos.x / scaling) << "," << int(pixelPos.y / scaling) << std::endl;

    {
        // Create a bool variable for locking the click.
        static bool lock_click;

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left && lock_click != true) {
                lock_click = true;

                cells[index] = !cells[index];
            }
        }

        if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                lock_click = false;
            }
        }
    }
}

void Board::updateBoard()
{
    std::vector<bool> cellCopy = cells;

    for (int row = 0; row < gridSize; ++row) {
        for (int col = 0; col < gridSize; ++col) {
            int n = 0;
            int index = row * gridSize + col;

            bool top = row > 0;
            bool bottom = row < (gridSize - 1);
            bool left = col > 0;
            bool right = col < (gridSize - 1);

            // Check neighbors
            if (top && left && cells[(row - 1) * gridSize + (col - 1)] == 1)
                n++;
            if (top && cells[(row - 1) * gridSize + col] == 1)
                n++;
            if (top && right && cells[(row - 1) * gridSize + (col + 1)] == 1)
                n++;
            if (left && cells[row * gridSize + (col - 1)] == 1)
                n++;
            if (right && cells[row * gridSize + (col + 1)] == 1)
                n++;
            if (bottom && left && cells[(row + 1) * gridSize + (col - 1)] == 1)
                n++;
            if (bottom && cells[(row + 1) * gridSize + col] == 1)
                n++;
            if (bottom && right && cells[(row + 1) * gridSize + (col + 1)] == 1)
                n++;

            // n = (top && left && cells[(row - 1) * gridSize + (col - 1)] == 1) + (top && cells[(row - 1) * gridSize + col] == 1) + (top && right && cells[(row - 1) * gridSize + (col + 1)] == 1) + (left && cells[row * gridSize + (col - 1)] == 1) + (right && cells[row * gridSize + (col + 1)] == 1) + (bottom && left && cells[(row + 1) * gridSize + (col - 1)] == 1) + (bottom && cells[(row + 1) * gridSize + col] == 1) + (bottom && right && cells[(row + 1) * gridSize + (col + 1)] == 1);

            // Apply Game of Life rules
            if (cells[index] == 1 && (n == 2 || n == 3)) {
                cellCopy[index] = 1; // Cell stays alive
            } else if (cells[index] == 0 && n == 3) {
                cellCopy[index] = 1; // Cell becomes alive
            } else {
                cellCopy[index] = 0; // Cell dies or stays dead
            }
        }
    }

    cells = cellCopy;
}
