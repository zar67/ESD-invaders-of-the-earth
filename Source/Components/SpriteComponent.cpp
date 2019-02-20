#include "SpriteComponent.h"
#include <Engine/Renderer.h>

SpriteComponent::~SpriteComponent()
{
  free();
}

bool SpriteComponent::loadSprite(ASGE::Renderer* renderer,
                                 const std::string& texture_file_name)
{
  free();
  sprite = renderer->createRawSprite();
  if (sprite->loadTexture(texture_file_name))
  {
    return true;
  }

  free();
  return false;
}

void SpriteComponent::free()
{
  if (sprite)
  {
    delete sprite;
    sprite = nullptr;
  }
}

ASGE::Sprite* SpriteComponent::getSprite()
{
  return sprite;
}

rect SpriteComponent::getBoundingBox() const
{
  rect bounding_box;
  bounding_box.x = sprite->xPos();
  bounding_box.y = sprite->yPos();
  bounding_box.length = sprite->width();
  bounding_box.height = sprite->height();

  return bounding_box;
}