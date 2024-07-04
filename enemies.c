#include <stdbool.h>
#include <stdlib.h>
#include <time.h>


#include "constants.h"

// Enemy struct. Used to keep track of the state of enemies
struct Enemy {
  double xPos;
  double yPos;
  int type;
  bool alive;
  int lastBeam;
};

struct Beam {
  double xPos;
  double yPos;
  bool alive;
};

int activeBeams = 0;

struct Beam BEAMS[ENEMY_BEAM_CAP];

// Static array of enemies to keep track of all of them
struct Enemy ENEMIES[ENEMY_CAP];

// Util function that generates a random int from 0 to max
// Dont know if i will use it yet
int generateRandomInt(int max) {

  int randomNumber = rand() % (max + 1);
  return randomNumber;
}

// Generates random values for type and xPos for a new enemy and returns the
// latter
struct Enemy generateEnemy(int i) {
  struct Enemy newEnemy;

  int slice = i * WIDTH / ENEMY_CAP;
  int offset = (WIDTH / ENEMY_CAP) / 2 - (ENEMY_W / 2);

  newEnemy.xPos = slice + offset;
  newEnemy.yPos = ENEMY_INITIAL_Y;
  newEnemy.alive = true;
  newEnemy.lastBeam = 0;
  return newEnemy;
}

// Populates the static array enemies using the random generation
void populateEnemyArray() {
  for (int i = 0; i < ENEMY_CAP; i++) {
    ENEMIES[i] = generateEnemy(i);
  }
}

// ###########################################
// #             ENEMY ATTACKS               #
// ###########################################

bool canShootLaser(struct Enemy e, int time) {
  if(e.alive && time - e.lastBeam > 1){
    return true;
  }
  return false;
}

bool shootLaser(struct Enemy e, int time){
  //cant shoot?
  if(!canShootLaser(e, time)){ 
    return false; 
  } else {
    struct Beam beam;

    float offsetX = ENEMY_W / 2 - BEAM_W / 2;
    float offsetY = ENEMY_H;

    beam.xPos = e.xPos + offsetX;
    beam.yPos = e.yPos + offsetY;

    if(activeBeams < ENEMY_BEAM_CAP){
      BEAMS[activeBeams] = beam;
      activeBeams++;
    } else {
      activeBeams = 0;
      BEAMS[activeBeams] = beam;
      activeBeams++;
    }
    return true;
  }
}

void moveLasers(){
  for(int i = 0; i < ENEMY_BEAM_CAP; i++){
    
    BEAMS[i].yPos += 6;
  }
}
