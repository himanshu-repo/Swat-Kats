#include<iostream>
#include <cmath>
#include"ioManager.h"
#include"gamedata.h"
#include"bullets.h"

#include "clock.h"

CollisionStrategy* getStrategy(const string& name){
    std::string sName = Gamedata::getInstance().getXmlStr(name+"Strategy");
    
    if (sName == "midpoint")
        return new MidPointCollisionStrategy;
        
    if (sName == "rectangular")
        return new RectangularCollisionStrategy;
    
    if (sName == "perpixel")
        return new PerPixelCollisionStrategy;
    
    throw std::string("No strategy in getStrategy");
}



Bullets::~Bullets()
{
//	SDL_FreeSurface(bulletSurface);
//	delete bulletFrame;

	delete strategy ;
}

Bullets::Bullets(const std::string& n):
	name(n),
	strategy(getStrategy(name)),
	frameInterval(Gamedata::getInstance().getXmlInt(name+"Interval")),
	timeSinceLastFrame(0),
	freeList(),
	bulletList()
	{}
	
Bullets::Bullets(const Bullets& rhs):
	name(rhs.name),
	strategy(rhs.strategy),
	frameInterval(rhs.frameInterval),
	timeSinceLastFrame(rhs.timeSinceLastFrame),
	freeList(rhs.freeList),
	bulletList(rhs.bulletList)
	{}
	
Bullets& Bullets::operator=(const Bullets& rhs)
{
	name = rhs.name;
	strategy = rhs.strategy ;
	frameInterval = rhs.frameInterval;
	timeSinceLastFrame = rhs.timeSinceLastFrame;
	freeList = rhs.freeList;
	bulletList = rhs.bulletList;
	
	return *this;
}


//bool Bullets::collidedWith(const Sprite* obj) const {
bool Bullets::collidedWith(const Drawable* obj) const {
    std::list<Bullet>::iterator ptr = bulletList.begin();
    while (ptr != bulletList.end()) {
        if(strategy->execute(*ptr, *obj))
        {
            freeList.push_back(*ptr);
            ptr = bulletList.erase(ptr);
            return true;
        }
        ++ptr;
    }
    return false;
}


void Bullets::shoot(const Vector2f& position , const Vector2f& velocity)
{
	if(timeSinceLastFrame > frameInterval)
	{
		if(freeList.empty())
		{
			Bullet b(name);
			b.setVelocity(velocity);
			b.setPosition(position);
			
			bulletList.push_back(b);
		}
		else
		{
			Bullet b = freeList.front();
			freeList.pop_front();
			
			b.reset();
			
			b.setVelocity(velocity);
			b.setPosition(position);
			
			bulletList.push_back(b);
		}
		
		timeSinceLastFrame = 0;
	}
}

void Bullets::draw() const
{/*
	unsigned k = bulletList.size() ;

	IOManager::getInstance().printMessageValueAt("Bullets List: " ,k, 350, 60 );
	
	k = freeList.size() ;
	
	IOManager::getInstance().printMessageValueAt("Free List: " ,k, 550, 60 );
	
	*/
	std::list<Bullet>::iterator ptr = bulletList.begin();
	
	while( ptr!= bulletList.end() )
	{
		ptr->draw();
		++ptr;
	}
}

void Bullets::update(Uint32 ticks)
{
	timeSinceLastFrame += ticks; 
	
	std::list<Bullet>::iterator ptr = bulletList.begin();
	
	while( ptr!= bulletList.end() )
	{
		ptr->update(ticks);
		
		if(ptr->goneTooFar())
		{
			freeList.push_back(*ptr);
		
			ptr = bulletList.erase(ptr);
		}
		else
		++ptr;
	}
}