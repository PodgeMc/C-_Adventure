#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream> // For save/load functionality
#include <conio.h> // For _getch()
#include "Map.h"
#include "Player.h"
#include "Point2D.h"
#include "Item.h"
#include "Enemy.h"

// This shows the main menu where I can start, load, or quit the game
void displayMainMenu()
{
    std::cout << "============================================" << std::endl;
    std::cout << "       WELCOME TO THE MAZE OF LOST SOULS    " << std::endl;
    std::cout << "   ==========================================" << std::endl;
    std::cout << "   |  [1] Start Game         | " << std::endl;
    std::cout << "   |  [2] Load Game          | " << std::endl;
    std::cout << "   |  [3] Quit               | " << std::endl;
    std::cout << "   ==========================================" << std::endl;
}

// Saves my game by writing the player's data to a file
void saveGame(const Player& player)
{
    std::ofstream saveFile("savegame.txt");

    // Save the player's current state
    saveFile << player.getName() << " " << player.getHealth() << " "
        << player.getXP() << " " << player.getPosition().getX() << " "
        << player.getPosition().getY() << "\n";

    // Save all the items in the inventory
    for (const auto& item : player.getInventory())
    {
        saveFile << item.getName() << " " << item.getDescription() << "\n";
    }

    saveFile.close();
    std::cout << "Game saved!" << std::endl;
}

// Loads my saved game from a file
void loadGame(Player& player, Map& gameMap)
{
    std::ifstream loadFile("savegame.txt");

    // Check if the save file exists
    if (!loadFile.is_open())
    {
        std::cout << "No save file found. Starting a new game." << std::endl;
        return;
    }

    std::string playerName;
    int health, xp, x, y;

    // Read player data
    if (!(loadFile >> playerName >> health >> xp >> x >> y))
    {
        std::cerr << "Error reading player data. Starting a new game." << std::endl;
        return;
    }

    player = Player(playerName, health, 10);
    player.setPosition(x, y);
    player.gainXP(xp);

    // Load the player's inventory
    std::string itemName, itemDescription;
    while (loadFile >> itemName)
    {
        std::getline(loadFile, itemDescription);
        if (!itemDescription.empty() && itemDescription[0] == ' ')
        {
            itemDescription.erase(0, 1);
        }
        player.collectItem(Item(itemName, itemDescription));
    }

    // Update the map with the player's position
    gameMap.setTile(x, y, '*');
    std::cout << "Game successfully loaded!" << std::endl;
}

int main()
{
    Enemy enemy; // Enemy to interact with during the game
    std::string playerName;
    char menuChoice;

    displayMainMenu(); // Show the game menu

    while (true)
    {
        menuChoice = _getch(); // Get input from the menu

        if (menuChoice == '1') // Start a new game
        {
            std::cout << "Enter your player name: ";
            std::getline(std::cin, playerName);

            if (playerName.empty())
            {
                playerName = "Hero"; // Default name
            }

            Player player(playerName, 100, 10); // Create a player
            Map gameMap(10, 10);               // Create a 10x10 map
            gameMap.generateMaze();           // Generate a random maze

            bool inventoryVisible = false; // Keep track of inventory visibility

            char input;
            while (true)
            {
                system("cls");          // Clear the console
                gameMap.printMap();     // Show the map

                // Display the player's current stats
                std::cout << "Player: " << player.getName() << " | Health: " << player.getHealth()
                    << " | XP: " << player.getXP() << " | Level: " << player.getLevel() << std::endl;

                // Show inventory if toggled
                if (inventoryVisible)
                {
                    player.showInventory();
                }

                input = _getch(); // Get input for movement or actions

                if (input == 'q') // Quit the game
                {
                    saveGame(player); // Save the game before exiting
                    break;
                }
                else if (input == ' ') // Pick up an item
                {
                    int x = player.getPosition().getX();
                    int y = player.getPosition().getY();
                    if (gameMap.isItem(x, y))
                    {
                        Item item = gameMap.getItem(x, y);
                        player.collectItem(item);

                        if (item.getName() == "Potion")
                        {
                            player.heal(20); // Heal player
                        }
                        else if (item.getName() == "Enemy")
                        {
                            enemy.Damage_Player(20); // Enemy interaction
                        }

                        gameMap.setTile(x, y, ' '); // Remove the item
                    }
                }
                else if (input == 'i') // Toggle inventory visibility
                {
                    inventoryVisible = !inventoryVisible;
                }
                else // Move the player
                {
                    player.move(input, gameMap);
                }
            }
            break; // Exit the game loop
        }
        else if (menuChoice == '2') // Load a saved game
        {
            Player player("Hero", 100, 10);
            Map gameMap(10, 10);
            gameMap.generateMaze();
            loadGame(player, gameMap);

            char input;
            while (true)
            {
                system("cls");
                gameMap.printMap();
                std::cout << "Player: " << player.getName() << " | Health: " << player.getHealth()
                    << " | XP: " << player.getXP() << " | Level: " << player.getLevel() << std::endl;
                input = _getch();

                if (input == 'q') // Quit the game
                {
                    break;
                }
                else if (input == ' ') // Pick up an item
                {
                    int x = player.getPosition().getX();
                    int y = player.getPosition().getY();
                    if (gameMap.isItem(x, y))
                    {
                        Item item = gameMap.getItem(x, y);
                        player.collectItem(item);

                        if (item.getName() == "Potion")
                        {
                            player.heal(20);
                        }
                        gameMap.setTile(x, y, ' '); // Remove the item
                    }
                }
                else // Move the player
                {
                    player.move(input, gameMap);
                }
            }
            break;
        }
        else if (menuChoice == '3') // Quit
        {
            std::cout << "Exiting game. Goodbye!" << std::endl;
            break;
        }
        else // Invalid menu choice
        {
            std::cout << "Invalid choice, please press '1', '2', or '3'." << std::endl;
        }
    }

    return 0;
}
