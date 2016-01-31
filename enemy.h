#ifndef ENEMY__H
#define ENEMY__H

#include <string>
#include <iostream>
#include "drawable.h"
#include "player.h"
#include "ioManager.h"

class Enemy : public Drawable {
public:
  Enemy(const std::string& , const Player& p);
 // Enemy(const Enemy& s);
  virtual ~Enemy() { } 
//  Enemy& operator=(const Enemy&);

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;

  virtual void update(Uint32 ticks);

  
  void goLeft();
 // void goRight();
  void goUp();
  void goDown();
  void stopY();
  
  static float getSafeDistance() { return safeDistance ; }
  
  
private:

  enum MODE{NORMAL, ATTACK} ;
  MODE currentMode ;

  static float safeDistance;
  
  const Player& target ;
  
  IOManager& io;
  
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  int getDistance(const Enemy*) const;
  
    Enemy& operator=(const Enemy&);
	 Enemy(const Enemy& s);
};
#endif
