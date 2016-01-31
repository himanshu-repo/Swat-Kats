#ifndef PLAYER__H
#define PLAYER__H

#include <string>
#include <iostream>
#include <vector>
#include "collisionStrategy.h"

using std::string;

#include "drawable.h"

class Player : public Drawable {
public:
  Player(const string&);

  Player(const std::string&, const Vector2f& pos, const Vector2f& vel,
              const std::vector<Frame*>& fmsLeft,const std::vector<Frame*>& fmsRight);

  Player(const Player&);
  virtual ~Player() { if(strategy!=NULL) delete strategy ; } 

  
  
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[currentFrame]; 
  }

  void goUp();
  void goDown();
  void goRight();
  void goLeft();
  void stopX();
  void stopY();
    
bool collidedWith(const Drawable* obj) const;	
	
protected:
	
  CollisionStrategy * strategy;	

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
  
  Player& operator=(const Player&);
};
#endif
