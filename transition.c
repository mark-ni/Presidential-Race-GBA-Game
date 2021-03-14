#include <stdio.h>
#include "game.h"
#include "gba.h"
#include "images/map.h"
#include "images/win.h"
#include "images/winScreen.h"
#include "images/embarrassedYe.h"
#include "images/biden.h"
#include "images/kanye.h"
#include "images/trump.h"
#include "images/tieScreen.h"

//Initialize votes
const u16 votePts[MAX_VOTES] = {9,11,6,55,9,7,3,3,29,16,4,20,11,6,6,8,8,4,10,11,16,10,6,10,5,6,4,14,5,29,15,3,18,7,20,4,9,3,11,38,6,3,13,12,5,10,3,3,7};
const u16 voteYs[MAX_VOTES] = {162,49,138,16,78,213,207,198,189,177,45,148,160,130,109,168,136,220,201,216,164,124,148,134,105,26,214,208,73,201,194,102,174,113,194,218,188,102,163,111,55,209,195,27,184,144,77,61,16};
const u16 voteXs[MAX_VOTES] = {112,101,102,80,80,51,69,72,133,111,49,70,74,64,83,87,127,28,69,46,54,41,114,84,68,68,40,63,105,49,92,33,70,101,63,50,103,51,96,123,70,39,80,19,77,48,61,30,36};

State initializeGameState(void) {
	State cs;
	cs.candidates[0].height = KANYE_HEIGHT;
  cs.candidates[0].width = KANYE_WIDTH;
  cs.candidates[0].image = kanye;
  cs.candidates[0].name = "Kanye\0";

  //TODO: initialize trump
  cs.candidates[1].height = TRUMP_HEIGHT;
  cs.candidates[1].width = TRUMP_WIDTH;
  cs.candidates[1].image = trump;
  cs.candidates[1].name = "Trump\0";

  //TODO: initialize biden
  cs.candidates[2].height = BIDEN_HEIGHT;
  cs.candidates[2].width = BIDEN_WIDTH;
  cs.candidates[2].image = biden;
  cs.candidates[2].name = "Biden\0";

  cs.currentButtons = BUTTONS;
  cs.previousButtons = BUTTONS;
	return cs;
}

void drawStart(void) {
	waitForVBlank();
	drawFullScreenImageDMA(kanyeWinsResized);
  drawString(25, 85, "PRESS START\0", GREEN);
}

State resetMainGameState(State cs) {
	//set up votes
  cs.totalX = 0;
  cs.totalY = 0;
  for (int i = 0; i < MAX_VOTES; i++) {
    cs.votes[i].x = voteXs[i];
    cs.totalX += voteXs[i] * votePts[i];
    cs.votes[i].y = voteYs[i];
    cs.totalY += voteYs[i] * votePts[i];
    cs.votes[i].points = votePts[i];
    cs.votes[i].enabled = 1;
  }
  cs.votesLeft = MAX_ELECTORAL_VOTES;

  //set up candidates
  //Kanye
  cs.candidates[0].x = 75;
  cs.candidates[0].y = 16;
  cs.candidates[0].points = 0;
  //Trump
  cs.candidates[1].x = 127;
  cs.candidates[1].y = 192;
  cs.candidates[1].points = 3;
  //Biden
  cs.candidates[2].x = 25;
  cs.candidates[2].y = 216;
  cs.candidates[2].points = 4;
  return cs;
}

void drawTransitionMainGame(State cs) {
	waitForVBlank();
	drawFullScreenImageDMA(usmap);
  //Draw votes
  for (u16 i = 0; i < MAX_VOTES; i++) {
    setPixel(cs.votes[i].x, cs.votes[i].y + 1, RED);
    setPixel(cs.votes[i].x + 1, cs.votes[i].y, RED);
    setPixel(cs.votes[i].x + 1, cs.votes[i].y + 1, RED);
    setPixel(cs.votes[i].x, cs.votes[i].y, RED);
  }
}

void drawWin(State cs) {
	waitForVBlank();
	drawFullScreenImageDMA(winScreen);
  drawRectDMA(132, 20, 200, 25, WHITE);
  drawCenteredString(140, 25, 190, 10, "You won! Press B to run again.\0", BLACK);

  char buffer[13];
  drawRectDMA(0, 165, 75, 60, BLACK);
  drawString(5, 170, "Final Tally\0", RED);
  drawString(15, 170, "-----------\0", RED);
  sprintf(buffer, "Kanye %d", cs.candidates[0].points);
  drawString(25, 170, buffer, RED);
  sprintf(buffer, "Trump %d", cs.candidates[1].points);
  drawString(35, 170, buffer, RED);
  sprintf(buffer, "Biden %d", cs.candidates[2].points);
  drawString(45, 170, buffer, RED);
}

void drawLoss(State cs) {
	waitForVBlank();
	drawFullScreenImageDMA(embarrassedYe);     
  drawRectDMA(132, 20, 200, 25, WHITE);
  drawCenteredString(140, 25, 190, 10, "You lost! Press B to run again.\0", BLACK);

  char buffer[13];
  drawRectDMA(0, 0, 75, 60, BLACK);
  drawString(5, 5, "Final Tally\0", RED);
  drawString(15, 5, "-----------\0", RED);
  sprintf(buffer, "Kanye %d", cs.candidates[0].points);
  drawString(25, 5, buffer, RED);
  sprintf(buffer, "Trump %d", cs.candidates[1].points);
  drawString(35, 5, buffer, RED);
  sprintf(buffer, "Biden %d", cs.candidates[2].points);
  drawString(45, 5, buffer, RED);
}

void drawTie(State cs) {
  waitForVBlank();
  drawFullScreenImageDMA(tieScreen);
  drawRectDMA(132, 20, 200, 25, WHITE);
  drawCenteredString(140, 25, 190, 10, "It's a tie! Press B to run again.\0", BLACK);

  char buffer[13];
  drawRectDMA(0, 0, 75, 60, BLACK);
  drawString(5, 5, "Final Tally\0", RED);
  drawString(15, 5, "-----------\0", RED);
  sprintf(buffer, "Kanye %d", cs.candidates[0].points);
  drawString(25, 5, buffer, RED);
  sprintf(buffer, "Trump %d", cs.candidates[1].points);
  drawString(35, 5, buffer, RED);
  sprintf(buffer, "Biden %d", cs.candidates[2].points);
  drawString(45, 5, buffer, RED);
}