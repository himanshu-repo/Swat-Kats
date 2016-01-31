#include <iostream>
#include <cmath>
#include "player.h"
#include "gamedata.h"
#include "frameFactory.h"

//CollisionStrategy* getStrategy1(const string& name){
CollisionStrategy* getStrategy1(){

	std::string sName = Gamedata::getInstance().getXmlStr("PlayerCollisionStrategy");
    
    if (sName == "midpoint")
        return new MidPointCollisionStrategy;
        
    if (sName == "rectangular")
        return new RectangularCollisionStrategy;
    
    if (sName == "perpixel")
        return new PerPixelCollisionStrategy;
    
    throw std::string("No strategy in getStrategy");
}


void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Player::Player( const std::string& name) :
  Drawable(name, 
          Vector2f(Gamedata::getInstance().getXmlInt(name+"RightX"), 
                    Gamedata::getInstance().getXmlInt(name+"RightY")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"RightSpeedX"),
                    Gamedata::getInstance().getXmlInt(name+"RightSpeedY"))
           ),
		   strategy(getStrategy1()),
  framesLeft( FrameFactory::getInstance().getFrames(name+"Left") ),
  framesRight( FrameFactory::getInstance().getFrames(name+"Right") ),
  frames(framesRight),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight")),

  dt(0),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"RightFrames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"RightFrameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ }

Player::Player(const std::string& name,
                         const Vector2f& pos, const Vector2f& vel,
                         const std::vector<Frame*>& fmsLeft,const std::vector<Frame*>& fmsRight) : 
  Drawable(name, pos, vel), 
  strategy(getStrategy1()),
  framesLeft(fmsLeft),
  framesRight(fmsRight),
  frames(fmsRight),
  worldWidth(Gamedata::getInstance().getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance().getXmlInt("worldHeight")),

  dt(0),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"RightFrames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"RightFrameInterval") ),
  timeSinceLastFrame( 0 ),
  frameWidth(fmsRight[0]->getWidth()),
  frameHeight(fmsRight[0]->getHeight())
{ }

Player::Player(const Player& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity()), 
  strategy(s.strategy),
  framesLeft(s.framesLeft),
  framesRight(s.framesRight),
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  dt(s.dt),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight )
  { }

  
   bool Player::collidedWith(const Drawable* obj) const {
    
	if(strategy->execute(*this, *obj))
	{
		return true;
	}
    
    return false;
}
  
  
  
void Player::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[currentFrame]->draw(x, y);
}

void Player::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    //velocityY( abs( velocityY() ) );
	//velocityY( 0 );
	stopY();
  }
  if ( Y() > worldHeight-frameHeight) {
    //velocityY( -abs( velocityY() ) );
	//velocityY( 0 );
	stopY();
  }

  if ( X() < 0) {
   // velocityX( abs( velocityX() ) );
	
	 //velocityX( 0 );
	stopX();
	//frames = framesRight;
  }
  if ( X() > worldWidth-frameWidth) {
  
    //velocityX(0);
	stopX();
 
  //  velocityX( -abs( velocityX() ) );
	
	//frames = framesLeft;
  }  

}

void Player::goRight() 
{ /*
	if ( X() < worldWidth-frameWidth) 
	{
		frames = framesRight;
		//velocityX(Gamedata::getInstance().getXmlInt(name+"SpeedX"));
		velocityX(1200);
	}
	else
	{
		stopX() ;
	}*/
}

void Player::goLeft() 
{/*
	 if ( X() > 0) 
	 {
	 
		frames = framesLeft;
		//velocityX(-1*Gamedata::getInstance().getXmlInt(name+"SpeedX"));
		velocityX(-1600);
	}
	else
	{
		stopX() ;
	}*/
}

void Player::goUp() 
{
	 if ( Y() > 50) 
	 {
	 
		//frames = framesLeft;
		//velocityX(-1*Gamedata::getInstance().getXmlInt(name+"SpeedX"));
		velocityY(-400);
	}
	else
	{
		stopY() ;
	}
}

void Player::goDown() 
{
	 if ( Y() < 2*(worldHeight-frameHeight)/3) 
	 {
	 
		//frames = framesLeft;
		//velocityX(-1*Gamedata::getInstance().getXmlInt(name+"SpeedX"));
		velocityY(400);
	}
	else
	{
		stopY() ;
	}
}

void Player::stopX() 
{
	//velocityX(0);
}
void Player::stopY() 
{
	velocityY(0);
}

