#include "GameObject.h"
#include <Engine/Renderer.h>

GameObject::~GameObject()
{
  free();
}

bool GameObject::addSpriteComponent(ASGE::Renderer* renderer,
                                    const std::string& texture_file_name)
{
  free();

  sprite_component = new SpriteComponent();
  if (sprite_component->loadSprite(renderer, texture_file_name))
  {
    return true;
  }

  free();
  return false;
}

void GameObject::free()
{
  delete sprite_component;
  sprite_component = nullptr;
}

SpriteComponent* GameObject::spriteComponent()
{
  return sprite_component;
}

vector2 GameObject::direction()
{
  return velocity;
}

void GameObject::direction(float new_x, float new_y)
{
  velocity.x = new_x;
  velocity.y = new_y;
}

float GameObject::getSpeed()
{
  return speed;
}

void GameObject::setSpeed(float new_speed)
{
  speed = new_speed;
}

bool GameObject::visible()
{
  return visibility;
}

void GameObject::visible(bool shown)
{
  visibility = shown;
}
