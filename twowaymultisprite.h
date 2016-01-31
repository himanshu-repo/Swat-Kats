#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H

#include <string>
#include <iostream>
#include <vector>
using std::string;

#include "drawable.h"

class TwoWayMultiSprite : public Drawable {
public:
  TwoWayMultiSprite(const string&);

  TwoWayMultiSprite(const std::string&, const Vector2f& pos, const Vector2f& vel,
              const std::vector<Frame*>& fmsLeft,const std::vector<Frame*>& fmsRight);

  TwoWayMultiSprite(const TwoWayMultiSprite&);
  virtual ~TwoWayMultiSprite() { } 

  
  
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }

    
protected:
  const std::vector<Frame *> framesLeft;
  const std::vector<Frame *> framesRight;
  std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  float dt;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  void advanceFrame(Uint32 ticks);
};
#endif
