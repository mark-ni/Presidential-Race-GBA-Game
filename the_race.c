#include "game.h"
#include "gba.h"
#include "images/map.h"
#include <stdio.h>

State updateCandidatePositions(State cs) {
	//Move Kanye
  if (KEY_DOWN(BUTTON_UP, cs.currentButtons)) {
    cs.candidates[0].x -= 1;
  }
  if (KEY_DOWN(BUTTON_DOWN, cs.currentButtons)) {
    cs.candidates[0].x += 1;
  }
  if (KEY_DOWN(BUTTON_RIGHT, cs.currentButtons)) {
    cs.candidates[0].y += 1;
  }
  if (KEY_DOWN(BUTTON_LEFT, cs.currentButtons)) {
    cs.candidates[0].y -= 1;
  }

	//Move Trump and Biden based on center of vote distribution
  cs.centerX = cs.totalX / cs.votesLeft;
  cs.centerY = cs.totalY / cs.votesLeft;
  for (int i = 1; i < NUM_CANDIDATES; i++) {
    if (cs.candidates[i].x < cs.centerX) {
      cs.candidates[i].x += 1;
    } else {
      cs.candidates[i].x -= 1;
    }
    if (cs.candidates[i].y < cs.centerY) {
      cs.candidates[i].y += 1;
    } else {
      cs.candidates[i].y -= 1;
    }
    cs.candidates[i].x += randint(-3, 4);
    cs.candidates[i].y += randint(-3, 4);
  }

  //Adjust candidates' positions within bounds
  for (int i = 0; i < NUM_CANDIDATES; i++) {
    if (cs.candidates[i].x < 0) {
      cs.candidates[i].x = 0;
    } else if (cs.candidates[i].x > HEIGHT - 1 - cs.candidates[i].height) {
      cs.candidates[i].x = HEIGHT - 1 - cs.candidates[i].height;
    }
    if (cs.candidates[i].y < 0) {
      cs.candidates[i].y = 0;
    } else if (cs.candidates[i].y > WIDTH - 1 - cs.candidates[i].width) {
      cs.candidates[i].y = WIDTH - 1 - cs.candidates[i].width;
    }
  }
  return cs;
}

State checkVoteCollisions(State cs) {
  for (int i = 0; i < MAX_VOTES; i++) {
    if (cs.votes[i].enabled) {
      for (int j = 0; j < NUM_CANDIDATES; j++) {
        if (cs.votes[i].x >= cs.candidates[j].x && cs.votes[i].x <= cs.candidates[j].x + cs.candidates[j].height - 2 
          && cs.votes[i].y >= cs.candidates[j].y && cs.votes[i].y <= cs.candidates[j].y + cs.candidates[j].width - 2) {

          cs.votes[i].enabled = 0;
          cs.candidates[j].points += cs.votes[i].points;
          cs.votesLeft -= cs.votes[i].points;
          cs.totalX -= cs.votes[i].x * cs.votes[i].points;
          cs.totalY -= cs.votes[i].y * cs.votes[i].points;
          break;
        }
      }
    }
  }
  return cs;
}

void drawCandidates(State cs, State ps) {
  //Update each Candidate
  for (int i = 0; i < NUM_CANDIDATES; i++) {
    //Erase previous image of candidates
    drawBackImageDMA(ps.candidates[i].x, ps.candidates[i].y, ps.candidates[i].width,
      ps.candidates[i].height, usmap);
  }
  for (int i = 0; i < NUM_CANDIDATES; i++) {
    //Create new image of candidates
    drawImageDMA(cs.candidates[i].x, cs.candidates[i].y, cs.candidates[i].width,
      cs.candidates[i].height, cs.candidates[i].image);
  }
}

void drawScore(State cs) {
	char buffer[40];
  sprintf(buffer, "Votes: %d", cs.candidates[0].points);
  drawString(5, 50, buffer, RED);
}