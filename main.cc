
#include "PathFinding.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "Renderer.h"
#include "MazeGenerator.h"

#define DEBUG false

void print_titlebar(sf::Clock &, double const &);

#if DEBUG

void push(std::vector<int> &v, int number)
{
    v.push_back(number);

    std::push_heap(v.begin(), v.end(), [](int a, int b) { return a > b; });
}
void print(std::vector<int> v)
{
    for (auto i : v)
        std::cout << i << ", ";
    std::cout << std::endl;
}

int main()
{
    std::vector<int> v{};

    auto pop = [](int a, int b) { return a > b; };

    push(v, 6);
    print(v);
    push(v, 2);
    print(v);
    push(v, 9);
    print(v);
    push(v, 3);
    print(v);
    push(v, 10);
    print(v);
    push(v, 1);
    print(v);

    std::pop_heap(v.begin(), v.end());
    std::cout << v.back();

    return 0;
}
#endif

/* int main()
{
    MazeGenerator maze_gen{};
    maze_gen.generate_maze(20,20);
    return 0;
} */

#if !DEBUG
int main()
{

    PathFinding pathFinder{10, 10};
    sf::Vector2f start{0, 0};
    sf::Vector2f goal{static_cast<float>(pathFinder.map_width - 1), static_cast<float>(pathFinder.map_height - 3)};
    sf::Vector2f temp_goal{};

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

    for (int i = 0; i < pathFinder.map_height - 1; i++)
    {
        pathFinder.obsticle_pos.push_back(sf::Vector2f(3, i));
    }
    std::vector<PathFinding::Node *> path{};
    while (Renderer::window.isOpen())
    {
        sf::Event event;
        while (Renderer::window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                //std::cout << "Quitting AstarTest." << std::endl;
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

        //Draw all tiles
        Renderer::window.clear();
        if (temp_goal != goal)
        {
            temp_goal = goal;
            sf::Clock time{};
            float elapsed_time{};

            //Use std::sort()
            if (false)
            {
                time.restart();
                path = pathFinder.construct_path_with_stl_sort(start, temp_goal);
                elapsed_time = static_cast<float>(time.getElapsedTime().asMilliseconds());
                time.restart();
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
                std::cout << "Time the algorithm took: "
                          << elapsed_time / 1000.0f << " s" << std::endl;
                std::cout << "----------------------------------" << std::endl;
            }
        }

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
#endif