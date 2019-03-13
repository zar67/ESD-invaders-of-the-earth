//
// Created by z2-rowbotham on 13/03/2019.
//

#ifndef SPACEINVADERS_GAMEOBJECTCONTROLLER_H
#define SPACEINVADERS_GAMEOBJECTCONTROLLER_H

#include "Components/GameObject.h"

class GameObjectController
{
 public:
  GameObjectController(const int width, const int height);
  ~GameObjectController() = default;

  bool setupObject(GameObject* object,
                   ASGE::Renderer* renderer,
                   const std::string& texture_file_name,
                   float pos_x,
                   float pos_y,
                   float dir_x,
                   float dir_y,
                   float start_speed,
                   float size_x,
                   float size_y,
                   bool start_shown);

  void moveObject(GameObject* object, double delta_time);

 private:
  float game_width = 0;
  float game_height = 0;
};

#endif // SPACEINVADERS_GAMEOBJECTCONTROLLER_H
