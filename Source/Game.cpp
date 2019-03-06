#include <iostream>
#include <string>

#include <Engine/DebugPrinter.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Keys.h>
#include <Engine/Sprite.h>

#include "Game.h"
#include "Utility/Rect.h"

/**
 *   @brief   Default Constructor.
 *   @details Consider setting the game's width and height
 *            and even seeding the random number generator.
 */
SpaceInvadersGame::SpaceInvadersGame()
{
  game_name = "Space Invaders: Gotta Pwn Them All";
}

/**
 *   @brief   Destructor.
 *   @details Remove any non-managed memory and callbacks.
 */
SpaceInvadersGame::~SpaceInvadersGame()
{
  this->inputs->unregisterCallback(static_cast<unsigned int>(key_callback_id));
  this->inputs->unregisterCallback(
    static_cast<unsigned int>(mouse_callback_id));
}

/**
 *   @brief   Initialises the game.
 *   @details The game window is created and all assets required to
 *            run the game are loaded. The keyHandler and clickHandler
 *            callback should also be set in the initialise function.
 *   @return  True if the game initialised correctly.
 */
bool SpaceInvadersGame::init()
{
  setupResolution();
  if (!initAPI())
  {
    return false;
  }

  renderer->setClearColour(ASGE::COLOURS::BLACK);

  toggleFPS();

  // input handling functions
  inputs->use_threads = false;

  key_callback_id =
    inputs->addCallbackFnc(ASGE::E_KEY, &SpaceInvadersGame::keyHandler, this);

  mouse_callback_id = inputs->addCallbackFnc(
    ASGE::E_MOUSE_CLICK, &SpaceInvadersGame::clickHandler, this);

  // GameObject Setup
  if (player.addSpriteComponent(renderer.get(),
                                "images/playerShip1_orange.png"))
  {
    player.spriteComponent()->getSprite()->xPos(float(game_width) / 2 - 50);
    player.spriteComponent()->getSprite()->yPos(float(game_height) - 100);
    player.speed(200.0f);
  }
  else
  {
    std::cout << "Player Sprite not set" << std::endl;
  }

  // Ship Setup
  for (int i = 0; i < NUM_OF_SHIPS; i++)
  {
    std::string file;
    if (i < COLUMNS)
    {
      file = "images/enemyBlack1.png";
    }
    else if (i < COLUMNS * 2)
    {
      file = "images/enemyBlue1.png";
    }
    else if (i < COLUMNS * 3)
    {
      file = "images/enemyGreen1.png";
    }
    else if (i < COLUMNS * 4)
    {
      file = "images/enemyRed1.png";
    }
    else
    {
      file = "images/enemyBlack1.png";
    }

    if (ships[i].addSpriteComponent(renderer.get(), file))
    {
      ships[i].spriteComponent()->getSprite()->height(50);
      ships[i].spriteComponent()->getSprite()->width(50);
      ships[i].spriteComponent()->getSprite()->xPos(float(i % COLUMNS) * 60 +
                                                    20);
      ships[i].spriteComponent()->getSprite()->yPos(float(i % 5) * 70 + 20);
      ships[i].speed(50);
    }
  }

  // Setup Shots
  for (int i = 0; i < NUM_OF_SHOTS; i++)
  {
    shots[i].addSpriteComponent(renderer.get(), "images/laserBlue03.png");
    shots[i].spriteComponent()->getSprite()->xPos(0);
    shots[i].spriteComponent()->getSprite()->yPos(0);
    shots[i].speed(200);
    shots[i].visible(false);
    vector2 dir = vector2(0, 1);
    dir.normalise();
    shots[i].direction(dir.x, dir.y);
  }

  return true;
}

/**
 *   @brief   Sets the game window resolution
 *   @details This function is designed to create the window size, any
 *            aspect ratio scaling factors and safe zones to ensure the
 *            game frames when resolutions are changed in size.
 *   @return  void
 */
void SpaceInvadersGame::setupResolution()
{
  // how will you calculate the game's resolution?
  // will it scale correctly in full screen? what AR will you use?
  // how will the game be framed in native 16:9 resolutions?
  // here are some arbitrary values for you to adjust as you see fit
  // https://www.gamasutra.com/blogs/KenanBolukbasi/20171002/306822/Scaling_and_MultiResolution_in_2D_Games.php
  game_width = 640;
  game_height = 920;
}

/**
 *   @brief   Processes any key inputs
 *   @details This function is added as a callback to handle the game's
 *            keyboard input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as
 *            you see fit.
 *   @param   data The event data relating to key input.
 *   @see     KeyEvent
 *   @return  void
 */
void SpaceInvadersGame::keyHandler(const ASGE::SharedEventData data)
{
  auto key = static_cast<const ASGE::KeyEvent*>(data.get());

  if (key->key == ASGE::KEYS::KEY_ESCAPE)
  {
    signalExit();
  }

  if (key->key == ASGE::KEYS::KEY_ENTER)
  {
    in_menu = false;
  }

  if (!in_menu && !game_over && !game_won && key->key == ASGE::KEYS::KEY_A)
  {
    if (key->action == ASGE::KEYS::KEY_RELEASED)
    {
      player.direction(0, 0);
    }
    else
    {
      player.direction(-1, 0);
    }
  }

  else if (!in_menu && !game_over && !game_won && key->key == ASGE::KEYS::KEY_D)
  {
    if (key->action == ASGE::KEYS::KEY_RELEASED)
    {
      player.direction(0, 0);
    }
    else
    {
      player.direction(1, 0);
    }
  }

  else if (!in_menu && !game_over && !game_won &&
           key->key == ASGE::KEYS::KEY_SPACE &&
           key->action == ASGE::KEYS::KEY_PRESSED)
  {
    for (int i = 0; i < NUM_OF_SHOTS; i++)
    {
      if (!shots[i].visible())
      {
        shots[i].visible(true);
        shots[i].spriteComponent()->getSprite()->xPos(
          player.spriteComponent()->getSprite()->xPos() +
          (player.spriteComponent()->getSprite()->width() / 2));
        shots[i].spriteComponent()->getSprite()->yPos(
          player.spriteComponent()->getSprite()->yPos() - 10);
        break;
      }
    }
  }
}

/**
 *   @brief   Processes any click inputs
 *   @details This function is added as a callback to handle the game's
 *            mouse button input. For this game, calls to this function
 *            are thread safe, so you may alter the game's state as you
 *            see fit.
 *   @param   data The event data relating to key input.
 *   @see     ClickEvent
 *   @return  void
 */
void SpaceInvadersGame::clickHandler(const ASGE::SharedEventData data)
{
  auto click = static_cast<const ASGE::ClickEvent*>(data.get());

  double x_pos = click->xpos;
  double y_pos = click->ypos;

  ASGE::DebugPrinter{} << "x_pos: " << x_pos << std::endl;
  ASGE::DebugPrinter{} << "y_pos: " << y_pos << std::endl;
}

/**
 *   @brief   Updates the scene
 *   @details Prepares the renderer subsystem before drawing the
 *            current frame. Once the current frame is has finished
 *            the buffers are swapped accordingly and the image shown.
 *   @return  void
 */
void SpaceInvadersGame::update(const ASGE::GameTime& game_time)
{
  // auto dt_sec = game_time.delta.count() / 1000.0;;
  // make sure you use delta time in any movement calculations!

  if (!in_menu && !game_over && !game_won)
  {
    // Move Player
    float new_x = player.spriteComponent()->getSprite()->xPos();
    if (player.direction().x == -1 &&
        player.spriteComponent()->getSprite()->xPos() > 0)
    {
      new_x =
        new_x - float(player.speed() * (game_time.delta.count() / 1000.f));
    }
    else if (player.direction().x == 1 &&
             player.spriteComponent()->getSprite()->xPos() <
               float(game_width - 100))
    {
      new_x =
        new_x + float(player.speed() * (game_time.delta.count() / 1000.f));
    }
    player.spriteComponent()->getSprite()->xPos(new_x);

    float prev_dir = enemy_direction;
    // Move Enemies
    for (int i = 0; i < NUM_OF_SHIPS; i++)
    {
      float enemey_x = ships[i].spriteComponent()->getSprite()->xPos();

      if (enemy_direction > 0)
      {
        enemey_x = enemey_x +
                   float(ships[i].speed() * (game_time.delta.count() / 1000.f));
      }
      else
      {
        enemey_x = enemey_x -
                   float(ships[i].speed() * (game_time.delta.count() / 1000.f));
      }

      if (ships[COLUMNS - 1].spriteComponent()->getSprite()->xPos() + 40 >
            float(game_width) - 20 &&
          prev_dir == enemy_direction)
      {
        enemy_direction = -1;
      }
      else if (ships[0].spriteComponent()->getSprite()->xPos() < 20 &&
               prev_dir == enemy_direction)
      {
        enemy_direction = 1;
      }

      ships[i].spriteComponent()->getSprite()->xPos(enemey_x);
    }

    // Update enemy y position
    if (prev_dir != enemy_direction)
    {
      for (int i = 0; i < NUM_OF_SHIPS; i++)
      {
        ships[i].spriteComponent()->getSprite()->yPos(
          ships[i].spriteComponent()->getSprite()->yPos() + 10);
      }
    }

    // Enemy Collision
    for (int i = 0; i < NUM_OF_SHIPS; i++)
    {
      if (ships[i].spriteComponent()->getBoundingBox().isInside(
            player.spriteComponent()->getBoundingBox()))
      {
        game_over = true;
      }
    }

    game_won = true;
    for (int i = 0; i < NUM_OF_SHIPS; i++)
    {
      if (ships[i].visible())
      {
        game_won = false;
        break;
      }
    }

    // Shot Collision Detection
    for (int i = 0; i < NUM_OF_SHOTS; i++)
    {
      for (int j = 0; j < NUM_OF_SHIPS; j++)
      {
        if (shots[i].spriteComponent()->getBoundingBox().isInside(
              ships[j].spriteComponent()->getBoundingBox()) &&
            shots[i].visible() && ships[j].visible())
        {
          ships[j].visible(false);
          score += 5;
        }
      }

      if (shots[i].spriteComponent()->getSprite()->yPos() < 0)
      {
        shots[i].visible(false);
      }
    }

    // Update Shots
    for (int i = 0; i < NUM_OF_SHOTS; i++)
    {
      if (shots[i].visible())
      {
        float current_y = shots[i].spriteComponent()->getSprite()->yPos();
        current_y -= float(shots[i].direction().y * shots[i].speed() *
                           (game_time.delta.count() / 1000.f));
        shots[i].spriteComponent()->getSprite()->yPos(current_y);
      }
    }
  }
}

/**
 *   @brief   Renders the scene
 *   @details Renders all the game objects to the current frame.
 *            Once the current frame is has finished the buffers are
 *            swapped accordingly and the image shown.
 *   @return  void
 */
void SpaceInvadersGame::render(const ASGE::GameTime&)
{
  renderer->setFont(0);

  if (in_menu)
  {
    renderer->renderText("Press ENTER to start the game", 180, 460);
  }
  else
  {
    renderer->renderSprite(*player.spriteComponent()->getSprite());

    for (int i = 0; i < NUM_OF_SHIPS; i++)
    {
      if (ships[i].visible())
      {
        renderer->renderSprite(*ships[i].spriteComponent()->getSprite());
      }
    }

    for (int i = 0; i < NUM_OF_SHOTS; i++)
    {
      if (shots[i].visible())
      {
        renderer->renderSprite(*shots[i].spriteComponent()->getSprite());
      }
    }

    std::string score_txt = "Score: ";
    score_txt += std::to_string(score);
    renderer->renderText(score_txt, 525, 50, 1, ASGE::COLOURS::WHITE);

    if (game_won)
    {
      renderer->renderText("Congratulations!", 230, 450);
      renderer->renderText(
        "You have saved the human race from destruction!", 70, 470);
    }
    else if (game_over)
    {
      renderer->renderText("You've Lost...", 235, 440);
      renderer->renderText(
        "You have single handily managed to cause the", 80, 460);
      renderer->renderText(
        "desolation of human kind due to your incompetence", 50, 480);
    }
  }
}
