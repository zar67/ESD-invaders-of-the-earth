//
// Created by z2-rowbotham on 13/03/2019.
//

#include "GameObjectController.h"
#include <iostream>

GameObjectController::GameObjectController(const int width, const int height)
{
  game_width = static_cast<float>(width);
  game_height = static_cast<float>(height);
}

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
    object->spriteComponent()->getSprite()->xPos(new_y);
  }
}
