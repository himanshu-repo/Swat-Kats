#include<list>
#include<iostream>
#include"bullet.h"
#include "ioManager.h"
#include "collisionStrategy.h"

class Bullets
{

	public :
	
	Bullets(const std::string&);

	Bullets(const Bullets &);
	
	~Bullets();
	
	void draw() const ;
	
	void update(Uint32 ticks);
	
	void shoot(const Vector2f& pos, const Vector2f& vel);
	
	unsigned int bulletCount() const {	return bulletList.size() ; }
	
	unsigned int freeCount() const {	return freeList.size() ; }
	
	//bool shooting() const { return bulletList.empty() ; }
	
	//bool collidedWith(const Sprite* obj) const;
	bool collidedWith(const Drawable* obj) const;
	
	private:
	
	std::string name;
	
	CollisionStrategy * strategy;
	
//	SDL_Surface *bulletSurface ;
//	Frame* bulletframe ;
	
	float frameInterval;
	float timeSinceLastFrame;
	
	mutable std::list<Bullet> freeList ;
	mutable std::list<Bullet> bulletList ;


	Bullets& operator=(const Bullets&);
};