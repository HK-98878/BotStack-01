#include <string.h>

#define N_ROUNDS    3
#define N_GUN_LOADS 3
#define MAX_HEALTH  3

#define MIN_BULLETS 3
#define MAX_BULLETS 7
#define dBULLETS    (MAX_BULLETS - MIN_BULLETS + 1)

#define GENERATE_ITEM (Item)(rand() % ITEM_COUNT) + 1

#define ROUND_OVER(msg)                                                                                          \
  printf("\n--------------- Round Over ---------------\n    " msg "\n------------------------------------------" \
         "\n\n");
#define GAME_OVER(msg)                                                      \
  printf("------------------------------------------\n    Game Over - " msg \
         "\n------------------------------------------\n");
#define RELOAD()                                                                              \
  printf("\n------------------------------------------\n    No more bullets - Reloading the " \
         "gun\n------------------------------------------\n\n");

// Enums
typedef enum TurnAction { ACTION_ShootSelf, ACTION_ShootOther } TurnAction;
typedef enum ItemAction { ACTION_NoItem, ACTION_Item1, ACTION_Item2 } ItemAction;
typedef enum Item {
  ITEM_Empty,
  ITEM_PeekCurrent,
  ITEM_EjectCurrent,
  ITEM_SkipEnemy,
  ITEM_HealthKit,
  ITEM_ResetGun
} Item;
#define ITEM_COUNT 5

typedef enum Bullet { BULLET_Blank = 0, BULLET_Live = 1, BULLET_Unknown } Bullet;

static const char *ItemToString(Item item) {
  switch (item) {
    case ITEM_Empty:
      return "None";
    case ITEM_PeekCurrent:
      return "Peek at current shell";
    case ITEM_EjectCurrent:
      return "Eject current shell";
    case ITEM_SkipEnemy:
      return "Skip next opponent turn";
    case ITEM_HealthKit:
      return "Gain one health";
    case ITEM_ResetGun:
      return "Empty and reload the gun";
  }
}

typedef struct Player {
  int rounds;
  int lives;
  Item item1;
  Item item2;
} Player;

typedef struct Gun {
  int currentBullets;
  int currentLiveBullets;
} Gun;

// Functions
int player1_abstain(Player self, Player other, Gun state, int opponentRequest);
ItemAction player1_item(Player self, Player other, Gun state);
TurnAction player1_action(Player self, Player other, Gun state, Bullet nextBullet);

int player2_abstain(Player self, Player other, Gun state, int opponentRequest);
ItemAction player2_item(Player self, Player other, Gun state);
TurnAction player2_action(Player self, Player other, Gun state, Bullet nextBullet);