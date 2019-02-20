#pragma once
#include <Engine/OGLGame.h>
#include <string>

#include "Components/GameObject.h"
#include "Utility/Rect.h"

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

  // Add your GameObjects
  bool in_menu = true;
};