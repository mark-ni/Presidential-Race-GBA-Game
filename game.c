#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "transition.h"
#include "the_race.h"

typedef enum {
  START,
  PLAY,
  WIN,
  LOSE,
  TIE
} GBAState;

int main(void) {
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  //Setup game state
  State cs, ps;
  cs = initializeGameState();

  //Transition to start screen
  GBAState state = START;
  drawStart();

  while (1) {
    cs.currentButtons = BUTTONS;  // Load the current state of the buttons

    if (KEY_JUST_PRESSED(BUTTON_SELECT, cs.currentButtons, cs.previousButtons)) {
      state = START;
      
      drawStart();
    }

    switch (state) {
      case START:
        waitForVBlank();

        if (KEY_JUST_PRESSED(BUTTON_START, cs.currentButtons, cs.previousButtons)) {
          state = PLAY;

          cs = resetMainGameState(cs);
          drawTransitionMainGame(cs);
        }
        break;

      case PLAY:
        ps = cs;

        cs = updateCandidatePositions(cs);
        cs = checkVoteCollisions(cs);

        waitForVBlank();
        
        //undraw score (layer 0)
        drawRectDMA(5, 50, 60, 8, BLACK);
        //drawback candidates (layer 0 and 1)
        drawCandidates(cs, ps);
        //draw score (layer 2)
        drawScore(cs);

        if (cs.votesLeft == 0) {
          if (cs.candidates[0].points > cs.candidates[1].points && cs.candidates[0].points > cs.candidates[2].points) {
            state = WIN;
            drawWin(cs);
          } else if (cs.candidates[0].points < cs.candidates[1].points || cs.candidates[0].points < cs.candidates[2].points) {
            state = LOSE;
            drawLoss(cs);
          } else {
            state = TIE;
            drawTie(cs);
          }
        }
        break;

      case WIN:
        if (KEY_JUST_PRESSED(BUTTON_B, cs.currentButtons, cs.previousButtons)) {
          state = START;

          drawStart();
        }
        break;

      case LOSE:
        if (KEY_JUST_PRESSED(BUTTON_B, cs.currentButtons, cs.previousButtons)) {
          state = START;

          drawStart();
        }
        break;

      case TIE:
        if (KEY_JUST_PRESSED(BUTTON_B, cs.currentButtons, cs.previousButtons)) {
          state = START;

          drawStart();
        }
        break;
    }
    cs.previousButtons = cs.currentButtons;  // Store the current state of the buttons
  }
  return 0;
}