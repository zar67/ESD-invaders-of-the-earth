#pragma once
#include "SpriteComponent.h"
#include "Utility/Vector2.h"
#include <string>

/**
 *  Objects used throughout the game.
 *  Provides a nice solid base class for objects in this game world.
 *  They currently support only sprite components, but this could easily
 *  be extended to include things like rigid bodies or input systems.
 *  @see SpriteComponent
 */
class GameObject
{
 public:
  /**
   *  Default constructor.
   */
  GameObject() = default;

  /**
   *  Destructor. Frees dynamic memory.
   */
  ~GameObject();

  /**
   *  Allocates and attaches a sprite component to the object.
   *  Part of this process will attempt to load a texture file.
   *  If this fails this function will return false and the memory
   *  allocated, freed.
   *  @param [in] renderer The renderer used to perform the allocations
   *  @param [in] texture_file_name The file path to the the texture to load
   *  @return true if the component is successfully added
   */
  bool addSpriteComponent(ASGE::Renderer* renderer,
                          const std::string& texture_file_name);

  /**
   *  Returns the sprite componenent.
   *  IT IS HIGHLY RECOMMENDED THAT YOU CHECK THE STATUS OF THE POINTER
   *  IS IS VALID FOR A COMPONENT TO BE NULL AS THEY ARE OPTIONAL!
   *  @return a pointer to the objects sprite component (if any)
   */
  SpriteComponent* spriteComponent();

 private:
  void free();
  SpriteComponent* sprite_component = nullptr;
};