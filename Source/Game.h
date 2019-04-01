#pragma once
#include <Engine/OGLGame.h>
#include <string>

#include "Components/GameObjectController.h"
#include "Utility/Rect.h"

const int NUM_OF_SHIPS = 40;
const int COLUMNS = 8;
const int ROWS = NUM_OF_SHIPS / COLUMNS;
const int NUM_OF_SHOTS = 10;

/**
 *  An OpenGL Game based on ASGE.
 */
class SpaceInvadersGame : public ASGE::OGLGame
{
 public:
  SpaceInvadersGame();
  ~SpaceInvadersGame();
  virtual bool init() override;

 private:
  void keyHandler(const ASGE::SharedEventData data);
  void clickHandler(const ASGE::SharedEventData data);
  void setupResolution();

  bool setupObjects();
  void updateGameStates();
  void moveObjects(double delta_time);
  void shotCollision();

  void
  normalEnemyMovement(float prev_dir, float enemy_direction, double delta_time);
  void gravityEnemyMovement(double delta_time);
  void quadraticEnemyMovement(double delta_time);
  void sinEnemyMovement(double delta_time);

  virtual void update(const ASGE::GameTime&) override;
  virtual void render(const ASGE::GameTime&) override;

  int key_callback_id = -1;   /**< Key Input Callback ID. */
  int mouse_callback_id = -1; /**< Mouse Input Callback ID. */

  GameObjectController controller;

  // GameObjects
  GameObject player;
  GameObject ships[NUM_OF_SHIPS];
  GameObject player_shots[NUM_OF_SHOTS];
  GameObject enemy_shots[NUM_OF_SHOTS];

  bool in_menu = true;
  bool game_over = false;
  bool game_won = false;
  int score = 0;
  int game_mode = 0;
};