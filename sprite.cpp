#include <iostream>
#include <cmath>
#include "sprite.h"
#include "gamedata.h"
#include "frameFactory.h"

Sprite::Sprite(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"X"), 
                    Gamedata::getInstance().getXmlInt(name+"Y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"SpeedX"), 
                    Gamedata::getInstance().getXmlInt(name+"SpeedY")) 
           ),
  frame( FrameFactory::getInstance().getFrame(name) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight"))
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity()), 
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight"))
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {

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

void Sprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

int Sprite::getDistance(const Sprite *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

float Sprite::getArea() const { 
  return	frameWidth*frameHeight;
}


void Sprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  
	setPosition(getPosition() + incr);


/*
  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
	//delete this;
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
	//delete this;
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
	//delete this;
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
	//delete this;
  }*/  
}
