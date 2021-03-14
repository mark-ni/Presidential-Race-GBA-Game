#ifndef THE_RACE_H
#define THE_RACE_H

#include "game.h"

State updateCandidatePositions(State ps);
State checkVoteCollisions(State cs);
void drawCandidates(State cs, State ps);
void drawScore(State cs);

#endif