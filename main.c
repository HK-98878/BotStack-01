#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "header.h"

Gun initGun(int **bullets);
Item getActionItem(Player *, ItemAction);

void addItem(Player *);

int main() {
  srand(time(NULL));

  Player p1 = {0, 0, 0, 0}, p2 = {0, 0, 0, 0};
  ItemAction item_action;
  TurnAction turn_action;
  Bullet nextBullet;

  int *bullets = NULL;
  int starting_player = rand() % 2;
  int currentIndex, player, reload_starting_player, skipOpponentFlag, player_abstain_flag, p1Abstain, p2Abstain;
  int nAbstains = 0, roundAbstains = 0;
  Gun gun;

  for (int round = 0; round < N_ROUNDS; ++round) {
    p1 = (Player){p1.rounds, MAX_HEALTH, GENERATE_ITEM, GENERATE_ITEM};
    p2 = (Player){p2.rounds, MAX_HEALTH, GENERATE_ITEM, GENERATE_ITEM};
    gun = initGun(&bullets);

    currentIndex = 0;
    reload_starting_player = starting_player = player = !starting_player;
    skipOpponentFlag = 0, player_abstain_flag = 0;
    p1Abstain = 1, p2Abstain = 1, roundAbstains = 0;

    int totalReloads = N_GUN_LOADS - 1;

    if (nAbstains == 2)
      break;

    printf("--------------- Round %d ---------------\n", round + 1);
    printf("Player %d will start:\n\n", player + 1);

    while (1) {
      printf("Player %d:\n", player + 1);
#ifdef DEBUG
      printf("Current bullets: [");
      for (int i = currentIndex; i < gun.currentBullets + currentIndex; ++i) {
        printf("%d ", bullets[i]);
      }
      printf("]\n");
#endif

      nextBullet = BULLET_Unknown;

      // -------------------- Handle abstain action --------------------
      // If it is the first turns of the player, or second turn of a player when
      // the previous player has requested an abstain
      if ((player == 0 && p1Abstain == 1) || (player == 1 && p2Abstain == 1) ||
          (player == reload_starting_player && player_abstain_flag == 1)) {
        int val;
        if (player == 0) {
          val = player1_abstain(p1, p2, gun, player_abstain_flag);
          p1Abstain = 0;
        }
        else {
          val = player2_abstain(p2, p1, gun, player_abstain_flag);
          p2Abstain = 0;
        }

        if (player_abstain_flag == 1 && val == 1) {
          ROUND_OVER("Both players have abstained - Draw")
          ++nAbstains;
          break;
        }
        else {
          player_abstain_flag = val;
          if (val == 1) {
            player = !player;
            continue;
          }
        }
      }

      // -------------------- Handle item action --------------------
      item_action = player == 0 ? player1_item(p1, p2, gun) : player2_item(p2, p1, gun);

      if (item_action != ACTION_NoItem) {
        Item actionItem = getActionItem(player == 0 ? &p1 : &p2, item_action);

        switch (actionItem) {
          case ITEM_Empty:
            break;
          case ITEM_PeekCurrent:
            nextBullet = bullets[currentIndex];
#ifdef PLAYER_MODE
            if (player == 1)
              printf("\tPeeking current bullet - %s\n", nextBullet ? "live" : "blank");
#endif
            break;
          case ITEM_EjectCurrent:
            if (bullets[currentIndex++] == BULLET_Live)
              --gun.currentLiveBullets;
            if (--gun.currentBullets == 0) {
              if (totalReloads-- == 0) {
                ROUND_OVER("No more reloads - Draw")
                break;
              }
              RELOAD()
              gun = initGun(&bullets);
              currentIndex = 0, player_abstain_flag = 0, p1Abstain = 1, p2Abstain = 1;
              reload_starting_player = player;

              addItem(&p1);
              addItem(&p2);
            }
            break;
          case ITEM_SkipEnemy:
            skipOpponentFlag = 1;
            break;
          case ITEM_HealthKit:
            if (player == 0)
              p1.lives++;
            else
              p2.lives++;
            break;
          case ITEM_ResetGun:
            printf("Reloading the gun\n\n");
            gun = initGun(&bullets);
            currentIndex = 0;
            break;
        }
      }

      // -------------------- Handle action --------------------
      turn_action = player == 0 ? player1_action(p1, p2, gun, nextBullet) : player2_action(p2, p1, gun, nextBullet);
      switch (turn_action) {
        case ACTION_ShootSelf:
          if (bullets[currentIndex] == BULLET_Live) {
            player == 0 ? --p1.lives : --p2.lives;
            if (!skipOpponentFlag) {
              player = !player;
            }
            skipOpponentFlag = 0;
            gun.currentLiveBullets--;

            printf("\tShot self - live round\n");
            break;
          }
          else {
            printf("\tShot self - empty round\n");
            break;
          }
        case ACTION_ShootOther:
          if (bullets[currentIndex] == BULLET_Live) {
            player == 0 ? --p2.lives : --p1.lives;
            if (!skipOpponentFlag) {
              player = !player;
            }
            skipOpponentFlag = 0;
            gun.currentLiveBullets--;

            printf("\tShot other - live round\n");
          }
          else {
            if (!skipOpponentFlag) {
              player = !player;
            }
            skipOpponentFlag = 0;

            printf("\tShot other - empty round\n");
          }
          break;
      }

      if (p1.lives == 0) {
        ROUND_OVER("Player 1 is dead")
        ++p2.rounds;
        break;
      }
      else if (p2.lives == 0) {
        ROUND_OVER("Player 2 is dead")
        ++p1.rounds;
        break;
      }

      currentIndex++;
      if (--gun.currentBullets == 0) {
        if (totalReloads-- == 0) {
          ROUND_OVER("No more reloads - Draw")
          break;
        }

        RELOAD()
        gun = initGun(&bullets);
        currentIndex = 0, player_abstain_flag = 0, p1Abstain = 1, p2Abstain = 1;
        reload_starting_player = player;

        addItem(&p1);
        addItem(&p2);
      }

#ifdef PLAYER_MODE
#ifndef NOPAUSE
      printf("\nPress ENTER to continue:");
      getchar();
#endif
#endif
    }

    if (p1.rounds == 2 || p2.rounds == 2)
      break;
  }

  // End states
  // P1 or P2 win with 2 rounds
  if (p1.rounds == 2)
    GAME_OVER("Player 1 wins")
  else if (p2.rounds == 2)
    GAME_OVER("Player 2 wins")
  // Draw with 2 abstains
  else if (nAbstains == 2)
    GAME_OVER("Draw by abstention")
  // Loss - max 1 win per player
  else if (p1.rounds != 2 && p2.rounds != 2)
    GAME_OVER("Loss")
}

Gun initGun(int **bullets) {
  Gun gun = {rand() % dBULLETS + MIN_BULLETS, 0};
  int ub = (int)(gun.currentBullets / 2.0f + 1) + 1;
  int lb = (int)(gun.currentBullets / 2.0f - 0.5f);

  gun.currentLiveBullets = rand() % (ub - lb) + lb;

  free(*bullets);
  (*bullets) = calloc(gun.currentBullets, sizeof(int));
  int n = gun.currentBullets, nlive = gun.currentLiveBullets;
  for (int i = 0; i < gun.currentBullets; ++i) {
    double chance = (rand() % 101) / 100.0;

    if (chance > (double)nlive / n) {
      (*bullets)[i] = BULLET_Blank;
    }
    else {
      (*bullets)[i] = BULLET_Live;
      --nlive;
    }
    --n;
  }

#ifdef DEBUG
  printf("Total: %d Live: %d\n", gun.currentBullets, gun.currentLiveBullets);
  for (int i = 0; i < gun.currentBullets; ++i) {
    printf("%d ", (*bullets)[i]);
  }
  printf("\n");
#endif

  return gun;
}

Item getActionItem(Player *p, ItemAction item) {
  Item ret;
  switch (item) {
    case ACTION_NoItem:
      return ITEM_Empty;
    case ACTION_Item1:
      ret = p->item1;
      p->item1 = ITEM_Empty;
      return ret;
    case ACTION_Item2:
      ret = p->item2;
      p->item2 = ITEM_Empty;
      return ret;
  }
}

void addItem(Player *p) {
  if (p->item1 == ITEM_Empty)
    p->item1 = GENERATE_ITEM;
  else if (p->item2 == ITEM_Empty)
    p->item2 = GENERATE_ITEM;
}