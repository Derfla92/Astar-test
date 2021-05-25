
#include "PathFinding.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "Renderer.h"
#include "MazeGenerator.h"

static PathFinding pathFinder{35, 39};
static sf::Vector2f start{16,7};
//static sf::Vector2f goal{static_cast<float>(pathFinder.map_width - 1), static_cast<float>(pathFinder.map_height - 3)};
static sf::Vector2f goal{16,24};
static sf::Vector2f temp_goal{};

void print_titlebar(sf::Clock &, double const &);
void end_point_movement();

int main()
{

    sf::RectangleShape tile{};
    tile.setSize(sf::Vector2f(Renderer::window.getSize().x / pathFinder.map_width, Renderer::window.getSize().y / pathFinder.map_height));
    tile.setOrigin(tile.getSize().x / 2, tile.getSize().y / 2);
    tile.setFillColor(sf::Color::Red);
    tile.setOutlineColor(sf::Color::White);
    tile.setOutlineThickness(1);

    sf::Font font{};
    font.loadFromFile("MATURASC.TTF");
    sf::Text text{};
    text.setCharacterSize(tile.getSize().y / 4);

    text.setFillColor(sf::Color::Black);
    text.setFont(font);

    sf::Clock clock{};
    double const fps{60.0};

    //Player wall
    pathFinder.insert_obsticle(11,4);
    pathFinder.insert_obsticle(12,4);
    pathFinder.insert_obsticle(12,5);
    pathFinder.insert_obsticle(13,5);
    pathFinder.insert_obsticle(13,6);
    pathFinder.insert_obsticle(14,6);
    pathFinder.insert_obsticle(14,7);
    pathFinder.insert_obsticle(15,7);
    pathFinder.insert_obsticle(15,8);
    pathFinder.insert_obsticle(16,8);
    pathFinder.insert_obsticle(17,8);
    pathFinder.insert_obsticle(17,7);
    pathFinder.insert_obsticle(18,7);
    pathFinder.insert_obsticle(18,6);
    pathFinder.insert_obsticle(19,6);
    pathFinder.insert_obsticle(19,5);
    pathFinder.insert_obsticle(20,5);
    pathFinder.insert_obsticle(20,4);
    pathFinder.insert_obsticle(21,4);

    for(int i=9;i <24; i++)
    {
        pathFinder.insert_obsticle(i,16);
    }

    //Zombie wall
    pathFinder.insert_obsticle(21,27);
    pathFinder.insert_obsticle(20,27);
    pathFinder.insert_obsticle(20,26);
    pathFinder.insert_obsticle(19,26);
    pathFinder.insert_obsticle(19,25);
    pathFinder.insert_obsticle(18,25);
    pathFinder.insert_obsticle(18,24);
    pathFinder.insert_obsticle(17,24);
    pathFinder.insert_obsticle(17,23);
    pathFinder.insert_obsticle(16,23);
    pathFinder.insert_obsticle(15,23);
    pathFinder.insert_obsticle(15,24);
    pathFinder.insert_obsticle(14,24);
    pathFinder.insert_obsticle(14,25);
    pathFinder.insert_obsticle(13,25);
    pathFinder.insert_obsticle(13,26);
    pathFinder.insert_obsticle(12,26);
    pathFinder.insert_obsticle(12,27);
    pathFinder.insert_obsticle(11,27);
    







    /* for (int i = 0; i < pathFinder.map_height - 1; i++)
    {
        pathFinder.obsticle_pos.push_back(sf::Vector2f(3, i));
    } */
    std::vector<PathFinding::Node *> path{};
    while (Renderer::window.isOpen())
    {
        //Enabling movement for the goal_node
        end_point_movement();

        //Find shortest path
        if (temp_goal != goal)
        {
            temp_goal = goal;
            sf::Clock time{};
            float elapsed_time{};

            //Use std::sort()
            if (true)
            {
                time.restart();
                path = pathFinder.construct_path_with_stl_sort(start, temp_goal);
                elapsed_time = static_cast<float>(time.getElapsedTime().asMilliseconds());
                time.restart();
                std::cout << "Number of nodes in the path: " << path.size() << std::endl;
                std::cout << "Time the algorithm took: "
                          << elapsed_time / 1000.0f << " s" << std::endl;
                std::cout << "----------------------------------" << std::endl;
            }
            //Use std::make_heap()
            if (true)
            {
                time.restart();
                path = pathFinder.construct_path_with_heap(start, temp_goal);
                elapsed_time = static_cast<float>(time.getElapsedTime().asMilliseconds());
                time.restart();
                std::cout << "Number of nodes in the path: " << path.size() << std::endl;
                std::cout << "Time the algorithm took: "
                          << elapsed_time / 1000.0f << " s" << std::endl;
                std::cout << "----------------------------------" << std::endl;
            }
        }

        //Draw all tiles
        Renderer::window.clear();
        for (int x = 0; x < pathFinder.map_width; x++)
        {
            for (int y = 0; y < pathFinder.map_height; y++)
            {
                tile.setPosition(x * tile.getSize().x + tile.getSize().x / 2, y * tile.getSize().y + tile.getSize().y / 2);
                tile.setFillColor(sf::Color::Red);
                Renderer::window.draw(tile);
                if (x == start.x && y == start.y)
                {
                    tile.setFillColor(sf::Color::Green);
                }
                else if (x == temp_goal.x && y == temp_goal.y)
                {
                    tile.setFillColor(sf::Color::Cyan);
                }
                else
                {
                    for (auto obsticle : pathFinder.obsticle_pos)
                    {
                        if (x == obsticle.x && y == obsticle.y)
                        {
                            tile.setFillColor(sf::Color::Black);
                        }
                    }
                }

                Renderer::window.draw(tile);
                if (path.size() > 0)
                {
                    auto it = std::find_if(path.begin(), path.end(), [x, y](PathFinding::Node *node) { return x == node->pos.x && y == node->pos.y; });
                    if (it != path.end())
                    {

                        PathFinding::Node *temp_node = *it;
                        sf::Vector2f tile_center = sf::Vector2f(tile.getPosition().x, tile.getPosition().y);

                        tile.setFillColor(sf::Color::Blue);
                        Renderer::window.draw(tile);
                        //Draw g_cost of each node.
                        text.setPosition(tile_center.x - tile.getSize().x / 2, tile_center.y - tile.getSize().y / 2);

                        text.setString("g: " + std::to_string(temp_node->g_cost));
                        text.setOrigin(0, 0);
                        Renderer::window.draw(text);

                        //Draw h_cost of each node.
                        text.setPosition(tile_center.x + tile.getSize().x / 2, tile_center.y - tile.getSize().y / 2);
                        text.setString("h: " + std::to_string(temp_node->h_cost));
                        text.setOrigin(text.getGlobalBounds().width, 0);
                        Renderer::window.draw(text);

                        //Draw f_cost of each node.
                        text.setPosition(tile_center.x, tile_center.y);
                        text.setString(std::to_string(temp_node->get_f_cost()));
                        text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
                        Renderer::window.draw(text);
                    }
                }
            }
        }
        Renderer::window.display();

        print_titlebar(clock, fps);
    }

    return 0;
}

void print_titlebar(sf::Clock &clck, double const &fps)
{
    std::stringstream ss;
    ss << "AstarTest | Current framerate: " << fps << " (LOCKED) | Possible framerate: " << (1 / clck.getElapsedTime().asSeconds());
    Renderer::window.setTitle(ss.str());
    sleep(sf::milliseconds(1000.0 / fps) - clck.getElapsedTime());
    clck.restart();
}

void end_point_movement()
{
    sf::Event event;
    while (Renderer::window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            std::cout << "Quitting AstarTest." << std::endl;
            exit(0);
        }
        if (event.type == sf::Event::KeyReleased)
        {

            if (event.key.code == sf::Keyboard::A)
            {
                if (goal.x > 0)
                {
                    goal = goal + sf::Vector2f(-1, 0);
                }
            }
            else if (event.key.code == sf::Keyboard::D)
            {
                if (goal.x < pathFinder.map_width - 1)
                {
                    goal = goal + sf::Vector2f(1, 0);
                }
            }
            else if (event.key.code == sf::Keyboard::W)
            {
                if (goal.y > 0)
                {
                    goal = goal + sf::Vector2f(0, -1);
                }
            }
            else if (event.key.code == sf::Keyboard::S)
            {
                if (goal.y < pathFinder.map_height - 1)
                {
                    goal = goal + sf::Vector2f(0, 1);
                }
            }
        }
    }
}