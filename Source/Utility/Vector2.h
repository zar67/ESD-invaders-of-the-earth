#pragma once
struct vector2
{
  // construction
  vector2(float x_, float y_);
  vector2(const vector2& rhs);

  // operations
  vector2 operator*(float scalar);
  vector2& operator=(const vector2& rhs);
  void normalise();

  // data
  float x = 0;
  float y = 0;
};
