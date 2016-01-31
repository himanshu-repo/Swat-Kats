#include <iostream>
#include <cmath>
#include <limits>
#include "rain.h"
#include "gamedata.h"
#include "frameFactory.h"

Rain::Rain(const std::string& name) :
  Drawable(name,
           Vector2f(rand()%(Gamedata::getInstance().getXmlInt("worldWidth")), 
                    rand()%(Gamedata::getInstance().getXmlInt("worldHeight"))), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"SpeedX"), 
                    Gamedata::getInstance().getXmlInt(name+"SpeedY")) 
           ),
  frame( FrameFactory::getInstance().getFrame(name) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight"))
{ }

Rain::Rain(const Rain& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity()), 
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight"))
{ }

Rain& Rain::operator=(const Rain& rhs) {
  setName( rhs.getName() );
  setPosition(rhs.getPosition());
  setVelocity(rhs.getVelocity());
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

void Rain::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

int Rain::getDistance(const Rain *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

void Rain::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  
	setPosition(getPosition() + incr);



  if ( Y() < 0) {
    //velocityY( abs( velocityY() ) );
	
	setPosition(Vector2f(rand()%(Gamedata::getInstance().getXmlInt("worldWidth")), 
                    rand()%(Gamedata::getInstance().getXmlInt("worldHeight"))));
	
	//delete this;
  }
  if ( Y() > worldHeight-frameHeight) {
    //velocityY( -abs( velocityY() ) );
	//delete this;
	
	setPosition(Vector2f(rand()%(Gamedata::getInstance().getXmlInt("worldWidth")), 
                    rand()%(Gamedata::getInstance().getXmlInt("worldHeight"))));
  }

  if ( X() < 0) {
    //velocityX( abs( velocityX() ) );
	//delete this;
	setPosition(Vector2f(rand()%(Gamedata::getInstance().getXmlInt("worldWidth")), 
                    rand()%(Gamedata::getInstance().getXmlInt("worldHeight"))));
  }
  if ( X() > worldWidth-frameWidth) {
    //velocityX( -abs( velocityX() ) );
	//delete this;
	setPosition(Vector2f(rand()%(Gamedata::getInstance().getXmlInt("worldWidth")), 
                    rand()%(Gamedata::getInstance().getXmlInt("worldHeight"))));
  }  
}
