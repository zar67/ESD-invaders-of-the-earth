//
// Created by z2-rowbotham on 13/03/2019.
//

#include "GameObjectController.h"
#include <math.h>

bool GameObjectController::setupObject(GameObject* object,
                                       ASGE::Renderer* renderer,
                                       const std::string& texture_file_name,
                                       float pos_x,
                                       float pos_y,
                                       float dir_x,
                                       float dir_y,
                                       float start_speed,
                                       float size_x,
                                       float size_y,
                                       bool start_shown)
{
  if (object->addSpriteComponent(renderer, texture_file_name))
  {
    object->spriteComponent()->getSprite()->xPos(pos_x);
    object->spriteComponent()->getSprite()->yPos(pos_y);

    object->spriteComponent()->getSprite()->width(size_x);
    object->spriteComponent()->getSprite()->height(size_y);

    object->direction(dir_x, dir_y);
    object->setSpeed(start_speed);
    object->visible(start_shown);
    return true;
  }
  return false;
}

void GameObjectController::moveObject(GameObject* object, double delta_time)
{
  if (object->direction().x != 0 &&
      ((object->direction().x < 0 &&
        object->spriteComponent()->getSprite()->xPos() > 0) ||
       (object->direction().x > 0 &&
        object->spriteComponent()->getSprite()->xPos() <
          game_width - object->spriteComponent()->getSprite()->width())))
  {
    float new_x = object->spriteComponent()->getSprite()->xPos();
    new_x += static_cast<float>(object->getSpeed() * object->direction().x *
                                delta_time);
    object->spriteComponent()->getSprite()->xPos(new_x);
  }

  if (object->direction().y != 0 &&
      ((object->direction().y < 0 &&
        object->spriteComponent()->getSprite()->yPos() > 0) ||
       (object->direction().y > 0 &&
        object->spriteComponent()->getSprite()->yPos() <
          game_height - object->spriteComponent()->getSprite()->height())))
  {
    float new_y = object->spriteComponent()->getSprite()->yPos();
    new_y += static_cast<float>(object->getSpeed() * object->direction().y *
                                delta_time);
    object->spriteComponent()->getSprite()->yPos(new_y);
  }
}

void GameObjectController::applyGravity(GameObject* object, double delta_time)
{
  float new_y = object->spriteComponent()->getSprite()->yPos();
  new_y += static_cast<float>(gravity * delta_time);
  object->spriteComponent()->getSprite()->yPos(new_y);
}

void GameObjectController::applyQuadraticTrajectory(GameObject* object,
                                                    double delta_time,
                                                    int ship_row)
{
  // y = (1/10)(x-centre)^2
  float new_y =
    object->spriteComponent()->getSprite()->xPos() - (game_width / 2);
  new_y *= new_y;
  new_y /= 500;
  new_y += static_cast<float>(ship_row * 70);
  object->spriteComponent()->getSprite()->yPos(new_y);
}

void GameObjectController::applySinTrajectory(GameObject* object,
                                              double delta_time,
                                              int ship_row)
{
  // y = 8sin(x/4) + 8
  float new_y = sin(object->spriteComponent()->getSprite()->xPos() / 4);
  new_y *= 8;
  new_y += 8;
  new_y += static_cast<float>(ship_row * 70);
  object->spriteComponent()->getSprite()->yPos(new_y);
}

void GameObjectController::gameWidth(float width)
{
  game_width = width;
}

void GameObjectController::gameHeight(float height)
{
  game_height = height;
}