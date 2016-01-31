#ifndef RAIN__H
#define RAIN__H

#include <string>
#include <iostream>
#include "drawable.h"

class Rain : public Drawable {
public:
  Rain(const std::string&);
  Rain(const Rain& s);
  virtual ~Rain() { } 
  Rain& operator=(const Rain&);

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;

  virtual void update(Uint32 ticks);

private:
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  int getDistance(const Rain*) const;
};
#endif
