#include "header.h"

/* ----------------------------------------
The terminal commands for testing the bot can be found in the "cmds.txt" file.
  Ensure the directory you are running the terminal in is correct or things will
  not load correctly at runtime
---------------------------------------- */

int player1_abstain(Player self, Player other, Gun state, int opponentRequest) {
  /* ----------------------------------------
  This is a technically optional piece of logic. The abstention mechanic is
    detailed in the game mechanics, but you have the option to not interact
    at all with this system - in that case, simply leave this function
    untouched.

  If you do want to utilise this mechanic, return 1 to request an abstain,
    or return 0 to deny.
  If the "opponentRequest" value is 1, then your opponent has also requested
    an abstain, and you are accepting or denying this. Otherwise, it is your
    initiative to request an abstention.
  ---------------------------------------- */
  return 1;
}
ItemAction player1_item(Player self, Player other, Gun state) {
  /* ----------------------------------------
  This is the first major function you will be modifying in order to create
    your bot

  It is responsible for using your available items if you wish to do so - you
   can return one of [ACTION_NoItem, ACTION_Item1, ACTION_Item2].
  Your available items are visible at [self.item1] and [self.item2] (as well
   as all other data for both yourself and opponent)
  ---------------------------------------- */
  return ACTION_NoItem;
}
TurnAction player1_action(Player self, Player other, Gun state, Bullet nextBullet) {
  /* ----------------------------------------
  This is the second major function you will be modifying in order to create
    your bot

  It is responsible for deciding who to shoot. You can see your own and your
   opponent's data, and the current total bullets as well as the total live
   bullets in the [state] struct.

  If your previous action was [ITEM_PeekCurrent], the value of nextBullet will
   be either [BULLET_Live | BULLET_Blank] depending on the observed bullet.
   Otherwise, this value will be BULLET_UNKNOWN
  ---------------------------------------- */
  return ACTION_ShootSelf;
}