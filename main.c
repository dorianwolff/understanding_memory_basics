#include "game.h"

int main() {

    // Initialize the game map (5x5 grid)
    struct GameMap *map = create_map(5, 5);

    // Initialize the player starting at position (0, 0) with 100 health
    struct Player *player = create_player(0, 0, 100, 20);

    // Initialize monsters
    struct Monster *monster_list = create_monster(2, 2, 30, 10); // Add one monster for now

    // Initialize the player's inventory
    struct Inventory inventory = { NULL };

    // Game loop
    char command;
    while (1) {
        //system("clear");  // Use "cls" for Windows
        print_map(map, player, monster_list);
        printf("\nInventory: ");
        print_inventory(&inventory);

        printf("\nMove (z/q/s/d), pick up item (p), or leave (l): ");
        scanf(" %c", &command);

        if (command == 'l') {
            break; // Exit the game loop
        } else {
            move_player(map, player, command, &monster_list, &inventory);

        }
    }

    // Cleanup
    free_map(map);
    free_inventory(&inventory);
    free_monsters(monster_list);
    free(player);

    return 0;
}
