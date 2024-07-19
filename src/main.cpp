#include "board.h"
#include <SFML/Graphics.hpp>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <iostream>

int main()
{
    uint32_t width = 1440;
    uint32_t height = 1440;
    bool start = true;

    sf::RenderWindow window = sf::RenderWindow { { width, height }, "Cuda Accelerated Game of Life", sf::Style::Titlebar | sf::Style::Close };

    Board board = Board(window, 1440, true);

    sf::Clock clock;
    float lastTime = 0;

    while (window.isOpen()) {
        float currentTime = clock.restart().asSeconds();
        float fps = 1.f / currentTime;

        lastTime = currentTime;

        std::cout << fps << '\n';

        for (auto event = sf::Event {}; window.pollEvent(event);) {

            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
                else if (event.key.code == sf::Keyboard::Space) {
                    start = !start;
                }
                break;
            case sf::Event::Resized:
                // set screen size
                // get the resized size
                sf::Vector2u size = window.getSize();
                // setup my wanted aspect ratio
                float heightRatio = (float)height / width;
                float widthRatio = (float)width / height;
                // adapt the resized window to my wanted aspect ratio
                if (size.y * widthRatio <= size.x) {
                    size.x = size.y * widthRatio;
                } else if (size.x * heightRatio <= size.y) {
                    size.y = size.x * heightRatio;
                }
                // set the new size
                window.setSize(size);
                break;
            }

            board.processInput(event);
        }

        window.clear();

        board.drawBoard();

        if (start) {
            board.updateBoard();
        }

        window.display();
    }
}