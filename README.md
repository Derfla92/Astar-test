# Astar-test
Trying two diffrent ways of handling A* pathfinding. 

To run this you need the sfml library.
Just run the makefile and you should be able to compile

sudo apt-get install libsfml-dev

1. Uses std::sort to sort a vector to find the node with the lowest "f-value".
2. Uses std::heap in an attempt to sort the vector faster. 
