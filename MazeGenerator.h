#ifndef MAZEGEN_H
#define MAZEGEN_H

#include <SFML/Graphics.hpp>

class MazeGenerator
{
    public:
    MazeGenerator() = default;

    sf::Image image{};
    sf::Image generate_maze(int const ,int const);    

};

#endif