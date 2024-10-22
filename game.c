#include "game.h"

struct GameMap *create_map(int rows, int cols)
{
    struct GameMap *map = malloc(sizeof(struct GameMap));

    if (!map)
    {
        printf("map creation failed!\n");
        exit(1);
    }

    map->rows = rows;
    map->cols = cols;

    // Allocate memory for the array of row pointers
    map->map = malloc(rows * sizeof(char *));
    if (!map->map) {
        printf("Failed to allocate memory for map rows\n");
        free(map); // Free the previously allocated memory
        exit(1);   // Exit if memory allocation fails
    }

    // Allocate memory for each row
    for (int i = 0; i < rows; i++)
    {
        map->map[i] = malloc(cols * sizeof(char));
        if (!map->map[i]) {
            printf("Memory allocation failed for map columns.\n");
            // Free previously allocated rows
            for (int j = 0; j < i; j++) {
                free(map->map[j]);
            }
            free(map->map);
            free(map);
            exit(1);
        }
    }

    // Initialize the map
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            map->map[i][j] = '.';
        }
    }
    return map;
}

void free_map(struct GameMap *game_map)
{
    if (!game_map || !game_map->map)
        return;

    for (int i = 0; i < game_map->rows; i++)
    {
        free(game_map->map[i]);
    }

    free(game_map->map);

    free(game_map);
}

void print_map(struct GameMap *game_map, struct Player *player, struct Monster *monster_list)
{
    if (!game_map || !player || !game_map->map)
        return;

    // Check each monster's position and mark it on the map with 'M'
    for (int i = 0; i < game_map->rows; i++)
    {
        for (int j = 0; j < game_map->cols; ++j)
        {
            int is_player = (i == player->y && j == player->x);
            int is_monster = 0;
            struct Monster *temp = monster_list;
            while (temp) {
                if (i == temp->y && j == temp->x) {
                    is_monster = 1;
                    break;
                }
                temp = temp->next;
            }

            if (is_player)
                printf("P");
            else if (is_monster)
                printf("M");
            else
                printf("%c", game_map->map[i][j]);
        }
        printf("\n");
    }
}

void player_combat(struct Player *player, struct Monster **monster_list, struct Inventory *inventory) {
    struct Monster *current = *monster_list;
    struct Monster *prev = NULL;

    while (current) {
        if (player->x == current->x && player->y == current->y) {
            // Clear the screen before the fight
            //system("clear");  // On Windows, use "cls"

            printf("Combat started with monster at (%d, %d)\n", current->x, current->y);

            // Player attacks the monster
            current->health -= player->attack_power;
            printf("You attacked the monster! Monster's health: %d\n", current->health);

            // Monster retaliates
            if (current->health > 0) {
                player->health -= current->attack_power;
                printf("The monster attacked you! Your health: %d\n", player->health);

                if (player->health <= 0) {
                    printf("You were defeated by the monster!\n");
                    exit(1);
                }
            }

            // Monster is defeated
            if (current->health <= 0) {
                printf("You defeated the monster!\n");

                // Add random item to inventory
                char *item_name = "Sword"; // Simplified random item drop
                add_item(inventory, item_name);
                printf("You found a %s!\n", item_name);

                // Remove monster from the list
                if (prev)
                    prev->next = current->next;
                else
                    *monster_list = current->next;

                free(current);
            }

            // Wait for player to press a key before resuming gameplay
            printf("\nPress ENTER to continue...\n");
            getchar();  // Waits for the user to press ENTER

            // Clear the screen again after combat
            //system("clear");  // On Windows, use "cls"

            return;
        }
        prev = current;
        current = current->next;
    }
}


struct Player *create_player(int start_x, int start_y, int health, int attack_power)
{
    struct Player *player = malloc(sizeof(struct Player));
    if (!player)
    {
        printf("Failed to allocate memory for player\n");
        exit(1);   // Exit if memory allocation fails
    }
    player->x = start_x;
    player->y = start_y;
    player->health = health;
    player->attack_power = attack_power;

    return player;
}

void move_player(struct GameMap *map, struct Player *player, char direction, struct Monster **monster_list, struct Inventory *inventory)
{
    if (!map || !player || !map->map)
        return;

    switch (direction)
    {
        case 'z':
            if (player->y > 0)
                player->y -= 1;
            break;
        case 'q':
            if (player->x > 0)
                player->x -= 1;
            break;
        case 's':
            if (player->y < map->rows - 1)
                player->y += 1;
            break;
        case 'd':
            if (player->x < map->cols - 1)
                player->x += 1;
            break;
        default:
            printf("Invalid Move\n");
    }

    // Check if the player encounters a monster and pass the inventory to combat
    player_combat(player, monster_list, inventory);
}


void add_item(struct Inventory *inventory, const char *item_name)
{
    struct Item *newItem = malloc(sizeof(struct Item));
    if (!newItem)
    {
        printf("Failed to allocate memory for new item\n");
        exit(1);
    }

    int length = 0;
    while (item_name[length] != '\0') {
        length++;
    }

    newItem->name = malloc(sizeof(char) * (length + 1));
    if (!newItem->name)
    {
        printf("Failed to allocate memory for item name\n");
        free(newItem);
        exit(1);
    }

    for (int i = 0; i <= length; i++) {
        newItem->name[i] = item_name[i]; // This includes the null terminator
    }

    newItem->next = inventory->head;
    inventory->head = newItem;
}


void print_inventory(struct Inventory *inventory)
{
    if (!inventory)
        return;
    struct Item *temp = inventory->head;
    if (!temp)
    {
        printf("No items in inventory\n");
        return;
    }
    while (temp)
    {
        if (temp->name)
            printf("| %s ", temp->name);
        else
            printf("| (Unnamed Item) ");
        temp = temp->next;
    }
    printf("|\n");
}

void free_inventory(struct Inventory *inventory)
{
    if (!inventory)
        return;
    while (inventory->head)
    {
        struct Item *temp = inventory->head->next;
        free(inventory->head->name);
        free(inventory->head);
        inventory->head = temp;
    }
}

struct Monster *create_monster(int x, int y, int health, int attack_power) {
    struct Monster *monster = malloc(sizeof(struct Monster));
    if (!monster) {
        printf("Failed to allocate memory for monster\n");
        exit(1);
    }
    monster->x = x;
    monster->y = y;
    monster->health = health;
    monster->attack_power = attack_power;
    monster->next = NULL;
    return monster;
}

void free_monsters(struct Monster *monster_list) {
    while (monster_list) {
        struct Monster *temp = monster_list;
        monster_list = monster_list->next;
        free(temp);
    }
}
