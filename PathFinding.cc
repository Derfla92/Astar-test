#include "PathFinding.h"
#include <iomanip>

PathFinding::PathFinding(int width, int height) : map_width{width}, map_height{height} {}

std::vector<PathFinding::Node *> PathFinding::construct_path_with_stl_sort(sf::Vector2f &start, sf::Vector2f &end)
{
    //list of closed nodes already evaluated
    std::vector<Node *> closed{};
    closed_list_checked = 0;
    //list of not closed nodes to be evaluated
    std::vector<Node *> open{};
    open_list_checked = 0;
    Node *currentNode{nullptr};
    Node *start_node{new Node(start)};
    Node *end_node{new Node(end)};
    std::vector<Node *> path{};
    open.push_back(start_node);
    
    iterations = 0;
    obsticle_list_checked = 0;
    closed_time = 0;
    open_time = 0;
    obsticle_time = 0;
    while (open.size() > 0)
    {
        //Sort the list of nodes to check, sorting with the node with the lowest F-cost to be in the back.
        std::sort(open.begin(), open.end(), [](Node *a, Node *b) { return a->get_f_cost() > b->get_f_cost(); });
        currentNode = open.back();
        open.pop_back();
        closed.push_back(currentNode);

        //Check if shortest path has been found.
        if (currentNode->pos == end_node->pos)
        {
            Node *current = currentNode;
            while (current != nullptr)
            {
                path.push_back(current);
                current = current->parent;
            }
            print_info();
            return path;
        }
        //Finding Children for the current node.
        std::vector<Node *> children{};
        for (int i = 0; i < 8; i++)
        {
            if (currentNode->pos.x + directions[i].x < map_width && currentNode->pos.y + directions[i].y < map_height)
            {
                if (currentNode->pos.x + directions[i].x >= 0 && currentNode->pos.y + directions[i].y >= 0)
                {
                    if (!is_obsticle(currentNode->pos.x + directions[i].x, currentNode->pos.y + directions[i].y))
                    {
                        Node *child = new Node(currentNode->pos + directions[i], currentNode);
                        children.push_back(child);
                    }
                }
            }
        }

        iterations++;
        for (auto &child : children)
        {
            if (contains_node_with_pos(closed, child->pos))
            {
                continue;
            }
            child->g_cost = currentNode->g_cost + 10;
            child->h_cost = pow(child->pos.x - end.x, 2) + pow(child->pos.y - end.y, 2);
            Node *node = get_node_with_pos(open, child->pos);
            if (node)
            {
                if (child->g_cost > node->g_cost)
                {
                    continue;
                }
            }
            open.push_back(child);
        }
    }
    std::cout << "Used std::sort method." << std::endl;
    print_info();
    return path;
}

std::vector<PathFinding::Node *> PathFinding::construct_path_with_heap(sf::Vector2f &start, sf::Vector2f &end)
{
    //list of closed nodes already evaluated
    std::vector<Node *> closed{};
    closed_list_checked = 0;
    //list of not closed nodes to be evaluated
    std::vector<Node *> open{};
    open_list_checked = 0;
    Node *currentNode{nullptr};
    Node *start_node{new Node(start)};
    Node *end_node{new Node(end)};
    std::vector<Node *> path{};
    open.push_back(start_node);
    
    iterations = 0;
    obsticle_list_checked = 0;
    closed_time = 0;
    open_time = 0;
    obsticle_time = 0;
    while (open.size() > 0)
    {
        std::pop_heap(open.begin(),open.end(),[](Node* a, Node* b){ return a->get_f_cost() > b->get_f_cost();});
        currentNode = open.back();
        open.pop_back();
        closed.push_back(currentNode);

        //Check if shortest path has been found.
        if (currentNode->pos == end_node->pos)
        {
            Node *current = currentNode;
            while (current != nullptr)
            {
                path.push_back(current);
                current = current->parent;
            }
            std::cout << "Used std::heap method." << std::endl;
            print_info();
            return path;
        }

        //Finding Children for the current node.
        std::vector<Node *> children{};
        for (int i = 0; i < 8; i++)
        {
            if (currentNode->pos.x + directions[i].x < map_width && currentNode->pos.y + directions[i].y < map_height)
            {
                if (currentNode->pos.x + directions[i].x >= 0 && currentNode->pos.y + directions[i].y >= 0)
                {
                    if (!is_obsticle(currentNode->pos.x + directions[i].x, currentNode->pos.y + directions[i].y))
                    {
                        Node *child = new Node(currentNode->pos + directions[i], currentNode);
                        children.push_back(child);
                    }
                }
            }
        }
        iterations++;
        for (auto &child : children)
        {
            if (contains_node_with_pos(closed, child->pos))
            {
                continue;
            }
            child->g_cost = currentNode->g_cost + 10;
            child->h_cost = pow(child->pos.x - end.x, 2) + pow(child->pos.y - end.y, 2);
            Node *node = get_node_with_pos(open, child->pos);
            if (node)
            {
                if (child->g_cost > node->g_cost)
                {
                    continue;
                }
            }
            open.push_back(child);
            std::push_heap(open.begin(),open.end(),[](Node* a, Node* b){ return a->get_f_cost() > b->get_f_cost();});
        }
    }
    std::cout << "Used std::heap method." << std::endl;
    print_info();
    return path;
}

void PathFinding::insert_obsticle(int x, int y)
{
    obsticle_pos.push_back(sf::Vector2f(x, y));
}

bool PathFinding::contains_node_with_pos(const std::vector<Node *> &closed_list, sf::Vector2f &pos)
{
   
    sf::Clock time{};
    for (auto node : closed_list)
    {
        if (pos == node->pos)
        {
            closed_time += static_cast<float>(time.getElapsedTime().asMicroseconds());
            return true;
        }
    }
    closed_list_checked++;
    closed_time += static_cast<float>(time.getElapsedTime().asMicroseconds());
    return false;
}

PathFinding::Node *PathFinding::get_node_with_pos(const std::vector<Node *> &nodes, sf::Vector2f &pos)
{
    sf::Clock time{};
    for (auto node : nodes)
    {
        if (pos == node->pos)
        {
            open_time += static_cast<float>(time.getElapsedTime().asMicroseconds());
            return node;
        }
    }
    open_list_checked++;
    open_time += static_cast<float>(time.getElapsedTime().asMicroseconds());
    return nullptr;
}
bool PathFinding::is_obsticle(float x, float y)
{
    sf::Clock time{};
    sf::Vector2f pos{x,y};
    for (auto obsticle : obsticle_pos)
    {
        if (obsticle == pos)
        {
            obsticle_time += static_cast<float>(time.getElapsedTime().asMicroseconds());
            return true;
        }
    }
    obsticle_list_checked++;
    obsticle_time += static_cast<float>(time.getElapsedTime().asMicroseconds());
    return false;
}

void PathFinding::print_info()const
{
    std::cout << "Obsticle list checked: " << obsticle_list_checked << std::endl;
    std::cout << "Total time to check Obsticle list: " << obsticle_time/1000000.0f << " seconds" <<  std::endl;
    std::cout << "Average time to check Obsticle list: " << (obsticle_time/obsticle_list_checked) << " microseconds" << std::endl; 
    std::cout << std::endl;
    std::cout << "Open list checked: " << open_list_checked << std::endl;
    std::cout << "Total time to check Open list: " << open_time/1000000.0f << " seconds" <<  std::endl;
    std::cout << "Average time to check Open list: " << (open_time/open_list_checked) << " microseconds" << std::endl; 
    std::cout << std::endl;
    std::cout << "Closed list checked: " << closed_list_checked << std::endl;
    std::cout << "Total time to check Closed list: " << closed_time/1000000.0f << " seconds" << std::endl;
    std::cout << "Average time to check Closed list: " << (closed_time/closed_list_checked) << " microseconds" << std::endl; 
    std::cout << std::endl;
    std::cout << "Iterations: " << iterations << std::endl;
}
