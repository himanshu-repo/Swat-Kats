#include "vector2f.h"
#include "ioManager.h"
#include "aaline.h"

class Fuel {
public:
  Fuel();
  Fuel(int sx, int sy, int tl, int cl, 
         float t, int inc, Uint32 c, float sp);
  void draw() const;
  void update(Uint32);
  void reset() { currentLength = totalLength; }
  int getcurrentLength() const{ return currentLength; }
private:
  SDL_Surface* screen;
  Vector2f start;
  int totalLength;
  int currentLength;
  int thick;
  int increments;
  float interval;
  int deltaTime;
  const Uint32 RED;
  const Uint32 GRAY;
  const Uint32 BLACK;
  const Uint32 color;
  void drawBox() const;
  Fuel(const Fuel&);
  Fuel& operator=(const Fuel&);
};
