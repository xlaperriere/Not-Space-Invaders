#include <stdbool.h>
#include <stdlib.h>
#include <time.h>


#include "constants.h"

/// @brief Enemy struct to keep track of enemies
struct Enemy {
  double xPos;
  double yPos;
  int type;
  bool alive;
  int lastBeam;
};

/// @brief Beam struct to keep track of fire beams by enemies
struct Beam {
  double xPos;
  double yPos;
  bool alive;
};

// Number of active beams
int activeBeams = 0;

// Static array to store the enemy beams
struct Beam BEAMS[ENEMY_BEAM_CAP];

// Static array of enemies to store and keep track of all of them
struct Enemy ENEMIES[ENEMY_CAP];

// Util function that generates a random int from 0 to max
// Dont know if i will use it yet
int generateRandomInt(int max) {
  int randomNumber = rand() % (max + 1);
  return randomNumber;
}


/// @brief Generate a new enemy at the right position with right values
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


/// @brief Populate the ENEMIES array used on game start and when refilling the array
void populateEnemyArray() {
  for (int i = 0; i < ENEMY_CAP; i++) {
    ENEMIES[i] = generateEnemy(i);
  }
}


// ###########################################
// #             ENEMY ATTACKS               #
// ###########################################


/// @brief Checks if an enemy can shoot a beam
/// @param e The enemy in question
/// @param time Time at wich it would shoot
/// @return True if it can shoot
bool canShootBeam(struct Enemy e, int time) {
  if(e.alive && time - e.lastBeam > 1){
    return true;
  }
  return false;
}


/// @brief Shoots a beam if canShootBeam returns true
/// @param e Enemy shooting the beam
/// @param time Time of the shot
/// @return If a beam was fired
bool shootBeam(struct Enemy e, int time){
  //cant shoot?
  if(!canShootBeam(e, time)){ 
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


/// @brief Moves the beams that are alive
void moveBeams(){
  for(int i = 0; i < ENEMY_BEAM_CAP; i++){
    
    BEAMS[i].yPos += 6;
  }
}
