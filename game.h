#ifndef GAME_H
#define GAME_H

#include "gba.h"

#define MAX_VOTES 49
#define MAX_ELECTORAL_VOTES 531;
#define NUM_CANDIDATES 3

typedef struct candidate {
	int x;
  int y;
  u16 height;
  u16 width;
  const u16 *image;
  char *name;
  u32 points;
} Candidate;

typedef struct vote {
  int x;
  int y;
  int points;
  u16 enabled;
} Vote;

typedef struct state {
	Candidate candidates[NUM_CANDIDATES];
	Vote votes[MAX_VOTES];
	int totalX;
  int totalY;
  u16 votesLeft;
  int centerX;
  int centerY;
  u16 previousButtons;
  u16 currentButtons;
} State;


#endif
