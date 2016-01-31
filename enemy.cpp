#include <iostream>
#include <cmath>
#include <limits>
#include "enemy.h"
#include "gamedata.h"
#include "frameFactory.h"

#include "viewport.h"

#include <sstream>

Enemy::Enemy(const std::string& name , const Player& p) :
  Drawable(name,
           Vector2f(rand()%(Gamedata::getInstance().getXmlInt("worldWidth")), 
                    rand()%(Gamedata::getInstance().getXmlInt("worldHeight"))), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"SpeedX"), 
                    Gamedata::getInstance().getXmlInt(name+"SpeedY")) 
           ),
	currentMode(NORMAL),	   
	target(p),	
    io(IOManager::getInstance()),	
  frame( FrameFactory::getInstance().getFrame(name) ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight"))
{ }

Enemy::Enemy(const Enemy& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity()), 
  currentMode(s.currentMode),
  target(s.target),
  io(IOManager::getInstance()),	
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight"))
{ }

//float Enemy::safeDistance = 200;
float Enemy::safeDistance = Gamedata::getInstance().getXmlInt("Radius");
Enemy& Enemy::operator=(const Enemy& rhs) {
  setName( rhs.getName() );
  setPosition(rhs.getPosition());
  setVelocity(rhs.getVelocity());
  currentMode = rhs.currentMode;
  //target = rhs.target;
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

void Enemy::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
  
  //writing mode
  /*
  int xx =  X() + Viewport::getInstance().X();
  int yy =  Y() + Viewport::getInstance().Y();
  
  std::stringstream strm;
  
  strm<<currentMode;
  io.printMessageAt(strm.str(),xx,yy);*/
}

int Enemy::getDistance(const Enemy *obj) const { 
  return hypot(X()-obj->X(), Y()-obj->Y());
}

float distance(float x1 , float y1 , float x2 , float y2)
{
	float dx = x1 - x2;
	float dy = y1 - y2;
	
	return hypot( dx , dy );
}

void Enemy::goLeft()
{
	setVelocity(Vector2f(Gamedata::getInstance().getXmlInt(getName()+"SpeedX")-Gamedata::getInstance().getXmlInt(getName()+"AttackSpeedX"),Gamedata::getInstance().getXmlInt(getName()+"SpeedY"))) ;
}

void Enemy::goUp()
{
	//velocityY(velocityY() - 100);
	setVelocity(Vector2f(Gamedata::getInstance().getXmlInt(getName()+"SpeedX"),Gamedata::getInstance().getXmlInt(getName()+"SpeedY")-Gamedata::getInstance().getXmlInt(getName()+"AttackSpeedY"))) ;
}

void Enemy::goDown()
{
	//velocityY(velocityY() +100);
	setVelocity(Vector2f(Gamedata::getInstance().getXmlInt(getName()+"SpeedX"),Gamedata::getInstance().getXmlInt(getName()+"SpeedY")+Gamedata::getInstance().getXmlInt(getName()+"AttackSpeedY"))) ;
}

void Enemy::stopY()
{
	velocityY(0);
}

void Enemy::update(Uint32 ticks) { 
	
	float enemy_x = X() + getFrame()->getWidth()/2 ;			//enemy
	float enemy_y = Y() + getFrame()->getHeight()/2 ;			//enemy
	
	float player_x = target.X() + target.getFrame()->getWidth()/2;	//player
	float player_y = target.Y() + target.getFrame()->getHeight()/2;	//player

	float distanceToTarget = ::distance(enemy_x,enemy_y,player_x,player_y);

	if(currentMode == NORMAL)
	{
		if(distanceToTarget < safeDistance)
		{
			currentMode = ATTACK;
		}
	}
	else if(currentMode == ATTACK)
	{
		if(distanceToTarget > safeDistance)
		{
			currentMode = NORMAL;
			
			setVelocity(Vector2f(Gamedata::getInstance().getXmlInt(getName()+"SpeedX"),Gamedata::getInstance().getXmlInt(getName()+"SpeedY"))) ;
		}
		else
		{
			if(enemy_x > player_x)
			{
				goLeft();
			}
			
			
			if( (enemy_y < (player_y+20)) &&  (enemy_y > (player_y-20)) )
			{
				stopY();
			}
			else if( enemy_y > player_y)
			{
				goUp();
			}	
			else if( enemy_y < player_y)
			{	
				goDown();
			}
			
		}
	}
	
	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  
	setPosition(getPosition() + incr);


/*
  if ( Y() < 0) {
    //velocityY( abs( velocityY() ) );
	
	setPosition(Vector2f( x +(Gamedata::getInstance().getXmlInt("viewWidth")/2) , rand()%(Gamedata::getInstance().getXmlInt("worldHeight")/3) ) );
	
	//setPosition(Vector2f(rand()%(Gamedata::getInstance().getXmlInt("worldWidth")),rand()%(Gamedata::getInstance().getXmlInt("worldHeight"))));
	
	//delete this;
  }
  if ( Y() > worldHeight-frameHeight) {
    //velocityY( -abs( velocityY() ) );
	//delete this;
	
	setPosition(Vector2f(x +(Gamedata::getInstance().getXmlInt("viewWidth")/2) , rand()%(Gamedata::getInstance().getXmlInt("worldHeight")/3) ) );
	
	//setPosition(Vector2f(rand()%(Gamedata::getInstance().getXmlInt("worldWidth")),rand()%(Gamedata::getInstance().getXmlInt("worldHeight"))));
  }

  if ( X() < 0) {
    //velocityX( abs( velocityX() ) );
	//delete this;
	
	setPosition(Vector2f(x + (Gamedata::getInstance().getXmlInt("viewWidth")/2) , rand()%(Gamedata::getInstance().getXmlInt("worldHeight")/3) ) );
	
	//setPosition(Vector2f(rand()%(Gamedata::getInstance().getXmlInt("worldWidth")),                    rand()%(Gamedata::getInstance().getXmlInt("worldHeight"))));
  }
  if ( X() > worldWidth-frameWidth) {
    //velocityX( -abs( velocityX() ) );
	//delete this;
	
	setPosition(Vector2f(x + (Gamedata::getInstance().getXmlInt("viewWidth")/2) , rand()%(Gamedata::getInstance().getXmlInt("worldHeight")/3) ) );
	
	//setPosition(Vector2f(rand()%(Gamedata::getInstance().getXmlInt("worldWidth")),                     rand()%(Gamedata::getInstance().getXmlInt("worldHeight"))));
  }  */
}
