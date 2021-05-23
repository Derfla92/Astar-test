#ifndef PATHFINDING_H
#define PATHFINDING_H
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <map>
#include "Renderer.h"

class PathFinding
{
public:
    int map_width{10};
    int map_height{10};

    static constexpr int diagonal_weight{14};
    static constexpr int side_weight{10};

    sf::Vector2f directions[8]{
        {0, -1}, // up
        {1, 0},  // right
        {0, 1},  // down
        {-1, 0}, // left
        {1, -1}, // up_right
        {1, 1},  // down_right
        {-1, 1}, // down_left
        {-1, -1} // up_left
    };

    struct Node
    {
        Node() = default;
        Node(sf::Vector2f position, Node *parent = nullptr) : pos{position}, parent(parent){};
        ~Node()
        {
            std::cout << "Deleting node." << std::endl;
        }
        sf::Vector2f pos;
        Node *parent;

        int g_cost{};
        int h_cost{};
        int get_f_cost()
        {
            return g_cost + h_cost;
        }
    };
    PathFinding(int = 10, int = 10);
    std::vector<sf::Vector2f> obsticle_pos{};
    std::vector<Node *> construct_path(sf::Vector2f &, sf::Vector2f &);
    void insert_obsticle(int, int);
    bool contains_node_with_pos(const std::vector<Node *> &, sf::Vector2f &);
    Node *get_node_with_pos(const std::vector<Node *> &, sf::Vector2f &);
    bool is_obsticle(float x, float y);
    void print_info() const;
    int obsticle_list_checked{};
    float obsticle_time{};

    int open_list_checked{};
    float open_time{};

    int closed_list_checked{};
    float closed_time{};

    int iterations{};
    
};

#endif