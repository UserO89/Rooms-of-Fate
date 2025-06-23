#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NAME_LEN 50
#define MAX_ROOMS 5

#define ARMOR_COST 30

typedef struct {
    char name[MAX_NAME_LEN];
    int health;
    int currentRoom;
    int gold;
    int hasSword;
    int hasArmor;
} Player;

typedef struct {
    char description[180];
    int hasRandomEvent;
    int goldAmount;
    int hasNPC;
    int hasTrader;
    int hasBoss;
    int bossHealth;
} Room;

void saveGame(Player* player) {
    char filename[100];
    snprintf(filename, sizeof(filename), "%s.save", player->name);
    FILE* file = fopen(filename, "wb");
    if (file) {
        fwrite(player, sizeof(Player), 1, file);
        fclose(file);
        printf("Game saved!\n");
    } else {
        printf("Failed to save the game.\n");
    }
}

void loadGame(Player* player, char* playerName) {
    char filename[100];
    snprintf(filename, sizeof(filename), "%s.save", playerName);
    FILE* file = fopen(filename, "rb");
    if (file) {
        fread(player, sizeof(Player), 1, file);
        fclose(file);
        printf("Save found. Welcome back, %s!\n", player->name);
    } else {
        memset(player, 0, sizeof(Player));
    }
}

Room* createWorldMap() {
    Room* rooms = malloc(sizeof(Room) * MAX_ROOMS);

    for (int i = 0; i < MAX_ROOMS; i++) {
        snprintf(rooms[i].description, sizeof(rooms[i].description), "You are in room #%d.", i + 1);
        rooms[i].hasRandomEvent = rand() % 2;
        rooms[i].goldAmount = rand() % 20;
        rooms[i].hasNPC = rand() % 2;
        rooms[i].hasTrader = (rand() % 4 == 0);
        rooms[i].hasBoss = 0;
        rooms[i].bossHealth = 0;
    }

    rooms[MAX_ROOMS - 1].hasBoss = 1;
    rooms[MAX_ROOMS - 1].bossHealth = 100;
    snprintf(rooms[MAX_ROOMS - 1].description, sizeof(rooms[MAX_ROOMS - 1].description), 
             "This is the final room. A powerful boss awaits!");

    return rooms;
}

void displayRoom(Room* rooms, int roomIndex, Player* player) {
    Room room = rooms[roomIndex];
    printf("\n\033[32m%s\033[0m\n", room.description);

    if (room.goldAmount > 0)
        printf("You see some gold here: \033[33m%d\033[0m coins.\n", room.goldAmount);
    if (room.hasNPC)
        printf("There is someone here who wants to talk to you.\n");
    if (room.hasTrader)
        printf("A trader offers to sell you gear.\n");
    if (room.hasBoss)
        printf("A terrifying boss stands before you!\n");

    printf("Your stats: ❤️ %d | 💰 %d | 🗡️ %s | 🛡️ %s\n", 
           player->health, player->gold,
           player->hasSword ? "Yes" : "No",
           player->hasArmor ? "Yes" : "No");
}

void playRandomEvent(Player* player) {
    int event = rand() % 3;
    switch (event) {
        case 0:
            printf("You fell into a trap and lost 10 HP!\n");
            player->health -= 10;
            break;
        case 1:
            printf("You found a healing potion and recovered 15 HP!\n");
            player->health += 15;
            break;
        case 2:
            printf("You encountered a thief and lost 5 gold!\n");
            if (player->gold >= 5) {
                player->gold -= 5;
            } else {
                player->gold = 0;
            }
            break;
    }
}

int main(int argc, char *argv[]) {
    char playerName[MAX_NAME_LEN] = "";

    if (argc > 1) {
        strncpy(playerName, argv[1], MAX_NAME_LEN - 1);
        playerName[MAX_NAME_LEN - 1] = '\0';
    } else {
        printf("Enter the name of the player: ");
        if (fgets(playerName, sizeof(playerName), stdin) != NULL) {
            size_t len = strlen(playerName);
            if (len > 0 && playerName[len - 1] == '\n') {
                playerName[len - 1] = '\0';
            }
        }
    }

    srand(time(NULL));

    Player player;
    loadGame(&player, playerName);
    if (strlen(player.name) == 0) {
        strncpy(player.name, playerName, MAX_NAME_LEN);
        player.name[MAX_NAME_LEN - 1] = '\0';
        player.health = 100;
        player.currentRoom = 0;
        player.gold = 0;
        player.hasSword = 0;
        player.hasArmor = 0;
        printf("Welcome, %s! Let's start a new game.\n", player.name);
    }

    Room* worldMap = createWorldMap();

    int gameRunning = 1;
    while (gameRunning) {
        displayRoom(worldMap, player.currentRoom, &player);

        if (worldMap[player.currentRoom].hasRandomEvent) {
            playRandomEvent(&player);
            worldMap[player.currentRoom].hasRandomEvent = 0;
        }

        if (player.health <= 0) {
            printf("You have died. Game over.\n");
            break;
        }

        printf("\n\033[36m[1] Next Room\n");
        if (worldMap[player.currentRoom].goldAmount > 0)
            printf("[2] Collect Gold\n");
        printf("[3] Save Game\n");
        printf("[4] Quit\033[0m\n");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                player.currentRoom = (player.currentRoom + 1) % MAX_ROOMS;
                break;
            case 2:
                if (worldMap[player.currentRoom].goldAmount > 0) {
                    printf("You collected %d gold!\n", worldMap[player.currentRoom].goldAmount);
                    player.gold += worldMap[player.currentRoom].goldAmount;
                    worldMap[player.currentRoom].goldAmount = 0;
                } else {
                    printf("There's no gold here.\n");
                }
                break;
            case 3:
                saveGame(&player);
                break;
            case 4:
                gameRunning = 0;
                break;
            default:
                printf("Invalid option.\n");
        }
    }
}