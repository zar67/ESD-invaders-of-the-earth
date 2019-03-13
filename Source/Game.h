#pragma once
#include <Engine/OGLGame.h>
#include <string>

#include "Components/GameObjectController.h"
#include "Utility/Rect.h"

const int NUM_OF_SHIPS = 40;
const int COLUMNS = 8;
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

  virtual void update(const ASGE::GameTime&) override;
  virtual void render(const ASGE::GameTime&) override;

  int key_callback_id = -1;   /**< Key Input Callback ID. */
  int mouse_callback_id = -1; /**< Mouse Input Callback ID. */

  GameObjectController controller =
    GameObjectController(game_width, game_height);

  // GameObjects
  GameObject player;
  GameObject ships[NUM_OF_SHIPS];
  GameObject player_shots[NUM_OF_SHOTS];
  GameObject enemy_shots[NUM_OF_SHOTS];

  bool in_menu = true;
  bool game_over = false;
  bool game_won = false;
  float enemy_direction = 1;
  int score = 0;
};