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
        std::sort(open.begin(), open.end(), [](Node *a, Node *b) { return a->get_f_cost() > b->get_f_cost(); });
        currentNode = open.back();
        open.pop_back();
        closed.push_back(currentNode);

        if (currentNode->pos == end_node->pos)
        {
            //std::cout << "found shortest path" << std::endl;
            Node *current = currentNode;
            while (current != nullptr)
            {
                path.push_back(current);
                current = current->parent;
            }
            //exit(0);
            print_info();
            return path;
        }
        std::vector<Node *> children{};
        //std::cout << "Parent node: " << "x: " <<currentNode->pos.x << ", y: " << currentNode->pos.y << std::endl;
        for (int i = 0; i < 8; i++)
        {
            //std::cout << "Looking at: " << "x: " << currentNode->pos.x + directions[i].x << ", y: " << currentNode->pos.y + directions[i].y << ". ";
            if (currentNode->pos.x + directions[i].x < map_width && currentNode->pos.y + directions[i].y < map_height)
            {
                if (currentNode->pos.x + directions[i].x >= 0 && currentNode->pos.y + directions[i].y >= 0)
                {
                    if (!is_obsticle(currentNode->pos.x + directions[i].x, currentNode->pos.y + directions[i].y))
                    {
                        //std::cout << "(" <<currentNode->pos.x << " + " << directions[i].x << ") * " << map_width << " + (" << currentNode->pos.y << " + " << directions[i].y << ") ="<< index << std::endl;
                        Node *child = new Node(currentNode->pos + directions[i], currentNode);
                        //std::cout << std::right << std::setw(17) << "Adding node x: " << temp->pos.x << ", y: " << temp->pos.y << " to the children list." << std::endl;
                        children.push_back(child);
                    }
                }
                else
                {
                    //std::cout << " Node is outside of the min bounds." << std::endl;
                }
            }
            else
            {
                //std::cout << "Node is outside of the max bounds." << std::endl;
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
            /* auto lower = std::lower_bound(open.begin(),open.end(),[child](Node* node){return node->get_f_cost() < child->get_f_cost();});
            std::cout << "Lower: " << (*lower)->get_f_cost() << "| Child: " << child->get_f_cost() << std::endl; */
            open.push_back(child);
        }

        //std::cout << "Sizes" << std::endl;
        //std::cout << "  children: " << children.size() << std::endl;
        //std::cout << "  closed: " << closed.size() << std::endl;
        //std::cout << "  open: " << open.size() << std::endl;
    }
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

        if (currentNode->pos == end_node->pos)
        {
            //std::cout << "found shortest path" << std::endl;
            Node *current = currentNode;
            while (current != nullptr)
            {
                path.push_back(current);
                current = current->parent;
            }
            //exit(0);
            print_info();
            return path;
        }
        std::vector<Node *> children{};
        //std::cout << "Parent node: " << "x: " <<currentNode->pos.x << ", y: " << currentNode->pos.y << std::endl;
        for (int i = 0; i < 8; i++)
        {
            //std::cout << "Looking at: " << "x: " << currentNode->pos.x + directions[i].x << ", y: " << currentNode->pos.y + directions[i].y << ". ";
            if (currentNode->pos.x + directions[i].x < map_width && currentNode->pos.y + directions[i].y < map_height)
            {
                if (currentNode->pos.x + directions[i].x >= 0 && currentNode->pos.y + directions[i].y >= 0)
                {
                    if (!is_obsticle(currentNode->pos.x + directions[i].x, currentNode->pos.y + directions[i].y))
                    {
                        //std::cout << "(" <<currentNode->pos.x << " + " << directions[i].x << ") * " << map_width << " + (" << currentNode->pos.y << " + " << directions[i].y << ") ="<< index << std::endl;
                        Node *child = new Node(currentNode->pos + directions[i], currentNode);
                        //std::cout << std::right << std::setw(17) << "Adding node x: " << temp->pos.x << ", y: " << temp->pos.y << " to the children list." << std::endl;
                        children.push_back(child);
                    }
                }
                else
                {
                    //std::cout << " Node is outside of the min bounds." << std::endl;
                }
            }
            else
            {
                //std::cout << "Node is outside of the max bounds." << std::endl;
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
            /* auto lower = std::lower_bound(open.begin(),open.end(),[child](Node* node){return node->get_f_cost() < child->get_f_cost();});
            std::cout << "Lower: " << (*lower)->get_f_cost() << "| Child: " << child->get_f_cost() << std::endl; */
            open.push_back(child);
            std::push_heap(open.begin(),open.end(),[](Node* a, Node* b){ return a->get_f_cost() > b->get_f_cost();});
        }

        //std::cout << "Sizes" << std::endl;
        //std::cout << "  children: " << children.size() << std::endl;
        //std::cout << "  closed: " << closed.size() << std::endl;
        //std::cout << "  open: " << open.size() << std::endl;
    }
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
