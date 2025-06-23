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
    // In next steps: displaying rooms and interaction...
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

    // Last room has the boss
    rooms[MAX_ROOMS - 1].hasBoss = 1;
    rooms[MAX_ROOMS - 1].bossHealth = 100;
    snprintf(rooms[MAX_ROOMS - 1].description, sizeof(rooms[MAX_ROOMS - 1].description), 
             "This is the final room. A powerful boss awaits!");

    return rooms;
}
