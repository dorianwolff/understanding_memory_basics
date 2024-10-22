#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>

// Define the game map as a 2D matrix of characters
struct GameMap {
    int rows;
    int cols;
    char **map; // 2D array representing the map
};

// Define the player structure
struct Player {
    int x; // Player's current x position (column)
    int y; // Player's current y position (row)
    int health;
    int attack_power;
};

// Define the monster structure
struct Monster {
    int x; // Monster's current x position
    int y; // Monster's current y position
    int health;
    int attack_power;
    struct Monster *next; // Pointer to the next monster (linked list)
};

// Linked list node for inventory items
struct Item {
    char *name;
    struct Item *next; // Pointer to the next item
};

// Define the inventory
struct Inventory {
    struct Item *head; // Pointer to the first item
};

// Function declarations
struct GameMap *create_map(int rows, int cols);
void free_map(struct GameMap *game_map);
void print_map(struct GameMap *game_map, struct Player *player, struct Monster *monster_list);
struct Player *create_player(int start_x, int start_y, int health, int attack_power);
struct Monster *create_monster(int x, int y, int health, int attack_power);
void move_player(struct GameMap *map, struct Player *player, char direction, struct Monster **monster_list,
        struct Inventory *inventory);
void add_item(struct Inventory *inventory, const char *item_name);
void print_inventory(struct Inventory *inventory);
void free_inventory(struct Inventory *inventory);
void free_monsters(struct Monster *monster_list);
void player_combat(struct Player *player, struct Monster **monster_list, struct Inventory *inventory);

#endif // GAME_H
