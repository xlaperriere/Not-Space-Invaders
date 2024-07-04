#include <stdbool.h>

#include "constants.h"

/// @brief Player struct to keep track of the state of the character
struct Player {
  double xPos;
  double yPos;
  int activeBullets;
  int lives;
  bool playerAlive;
  int lastBulletTime;
};

/// @brief Bullet struct to keep track of the fired bullets
struct Bullet {
  double xPos;
  double yPos;
  bool isAlive;
};

// Static array of bullets to keep track
struct Bullet BULLETS[MAX_BULLETS];

// Character's three lives
bool LIVES[3];


/// @brief Initilizes a player character with specific values
/// @param p Player to initialize
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


/// @brief Move the player characer right
/// @param p Player character
void moveRight(struct Player *p) {
  p->xPos += 6;
}


/// @brief Move the player characer left
/// @param p Player character
void moveLeft(struct Player *p) {
  p->xPos -= 6;
}


// #######################################
//            Bullet functions
// #######################################


/// @brief Shoots a bullet from player characer position (if possible)
/// @param p Player character
/// @param time Time at wich the bullet is fired
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


/// @brief Move the bullets from the BULLETS array if they are alive
void moveBullets() {
  for (int i = 0; i < MAX_BULLETS; i++) {
    BULLETS[i].yPos -= 8;
  }
}
