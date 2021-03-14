#ifndef TRANSITION_H
#define TRANSITION_H

#include "game.h"
#include "gba.h"

extern const u16 votePts[49];
extern const u16 voteYs[49];
extern const u16 voteXs[49];

State initializeGameState(void);
void drawStart(void);
State resetMainGameState(State cs);
void drawTransitionMainGame(State cs);
void drawWin(State cs);
void drawLoss(State cs);
void drawTie(State cs);

#endif