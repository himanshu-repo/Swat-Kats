#include <list>
#include <vector>
#include <SDL.h>

#include "ioManager.h"
#include "clock.h"
#include "world.h"
#include "player.h"
#include "fuel.h"
#include "rain.h"
#include "enemy.h"
#include "viewport.h"
#include "sprite.h"
#include "bullets.h"
#include "multisprite.h"

class Manager 
{

  public:
	
	Manager ();
	~Manager ();
	void play();


  void pause() { clock.pause(); }
  void unpause() { clock.unpause(); }
  void setLevel(int);
	
	void adjustVelocity();
	
	bool ifgameOver() const { return gameOver; }
	
	float getPlayerX(){return myPlayer->X();}
	
  private:
	
	static int score;
	static int livesRemaining;
	
	Fuel bar;
	
	bool gameOver;
	
	int level ;
	
	float explosionInterval;
	float timeSinceLastExplosion;
	
	const bool env;
	
	const IOManager& io;
	Clock& clock;

	SDL_Surface * const screen;
	World world1,world2;
	Viewport& viewport;

	std::vector<Drawable*> sprites;
	  
	std::vector<Rain*> Rainsprites;
	  
	//std::vector<Drawable*> parachutes;

	std::vector<Sprite*> parachutes;
  
	
	Player *cachePlayer;
	
	MultiSprite *blast;
  
	Drawable *myPlayer;
  
	bool explosion;
  
	//std::list<Enemy*> enemies;
	
	std::vector<Enemy*> cacheEnemies;
	std::vector<MultiSprite*> enemyBlasts;
	std::vector<Drawable*> enemies;
	std::vector<bool> enemyExploded;
  
  
  
	Bullets bullets;
  
	//Fuel bar;
  
	int currentSprite;
 
    bool showHelp;
	bool showHUD;

	void draw() const;
	void update();

	Manager(const Manager&);
	Manager& operator=(const Manager&);
};


class ParachuteCompare {
public:
  bool operator()(Sprite* lhs, Sprite* rhs) { 
    return lhs->getArea() < rhs->getArea(); 
  }
};

