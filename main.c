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
