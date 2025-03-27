#include "header.h"

#ifndef PLAYER_MODE

/* ----------------------------------------
If you want to create a second bot, these are the other functions you will be
modifying

The terminal commands for testing the bots against each-other can also be found
  in the "cmds.txt" file. Ensure the directory you are running the terminal in
  is correct or things will not load correctly at runtime
---------------------------------------- */

int player2_abstain(Player self, Player other, Gun state, int opponentRequest) { return 0; }
ItemAction player2_item(Player self, Player other, Gun state) { return ACTION_NoItem; }
TurnAction player2_action(Player self, Player other, Gun state, Bullet nextBullet) { return ACTION_ShootOther; }

// Do not touch the following code:
#else
#include <stdio.h>

void gameState(Player self, Player other, Gun state, int displayItems) {
  printf("\tCurrent game state:\n");
  printf("\t  Health | Self: %d   | Opponent: %d\n", self.lives, other.lives);
  printf("\t  Gun:   | Total: %d  | Live: %d\n\n", state.currentBullets, state.currentLiveBullets);

  if (!displayItems)
    return;
  printf("\t  Available items: [%s, %s]\n", ItemToString(self.item1), ItemToString(self.item2));
  printf("\t  Opponents items: [%s, %s]\n\n", ItemToString(other.item1), ItemToString(other.item2));
}

int player2_abstain(Player self, Player other, Gun state, int opponentRequest) {
  gameState(self, other, state, 1);

  char str[5];
  while (1) {
    if (!opponentRequest)
      printf("\tWould you like to request an abstain - [0:no, 1:yes]: ");
    else
      printf("\tYour opponent has requested an abstain; do you accept - [0:no, 1:yes]: ");

    if (fgets(str, 5, stdin) == NULL)
      continue;

    if (str[0] == '0')
      return 0;
    else if (str[0] == '1')
      return 1;
  }
}

ItemAction player2_item(Player self, Player other, Gun state) {
  gameState(self, other, state, 1);

  char str[5];
  while (1) {
    printf("\tEnter the item you want to use - [0:none, 1:item 1, 2:item 2]: ");

    if (fgets(str, 5, stdin) == NULL)
      continue;

    if (str[0] == '0')
      return ACTION_NoItem;
    else if (str[0] == '1')
      return ACTION_Item1;
    else if (str[0] == '2')
      return ACTION_Item2;
  }
}
TurnAction player2_action(Player self, Player other, Gun state, Bullet nextBullet) {
  gameState(self, other, state, 0);

  char str[5];
  while (1) {
    printf("\tEnter who you want to fire at - [0:self, 1:opponent]: ");
    if (fgets(str, 5, stdin) == NULL)
      continue;

    if (str[0] == '0')
      return ACTION_ShootSelf;
    else if (str[0] == '1')
      return ACTION_ShootOther;
  }
}
#endif