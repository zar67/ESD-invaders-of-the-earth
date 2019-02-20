#include "Vector2.h"
#include <math.h>

/**
 *   @brief   Constructor.
 *   @details Requires values for it's x and y magnitudes.
 *   @return  void
 */
vector2::vector2(float x_, float y_) : x(x_), y(y_) {}

/**
 *   @brief   Copies a vector.
 *   @details Constructs a new vector2 from an existing.
 *   @return  void
 */
vector2::vector2(const vector2& rhs)
{
  this->x = rhs.x;
  this->y = rhs.y;
}

/**
 *   @brief   Copies a vector.
 *   @details Updates vector2 from an existing.
 *   @return  void
 */
vector2& vector2::operator=(const vector2& rhs)
{
  this->x = rhs.x;
  this->y = rhs.y;

  return *this;
}

/**
 *   @brief   Normalises vector.
 *   @details Turns the vector into a unit vector.
 *   @return  void
 */
void vector2::normalise()
{
  float magnitude = sqrtf((x * x) + (y * y));

  if (!magnitude)
    return;

  x /= magnitude;
  y /= magnitude;
}

/**
 *   @brief   Scales the vector.
 *   @details Uses a single scalar value to adjust the vector.
 *   @return  void
 */
vector2 vector2::operator*(float scalar)
{
  vector2 vec(*this);
  vec.x *= scalar;
  vec.y *= scalar;
  return vec;
}
