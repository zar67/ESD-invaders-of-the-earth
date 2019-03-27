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

bool SpaceInvadersGame::setupObjects()
{
  // Player Setup
  float player_x = static_cast<float>(game_width) / 2 - 50;
  float player_y = static_cast<float>(game_height) - 100;
  if (!controller.setupObject(&player,
                              renderer.get(),
                              "images/playerShip1_orange.png",
                              player_x,
                              player_y,
                              0,
                              0,
                              200.0f,
                              99,
                              75,
                              true))
  {
    std::cout << "Player NOT setup correctly" << std::endl;
    return false;
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

    if (!controller.setupObject(&ships[i],
                                renderer.get(),
                                file,
                                static_cast<float>(i % COLUMNS) * 60 + 20,
                                static_cast<float>(i % ROWS) * 70 + 20,
                                1,
                                0,
                                50,
                                50,
                                50,
                                true))
    {
      std::cout << "Ship " << i << " NOT setup correctly" << std::endl;
      return false;
    }
  }

  // Setup Player Shots
  for (int i = 0; i < NUM_OF_SHOTS; i++)
  {
    if (!controller.setupObject(&player_shots[i],
                                renderer.get(),
                                "images/laserBlue03.png",
                                0,
                                0,
                                0,
                                -1,
                                200,
                                10,
                                20,
                                false))
    {
      std::cout << "Player Shot " << i << " NOT setup correctly" << std::endl;
      return false;
    }
  }

  // Setup Enemy Shots
  for (int i = 0; i < NUM_OF_SHOTS; i++)
  {
    if (!controller.setupObject(&enemy_shots[i],
                                renderer.get(),
                                "images/laserRed03.png",
                                0,
                                0,
                                0,
                                1,
                                200,
                                10,
                                20,
                                false))
    {
      std::cout << "Enemy Shot " << i << " NOT setup correctly" << std::endl;
      return false;
    }
  }
  return true;
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
  std::srand(static_cast<unsigned int>(time(nullptr)));
  setupResolution();
  controller.gameHeight(static_cast<float>(game_height));
  controller.gameWidth(static_cast<float>(game_width));
  if (!initAPI())
  {
    return false;
  }

  renderer->setClearColour(ASGE::COLOURS::BLACK);

  toggleFPS();

  // Input handling functions
  inputs->use_threads = false;

  key_callback_id =
    inputs->addCallbackFnc(ASGE::E_KEY, &SpaceInvadersGame::keyHandler, this);

  mouse_callback_id = inputs->addCallbackFnc(
    ASGE::E_MOUSE_CLICK, &SpaceInvadersGame::clickHandler, this);

  return setupObjects();
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
      if (!player_shots[i].visible())
      {
        player_shots[i].visible(true);
        player_shots[i].spriteComponent()->getSprite()->xPos(
          player.spriteComponent()->getSprite()->xPos() +
          (player.spriteComponent()->getSprite()->width() / 2));
        player_shots[i].spriteComponent()->getSprite()->yPos(
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

void SpaceInvadersGame::updateGameStates()
{
  game_won = true;
  for (int i = 0; i < NUM_OF_SHIPS; i++)
  {
    if (ships[i].visible())
    {
      game_won = false;
      break;
    }
  }

  for (int i = 0; i < NUM_OF_SHIPS; i++)
  {
    if (ships[i].spriteComponent()->getBoundingBox().isInside(
          player.spriteComponent()->getBoundingBox()))
    {
      game_over = true;
    }
  }
}

void SpaceInvadersGame::moveObjects(double delta_time)
{
  controller.moveObject(&player, delta_time);

  // Move Enemies
  float enemy_direction = ships[0].direction().x;
  float prev_dir = enemy_direction;

  if (ships[COLUMNS - 1].spriteComponent()->getSprite()->xPos() + 40 >
        static_cast<float>(game_width) - 20 &&
      prev_dir == enemy_direction)
  {
    enemy_direction = -1;
  }
  else if (ships[0].spriteComponent()->getSprite()->xPos() < 20 &&
           prev_dir == enemy_direction)
  {
    enemy_direction = 1;
  }

  /*
  if (prev_dir != enemy_direction)
  {
    for (int i = 0; i < NUM_OF_SHIPS; i++)
    {
      ships[i].direction(enemy_direction, 10);
    }
  }
   */

  for (int i = 0; i < NUM_OF_SHIPS; i++)
  {
    controller.moveObject(&ships[i], delta_time);

    ships[i].direction(enemy_direction, 0);

    // controller.applyGravity(&ships[i], delta_time);
    controller.applyQuadraticTrajectory(&ships[i], delta_time, i % ROWS);
  }

  for (int i = 0; i < NUM_OF_SHOTS; i++)
  {
    if (player_shots[i].visible())
    {
      controller.moveObject(&player_shots[i], delta_time);
    }
  }

  for (int i = 0; i < NUM_OF_SHOTS; i++)
  {
    if (enemy_shots[i].visible())
    {
      controller.moveObject(&enemy_shots[i], delta_time);
    }
  }
}

void SpaceInvadersGame::shotCollision()
{
  // Player Shots
  for (int i = 0; i < NUM_OF_SHOTS; i++)
  {
    for (int j = 0; j < NUM_OF_SHIPS; j++)
    {
      if (player_shots[i].spriteComponent()->getBoundingBox().isInside(
            ships[j].spriteComponent()->getBoundingBox()) &&
          player_shots[i].visible() && ships[j].visible())
      {
        ships[j].visible(false);
        player_shots[i].visible(false);
        score += 5;
      }
    }

    if (player_shots[i].spriteComponent()->getSprite()->yPos() < 0)
    {
      player_shots[i].visible(false);
    }
  }

  // Enemy Shots
  for (int i = 0; i < NUM_OF_SHOTS; i++)
  {
    if (enemy_shots[i].spriteComponent()->getSprite()->yPos() >
        static_cast<float>(game_height) -
          enemy_shots[i].spriteComponent()->getSprite()->height())
    {
      enemy_shots[i].visible(false);
    }

    if (enemy_shots[i].spriteComponent()->getBoundingBox().isInside(
          player.spriteComponent()->getBoundingBox()) &&
        enemy_shots[i].visible())
    {
      game_over = true;
    }
  }
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
    updateGameStates();

    moveObjects(game_time.delta.count() / 1000.0f);

    shotCollision();

    // Spawn Enemy Shots
    for (int i = 0; i < NUM_OF_SHOTS; i++)
    {
      int random_enemy = std::rand() % (NUM_OF_SHIPS);

      if (ships[random_enemy].visible() && std::rand() % 50000000 + 1 < 2)
      {
        float new_x =
          ships[random_enemy].spriteComponent()->getSprite()->xPos() +
          ships[random_enemy].spriteComponent()->getSprite()->width() / 2;
        float new_y =
          ships[random_enemy].spriteComponent()->getSprite()->yPos() +
          ships[random_enemy].spriteComponent()->getSprite()->height() + 5;
        enemy_shots[i].spriteComponent()->getSprite()->xPos(new_x);
        enemy_shots[i].spriteComponent()->getSprite()->yPos(new_y);
        enemy_shots[i].setSpeed(200);
        enemy_shots[i].visible(true);
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
      if (player_shots[i].visible())
      {
        renderer->renderSprite(*player_shots[i].spriteComponent()->getSprite());
      }
    }

    for (int i = 0; i < NUM_OF_SHOTS; i++)
    {
      if (enemy_shots[i].visible())
      {
        renderer->renderSprite(*enemy_shots[i].spriteComponent()->getSprite());
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
