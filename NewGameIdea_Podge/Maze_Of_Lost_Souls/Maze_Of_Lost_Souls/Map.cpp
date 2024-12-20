
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Map.h"
#include <windows.h> // text color changes
// Constructor for Map
Map::Map(int width, int height) : _width(width), _height(height)
{
    map.resize(height, std::vector<char>(width, ' '));
    srand(time(0));
}

// Checks for Item
bool Map::isItem(int x, int y)
{
    return itemMap.find({ x, y }) != itemMap.end();
}

// Checks if Tile is Walkable
bool Map::isWalkable(int x, int y)
{
    return x >= 0 && x < _width && y >= 0 && y < _height && map[y][x] != '#';
}

// Gets Tile 
char Map::getTile(int x, int y)
{
    return map[y][x];
}

// Collects Item and erases from Map
Item Map::getItem(int x, int y)
{
    auto it = itemMap.find({ x, y });
    if (it != itemMap.end())
    {
        Item item = it->second;
        itemMap.erase(it);
        return item;
    }
    throw std::runtime_error("Item not found.");
}

// Generates the Maze
void Map::generateMaze()
{
    for (int y = 0; y < _height; ++y)
    {
        for (int x = 0; x < _width; ++x)
        {
            if (rand() % 10 < 2)
            {
                setTile(x, y, '#');
            }
        }
    }

    placeItemsOnMap();// places items within game
}

// Places Items on the Map
void Map::placeItemsOnMap()
{
    bool keyPlaced = false;
    bool doorPlaced = false;
    bool enemyPlaced = false;

    while (!keyPlaced || !doorPlaced || !enemyPlaced)
    {
        int x = rand() % _width;
        int y = rand() % _height;

        if (map[y][x] == ' ')
        {
            if (!keyPlaced)
            {
                setTile(x, y, 'K');
                itemMap[{x, y}] = Item("Key", "A key to open locked doors.");
                keyPlaced = true;
            }
            else if (!doorPlaced)
            {
                setTile(x, y, 'D');
                itemMap[{x, y}] = Item("Door", "A Door to enter next level.");
                doorPlaced = true;
            }
            else if (!enemyPlaced)
            {
                setTile(x, y, 'p'); // named x for testing
                itemMap[{x, y}] = Item("Enemy", "Enemie to damage.");
                enemyPlaced = true;
            }
        }
    }

    // Add additional items
    for (int i = 0; i < 5; ++i)
    {
        int x = rand() % _width;
        int y = rand() % _height;

        if (map[y][x] == ' ')
        {
            itemMap[{x, y}] = Item("Potion", "Restores health.");
            setTile(x, y, 'P');
        }
    }
}

// Prints Map and Player Sprite
void Map::printMap()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // found on google to make it more clear where player is

    for (int y = 0; y < _height; ++y)
    {
        for (int x = 0; x < _width; ++x)
        {
            if (map[y][x] == '*') // If the tile is the *
            {
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Green color
                std::cout << "[" << map[y][x] << "]";
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // Reset color
            }
            else
            {
                std::cout << "[" << map[y][x] << "]"; // Print other tiles as-is
            }
        }
        std::cout << std::endl;
    }
}

// Sets Tile
void Map::setTile(int x, int y, char symbol)
{
    if (x >= 0 && x < _width && y >= 0 && y < _height)
    {
        map[y][x] = symbol;
    }
}