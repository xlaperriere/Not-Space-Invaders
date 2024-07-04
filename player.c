#include <stdbool.h>

#include "constants.h"

// Player struct to keep track of the state of the character
struct Player {
  double xPos;
  double yPos;
  int activeBullets;
  int lives;
  bool playerAlive;
  int lastBulletTime;
};

// Bullet struct to keep track of its state
struct Bullet {
  double xPos;
  double yPos;
  bool isAlive;
};

// Static array of bullets to keep track of them
struct Bullet BULLETS[MAX_BULLETS];

bool LIVES[3];

// Initialisation function of the player (character)
void initPlayer(struct Player *p) {
  p->xPos = INITIAL_X;
  p->yPos = INITIAL_Y;
  p->lives = 3;
  p->activeBullets = 0;
  p->playerAlive = true;
  p->lastBulletTime = 0;
  for(int i = 0; i < 3; i++){
    LIVES[i] = true;
  }
}

// ###################################
//         Movement functions
// ###################################

void moveRight(struct Player *p) {
  p->xPos += 6;
}

void moveLeft(struct Player *p) {
  p->xPos -= 6;
}

void moveUp(struct Player *p) {
  p->yPos -= 4;
}

void moveDown(struct Player *p) {
  p->yPos += 4;
}

void resetPosition(struct Player *p) {
  p->xPos = INITIAL_X;
  p->yPos = INITIAL_Y;
}


// #######################################
//            Bullet functions
// #######################################

// Shoot bullets
void shootBullet(struct Player *p, int time) {
  
  if(time - p->lastBulletTime > 1){
    struct Bullet bullet;
    bullet.xPos = p->xPos + CHARACTER_W / 2;
    bullet.yPos = INITIAL_Y;
    bullet.isAlive = true;

    if (p->activeBullets < MAX_BULLETS) {
      BULLETS[p->activeBullets] = bullet;
      p->activeBullets++;
    } else {
      p->activeBullets = 0;
      BULLETS[p->activeBullets] = bullet;
      p->activeBullets++;
    }
    p->lastBulletTime = time;
  }
  
}

// Bullet movement at each frame
void moveBullets() {
  for (int i = 0; i < MAX_BULLETS; i++) {
    BULLETS[i].yPos -= 8;
  }
}
