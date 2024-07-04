#include "include/raylib.h"
#include <stdio.h>
#include <time.h>

#include "constants.h"

#include "enemies.c"
#include "player.c"


/// @brief Handles the keyboard inputs 
/// @param p player character in the game
/// @param time diplayed time in the game
void keyboardInputs(struct Player *p, int time) {
  if (IsKeyDown(KEY_RIGHT)) {
    moveRight(p);
  }
  if (IsKeyDown(KEY_LEFT)) {
    moveLeft(p);
  }
  if (IsKeyPressed(KEY_SPACE)){
    shootBullet(p, time);
  }
}


/// @brief Checks for collision between player character and borders
/// @param p player character
void playerBorderCollision(struct Player *p) {
  if (p->yPos < 0) {
    p->yPos = 1;
  }
  if (p->yPos > HEIGHT - CHARACTER_H) {
    p->yPos = HEIGHT - CHARACTER_H - 1;
  }
  if (p->xPos < 0) {
    p->xPos = 1;
  }
  if (p->xPos > WIDTH - CHARACTER_W) {
    p->xPos = WIDTH - CHARACTER_W - 1;
  }
}


/// @brief Checks for collisions between player bullets and enemies
/// @return true is collision
bool bulletCollision() {
  Vector2 coords;
  Rectangle rec;
  rec.height = ENEMY_H;
  rec.width = ENEMY_W;
  for (int i = 0; i < ENEMY_CAP; i++) {
    if (ENEMIES[i].alive) {
      rec.x = ENEMIES[i].xPos;
      rec.y = ENEMIES[i].yPos;
      for (int j = 0; j < MAX_BULLETS; j++) {
        coords = (Vector2){BULLETS[j].xPos, BULLETS[j].yPos};
        if (CheckCollisionCircleRec(coords, BULLET_RADIUS, rec) && BULLETS[j].isAlive) {
          ENEMIES[i].alive = false;
          BULLETS[j].isAlive = false;
          return true;
        }
      }
    }
  }
  return false;
}


/// @brief Checks for collisions between enemy beam and player character
/// @param p player character
void beamCollision(struct Player *p){
  Vector2 coords;
  Rectangle rec;
  rec.height = ENEMY_H;
  rec.width = ENEMY_W;

  rec.height = CHARACTER_H;
  rec.width = CHARACTER_W;
  rec.x = p->xPos;
  rec.y = p->yPos;

  Vector2 coordsBeam;
  Rectangle recBeam;
  recBeam.height = BEAM_H;
  recBeam.width = BEAM_W;

  for(int i = 0; i < ENEMY_BEAM_CAP; i++){
    if(BEAMS[i].alive){
      recBeam.x = BEAMS[i].xPos;
      recBeam.y = BEAMS[i].yPos;
      if(CheckCollisionRecs(rec, recBeam)){
        BEAMS[i].alive = false;
        LIVES[p->lives - 1] = false;
        p->lives--;
      }
    }
  }
}


/// @brief Draws the bullets if alive
void renderBullets() {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (BULLETS[i].isAlive) {
      DrawCircle(BULLETS[i].xPos, BULLETS[i].yPos, 5.0f, GREEN);
    }
  }
  moveBullets();
}


/// @brief Draws the enemies if they are alive
/// @param enemyTexture Enemy 2d texture (image)
void renderEnemies(Texture2D enemyTexture) {
  Rectangle sourceRect = {0, 0, ENEMY_W, ENEMY_H};
  Vector2 enemyPos = {0, 0};
  for (int i = 0; i < ENEMY_CAP; i++) {
    enemyPos.x = ENEMIES[i].xPos;
    enemyPos.y = ENEMIES[i].yPos;
    if (ENEMIES[i].alive) {
      DrawTextureRec(enemyTexture, sourceRect, enemyPos, WHITE);
    }
  }
}


/// @brief Refills the ENEMIES array when all are dead
void repopulateEnemies() {
  for (int i = 0; i < ENEMY_CAP; i++) {
    ENEMIES[i].alive = true;
  }
}


/// @brief Draws enemy beams if they are alive
/// @param time Displayed time on screen
/// @param p Player character
void renderBeams(int time, struct Player *p){
  for(int i = 0; i < ENEMY_CAP; i ++){
    if((p->xPos + CHARACTER_W / 2) - (ENEMIES[i].xPos + ENEMY_W / 2)  < 45 && 
       (p->xPos + CHARACTER_W / 2) - (ENEMIES[i].xPos + ENEMY_W / 2)  > -45){
      if(shootLaser(ENEMIES[i], time)){
        ENEMIES[i].lastBeam = time;
      }
    }
  }
  for(int i = 0; i < ENEMY_BEAM_CAP; i++){
    if(BEAMS[i].alive){
      DrawRectangle(BEAMS[i].xPos, BEAMS[i].yPos, BEAM_W, BEAM_H, RED);
    }
  }
  moveLasers();
}


/// @brief Draws the number of lives (heart) remaining
/// @param p Player character
/// @param heart Heart 2D texture (image)
void renderLives(struct Player *p, Texture2D heart){
  Rectangle rec;
  rec.height = 40;
  rec.width = 40;
  rec.x = 0;
  rec.y = 0;
  
  Vector2 coords;

  for(int i = 0; i < 3; i++){
    if(LIVES[i]){
      coords.x = WIDTH / 2 - LIVES_CONTAINER_WIDTH / 2 + 50*i;
      coords.y = LIVES_POSITION;
      DrawTextureRec(heart, rec, coords, WHITE);
    }
  }
}


/// @brief Checks if the game is still alive (player lives != 0)
/// @param p Player character
/// @param timeOfEnd Time at wich the game ends
/// @return true if the game is finished 
bool checkForGameEnd(struct Player *p, float *timeOfEnd){
  // Only want to set the *timeOfEnd to the time of death once. 
  if(!(p->playerAlive)){
    return p->playerAlive; 
  } else if (p->lives == 0){  // Check if lives reach 0
    // First time it reaches 0 we set the timeOfEnd, after that we return
    // the previous if statement, player.playerAlive will be false
    *timeOfEnd = GetTime();
    p->playerAlive = false;
    for(int i = 0; i < ENEMY_BEAM_CAP; i++){
      BEAMS[i].alive = false;
      BULLETS[i].isAlive = false;
    }
  }
  return p->playerAlive;
}


/// @brief Creates a new game, initialises needed variables and states
/// @param p Player character
/// @param score Game score
/// @param initTime Time of initialisation (used for displayed time)
/// @param currTime Current time
/// @param displayTime Displayed time
/// @param enemyAlive Number of alive enemies (set to max)
void newGame(struct Player *p, int *score, float *initTime, float *currTime, int displayTime, int enemyAlive){
  initPlayer(p);
  *score = 0;
  *initTime = GetTime();
  *currTime = 0.0f;
  displayTime = 0;
  enemyAlive = ENEMY_CAP;
  populateEnemyArray();
}



int main(void) {
  InitWindow(WIDTH, HEIGHT, "The Game");
  srand(time(NULL));

  // Background image and texture
  Image background = LoadImage("ressources/back_2.png");
  Texture2D backgroundTexture = LoadTextureFromImage(background);
  UnloadImage(background); // Free the image now that texture is set

  // Character image and texture
  Image characterImage = LoadImage("ressources/spaceship_sheet_small.png");
  Texture2D character = LoadTextureFromImage(characterImage);
  UnloadImage(characterImage);

  // Enemy image and texture
  Image enemyImage = LoadImage("ressources/small_alien.png");
  Texture2D enemy = LoadTextureFromImage(enemyImage);
  UnloadImage(enemyImage);

  // Life image and texture (heart)
  Image lifeImage = LoadImage("ressources/heart2.png");
  Texture2D heart = LoadTextureFromImage(lifeImage);
  UnloadImage(lifeImage);

  // Initialisation of player character
  struct Player player1;

  // Keep track of the score and enemies
  int score = 0;
  int enemiesAlive = ENEMY_CAP; 

  // Spaceship sheet has 4 orientations, each 1/4 of image width
  // see ressources/spaceship_sheet_small.png.
  // We chose only the facing up orientation.
  float updatedCharW = character.width * 0.25f;
  float updatedCharH = character.height * 1.0f;

  // Time variables for display and game state tracking
  float initialTime = GetTime();
  float currTime = 0.0f;
  int displayTime = 0;
  float timeOfEnd = 0.0f;

  newGame(&player1, &score, &initialTime, &currTime, displayTime, enemiesAlive);

  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    currTime = GetTime();
    displayTime = (int)( currTime - initialTime);

    keyboardInputs(&player1, displayTime); // Check for inputs
    playerBorderCollision(&player1);

    if (enemiesAlive == 0) {
      repopulateEnemies();
      enemiesAlive = ENEMY_CAP;
    }
    if (bulletCollision()) {
      score++;
      enemiesAlive--;
    }
    beamCollision(&player1);

    BeginDrawing();
    
      if(checkForGameEnd(&player1, &timeOfEnd)){
        ClearBackground(DARKPURPLE);
        DrawTexture(backgroundTexture, 0, 0, WHITE);
        Rectangle sourceRect = {0, 0, updatedCharW, updatedCharH};
        Vector2 playerPosition = {player1.xPos, player1.yPos};

        DrawTextureRec(character, sourceRect, playerPosition, WHITE);

        DrawText(TextFormat("Score : %d", score), 15, 760, 20, RED);
        DrawText(TextFormat("%d", displayTime), 950, 760, 20, WHITE);

        renderLives(&player1, heart);
        renderBeams(displayTime, &player1);
        renderEnemies(enemy);
        renderBullets();
      } else if(!checkForGameEnd(&player1, &timeOfEnd)) {
        ClearBackground(DARKPURPLE);
        DrawTexture(backgroundTexture, 0, 0, WHITE);
        DrawText(TextFormat("GAME OVER"), WIDTH / 2 - 170, HEIGHT / 2 - 30, 60, RED);
        DrawText(TextFormat("Score : %d", score), 15, 760, 20, RED);
        if(currTime - timeOfEnd > GAME_END_TIMER){
          newGame(&player1, &score, &initialTime, &currTime, displayTime, enemiesAlive);
        }
      }
    EndDrawing();
    
  }
  UnloadTexture(character);
  UnloadTexture(backgroundTexture);
  UnloadTexture(enemy);
  UnloadTexture(heart);
  CloseWindow();

  return 0;
}