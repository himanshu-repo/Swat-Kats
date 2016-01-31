#include <iostream>
#include <string>
#include <limits>
#include <algorithm>
#include <cstdlib>
#include "sprite.h"
#include "multisprite.h"
#include "twowaymultisprite.h"
#include "player.h"
#include "gamedata.h"
#include "manager.h"
#include "sound.h"
#include <list>

int Manager::score =0;
int Manager::livesRemaining =4;

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
	}
	
	//delete myPlayer;
	delete cachePlayer;
	delete blast;
  
  
  for (unsigned i = 0; i < Rainsprites.size(); ++i) 
  {
    delete Rainsprites[i];
  }
  for (unsigned i = 0; i < parachutes.size(); ++i) 
  {
    delete parachutes[i];
  }
  
  /*
  for (unsigned i = 0; i < cacheEnemies.size(); ++i) 
  {
    delete cacheEnemies[i];
  }
  
   for (unsigned i = 0; i < enemyBlasts.size(); ++i) 
  {
    delete enemyBlasts[i];
  }
  
   for (unsigned i = 0; i < enemies.size(); ++i) 
  {
    delete enemies[i];
  }*/
}

Manager::Manager() :
	bar(),
	gameOver(false),
	level(2),
	explosionInterval(Gamedata::getInstance().getXmlInt("NoKillInterval")),
	timeSinceLastExplosion(0),
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  world1("back", Gamedata::getInstance().getXmlInt("backFactor") ),
  world2("back1", Gamedata::getInstance().getXmlInt("back1Factor") ),
  viewport( Viewport::getInstance() ),
  sprites(),
  Rainsprites(),
  parachutes(),
  cachePlayer(new Player("Jet")),
  blast(new MultiSprite("blast")),
  myPlayer(cachePlayer),
  explosion(false),
  cacheEnemies(),
  enemyBlasts(),
  enemies(),
  enemyExploded(),
  bullets("bullet"),
  currentSprite(0),
  showHelp(false),
  showHUD(true)
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  atexit(SDL_Quit);
  //sprites.push_back( new TwoWayMultiSprite("Jet") );
  
  //Player myPlayer = new Player("Jet");
  
  for(unsigned int i =0; i<5000 ; i++)
  Rainsprites.push_back( new Rain("smallrain") );
  
  for(unsigned int i =0; i<1000 ; i++)
  Rainsprites.push_back( new Rain("bigrain") );
  
  //sprites.push_back( myPlayer );
  
  sprites.push_back( new TwoWayMultiSprite("tanks") );
  
  sprites.push_back( new Sprite("logo") );
  //viewport.setObjectToTrack(sprites[currentSprite]);
  //viewport.setObjectToTrack(sprites[1]);
  viewport.setObjectToTrack(myPlayer);
  
  clock.pause();
  
}

void Manager::adjustVelocity()
{
	if(level == 1)
	{
		myPlayer->velocityX(2 * Gamedata::getInstance().getXmlInt("JetRightSpeedX")/3);
	}
	else if(level == 2)
	{
		myPlayer->velocityX(Gamedata::getInstance().getXmlInt("JetRightSpeedX"));
	}
	else
	{
		myPlayer->velocityX(4*Gamedata::getInstance().getXmlInt("JetRightSpeedX")/3);
	}
}


void Manager::setLevel(int l)
{
	level = l;
	
	adjustVelocity();
}



void Manager::draw() const {
/*
	Fuel bar;

	unsigned prevTicks = 0;
  unsigned currTicks = SDL_GetTicks();
  unsigned ticks = 0;*/
//  Uint32 ticks = 0;



  world1.draw();
  
  for (unsigned i = 0; i < 5*Rainsprites.size()/6; ++i) {
   	
	
	 Rainsprites[i]->draw();
   
  }
  
  /////functor sort
  
 // sort(parachutes.begin(), parachutes.end(), ParachuteCompare());
 //std::sort(parachutes.begin(), parachutes.end(), ParachuteCompare()); 
   
  
  for (unsigned i = 0; i < parachutes.size(); ++i) {
   	
	//if(i!=2)
	 parachutes[i]->draw();
   
  }
  world2.draw();
  for (unsigned i = 0; i < sprites.size(); ++i) {
   	
	//if(i!=2)
	if(i!=1)
	 sprites[i]->draw();
   
  }

for (unsigned i = 5*Rainsprites.size()/6; i < Rainsprites.size(); ++i) 
{	
	 Rainsprites[i]->draw();  
}
  
  myPlayer->draw();
 
 for (unsigned i = 0; i < enemies.size(); ++i) 
{	
	 enemies[i]->draw();  
}
 
 
  bullets.draw();
  
  /*
  io.printMessageValueAt("FPS: ", clock.getAvgFps(), 10, 30);
  string message = "Tracking: "+ sprites[currentSprite]->getName(); 
	  io.printMessageAt(message, 150, 30);
   io.printMessageAt("F1: Help", 200, 30);
  */
  if(showHelp)
  {
	/*
	sprites[2]->velocityX(0);
	sprites[2]->velocityY(0);
	sprites[2]->draw();
	*/
	sprites[1]->velocityX(0);
	sprites[1]->velocityY(0);
	sprites[1]->draw();
	
	  io.printMessageAt("Press r to reset fuel", 580, 460);
	  io.printMessageAt("Press h to hide HUD", 580, 480);
	  
	  io.printMessageAt("Press e to explode the player", 570, 500);
	  //io.printMessageAt("Press t to switch sprites", 570, 500);
	  io.printMessageAt("Press f to fire missiles!!", 570, 520);
	  io.printMessageAt("Press p to call Parachute Backup!!", 530, 540);
	 // io.printMessageAt("Press Up, Down , Right arrows to move player", 530, 560);
	  
	  
	  
	  io.printMessageAt("Credits", 630, 590);
	  io.printMessageAt("Himanshu Chaturvedi", 570, 610);
  }
/*
   // **************** Draw Health Meter ********************
     bar.draw();
     io.printMessageCenteredAt("Press f1 to reset health meter", 10);
     currTicks = SDL_GetTicks();
	// currTicks = clock.getElapsedTicks();
     ticks = currTicks-prevTicks;
     prevTicks = currTicks;
     bar.update(ticks);
  */  
  //SDL_Flip(screen);
}

void Manager::update() {
  ++clock;
  Uint32 ticks = clock.getElapsedTicks();
  
  timeSinceLastExplosion += ticks; 
  
  for (unsigned int i = 0; i < Rainsprites.size(); ++i) {
    Rainsprites[i]->update(ticks);
  }
  for (unsigned int i = 0; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
  }
  for (unsigned int i = 0; i < parachutes.size(); ++i) {
    parachutes[i]->update(ticks);
  }
  world1.update();
  world2.update();
  
  myPlayer->update(ticks);
  
 for (unsigned int i = 0; i < enemies.size(); ++i) {
 
    enemies[i]->update(ticks);
	
  }
  
  //check for each enemy if it collided with any bullet
 
  for(unsigned i =0; i< enemies.size(); ++i)
  {
	if(!enemyExploded[i])
	{
		if(bullets.collidedWith(enemies[i]))  // i th enemy was killed
		{
			score++;
		
			//change its status to killed
			enemyExploded[i] = true;
					
			enemyBlasts[i]->resetCurrentFrame();
			
			enemyBlasts[i]->setPosition(Vector2f(enemies[i]->X(),enemies[i]->Y()));
			
			enemyBlasts[i]->setVelocity(Vector2f(0,0));
			
			enemies[i] = enemyBlasts[i] ;
		}
	}
  }
  
	
  if(timeSinceLastExplosion > explosionInterval)
  {
	   //check player with each enemy
	  for(unsigned i =0; i< enemies.size(); ++i)
	  {
		if(!enemyExploded[i] && explosion==false)
		{
			if(static_cast<Player*>(myPlayer)->collidedWith(enemies[i]))  // player collided with enemy 
			{
							
				livesRemaining--;
			
				//change enemy status to killed
				enemyExploded[i] = true;
						
				enemyBlasts[i]->resetCurrentFrame();
				
				enemyBlasts[i]->setPosition(Vector2f(enemies[i]->X(),enemies[i]->Y()));
				
				enemyBlasts[i]->setVelocity(Vector2f(0,0));
				
				enemies[i] = enemyBlasts[i] ;
				
				
				
				explosion = true;
				
				blast->resetCurrentFrame();
				
				blast->setPosition(Vector2f(myPlayer->X(),myPlayer->Y()));
				
				blast->setVelocity(Vector2f(0,0));
				
				myPlayer = blast ;
				
			}
		}
	  }
  
  }
  
  
  bullets.update(ticks);
  
  viewport.update(); // always update viewport last
}

void Manager::play() {
  SDL_Event event;

  SDLSound sound;
  
  bool done = false;
  bool keyCatch = false;
  
  int rnd ;
  
  //Fuel bar;

	unsigned prevTicks = 0;
  unsigned currTicks = SDL_GetTicks();
  unsigned ticks = 0;
  
  
  clock.unpause();
    
  
  while ( not done ) {
  
  
	if ( ( myPlayer->X() > ( Gamedata::getInstance().getXmlInt("worldWidth")  - 200) ) || (livesRemaining<0) ) {
	  
	  
	  io.printMessageAt("Game Over !!!", 800, 400);
		
		gameOver = true;
	  
		clock.pause();
		
			SDL_Flip(screen);
		
		SDL_Delay(1250);
		
        done = true;
        break;
      }
  
  

	//if(cacheEnemies.size()<=5)
	//{
		
		if(level == 1)
		{
			rnd = rand()%200;
		}
		else if(level == 2)
		{
			rnd = rand()%100;
		}
		else
		{
			rnd = rand()%50;
		}
	  
		if(rnd == 5)
		{
			//Enemy *newenemy = new Enemy("enemy");
			Enemy *newenemy = new Enemy("enemy", *cachePlayer);
			
			newenemy->setPosition(Vector2f(cachePlayer->X() + (3*Gamedata::getInstance().getXmlInt("viewWidth")/4) , rand()%(2*Gamedata::getInstance().getXmlInt("worldHeight")/3)+60 ) );
			
			
			cacheEnemies.push_back(newenemy); //insert into cache
			enemies.push_back(newenemy); //insert into enemies currently drawn
			enemyBlasts.push_back(new MultiSprite("blast")); //insert corresponding blast sprite
			enemyExploded.push_back(false); //on creation explosion is false
			
			
		}	
	//}
	
	
	for(unsigned i=0; i<enemies.size(); ++i)
	{
		if(enemyExploded[i])
		{
			//if ( static_cast<MultiSprite*>(enemies[i])->getCurrentFrame() == 0 ) 
			//sound[1];
		
			if ( static_cast<MultiSprite*>(enemies[i])->getCurrentFrame() == 6 ) 
			{	/*
				//enemyExploded[i] = false;
				cacheEnemies[i].erase;
				enemies[i].erase;
				enemyBlasts[i].erase;
				enemyExploded[i].erase;
				//enemies[i] = cacheEnemies[i] ;
				*/
				unsigned ctr =0;
				std::vector<Enemy*>::iterator ptr = cacheEnemies.begin();
				while ( ctr < i ) 
				{
					ctr++;
					ptr++;
				}
				cacheEnemies.erase(ptr);
				
				
				ctr =0;
				std::vector<MultiSprite*>::iterator ptr1 = enemyBlasts.begin();
				while ( ctr < i ) 
				{
					ctr++;
					ptr1++;
				}
				enemyBlasts.erase(ptr1);
				
				ctr =0;
				std::vector<Drawable*>::iterator ptr2 = enemies.begin();
				while ( ctr < i ) 
				{
					ctr++;
					ptr2++;
				}
				enemies.erase(ptr2);
				
				ctr =0;
				std::vector<bool>::iterator ptr3 = enemyExploded.begin();
				while ( ctr < i ) 
				{
					ctr++;
					ptr3++;
				}
				enemyExploded.erase(ptr3);
				
			}
		}
	}
	
	
	if(explosion)
	{
		
		if ( static_cast<MultiSprite*>(myPlayer)->getCurrentFrame() == 0 )
		{
			sound[1];
		}
		
		if ( static_cast<MultiSprite*>(myPlayer)->getCurrentFrame() == 6 ) 
		{
			timeSinceLastExplosion = 0;
		
			explosion = false;
			
			myPlayer = cachePlayer ;
		}
	}
    SDL_PollEvent(&event);
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (event.type ==  SDL_QUIT) { done = true; break; }
    
	if(event.type == SDL_KEYUP) { 
	
		//std::cout<<"******************8KEY_UP*****";
	
      keyCatch = false; 
	  if (!keystate[SDLK_RIGHT] && !keystate[SDLK_LEFT])
        static_cast<Player*>(myPlayer)->stopX();
		if (!keystate[SDLK_UP] && !keystate[SDLK_DOWN])
        static_cast<Player*>(myPlayer)->stopY();
		
		
		
		
		if (keystate[SDLK_LEFT] && !keyCatch && !showHelp ) {
        if(bar.getcurrentLength())
		{
			static_cast<Player*>(myPlayer)->goLeft();
			//keyCatch = false; 
		}	
		else
		{
			static_cast<Player*>(myPlayer)->stopX();
			static_cast<Player*>(myPlayer)->stopY();
		}	
      }
	  if (keystate[SDLK_RIGHT]  && !showHelp) {
	  
		//std::cout<<bar.getcurrentLength();
		if(bar.getcurrentLength()>0)
		{
		//	std::cout<<"Condition checked";
			static_cast<Player*>(myPlayer)->goRight();
		}
		else
		{
			//std::cout<<"Condition FALSE";
			static_cast<Player*>(myPlayer)->stopX();
			static_cast<Player*>(myPlayer)->stopY();
		}	
		
      }
	  
	  if (keystate[SDLK_UP]  && !showHelp && !explosion) {
	  
		//std::cout<<bar.getcurrentLength();
		if(bar.getcurrentLength()>0)
		{
		//	std::cout<<"Condition checked";
			static_cast<Player*>(myPlayer)->goUp();
		}
		else
		{
			//std::cout<<"Condition FALSE";
			static_cast<Player*>(myPlayer)->stopX();
			static_cast<Player*>(myPlayer)->stopY();
		}	
		
      }
	  
	  if (keystate[SDLK_DOWN] && !showHelp && !explosion  ) {
	  
		//std::cout<<bar.getcurrentLength();
		if(bar.getcurrentLength()>0)
		{
		//	std::cout<<"Condition checked";
			static_cast<Player*>(myPlayer)->goDown();
		}
		else
		{
		//	std::cout<<"Condition FALSE";
			static_cast<Player*>(myPlayer)->stopX();
			static_cast<Player*>(myPlayer)->stopY();
		}	
		
      }
		
		
		
    }
	
	
	
    if(event.type == SDL_KEYDOWN) {
      if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
	  
		clock.pause();
        done = true;
        break;
      }
	  
	  if (keystate[SDLK_SPACE] && !keyCatch) {
        keyCatch = true;
		
		//io.clearString();
            sound.toggleMusic();
		
        break;
      }
	  
	  
	  if (keystate[SDLK_r] && !keyCatch && !explosion) 
	  {
		keyCatch = true;
				
		sound[0];
		
		bullets.shoot( Vector2f(myPlayer->X(),myPlayer->Y() +40) , Vector2f(Gamedata::getInstance().getXmlInt("bulletSpeedX"),Gamedata::getInstance().getXmlInt("bulletSpeedY")) );
		
		
      }
	  
	  if (keystate[SDLK_p] && !keyCatch && parachutes.size()<100) 
	  {
		keyCatch = true;
		Sprite *newparachute;
		
		int rnd = rand()%3;
		
		if(rnd==0)
		{
			newparachute= new Sprite("parachuteBig") ;
			
			//newparachute->setPosition( Vector2f(sprites[0]->X() + rand()%(Gamedata::getInstance().getXmlInt("viewWidth")/2) , rand()%100) );
			newparachute->setVelocity( Vector2f(-100 , 100 ));
		}
		else if(rnd==1)
		{
			newparachute= new Sprite("parachuteMed") ;
			
			//newparachute->setPosition( Vector2f(sprites[0]->X() + rand()%(Gamedata::getInstance().getXmlInt("viewWidth")/2) , rand()%100) );
		
			newparachute->setVelocity( Vector2f(-50 , 50 ));
		}
		else
		{
			newparachute= new Sprite("parachuteSmall") ;
			
			//newparachute->setPosition( Vector2f(sprites[0]->X() + rand()%(Gamedata::getInstance().getXmlInt("viewWidth")/2) , rand()%100) );
		
			newparachute->setVelocity( Vector2f(-25 , 25 ));
		}
		
		newparachute->setPosition( Vector2f(sprites[0]->X() + rand()%(Gamedata::getInstance().getXmlInt("viewWidth")/2) , rand()%100) );
		
				
		parachutes.push_back(newparachute);
	

	std::sort(parachutes.begin(), parachutes.end(), ParachuteCompare()); 		
      }
	  
	  
	  
	 /* 
      if (keystate[SDLK_t] && !keyCatch) {
        keyCatch = true;
       // currentSprite = (currentSprite+1) % sprites.size();
	    currentSprite = (currentSprite+1) % 2;
        viewport.setObjectToTrack(sprites[currentSprite]);
      }*//*
	if (keystate[SDLK_F1] && !keyCatch) 
	{
		keyCatch = true;
	
			
        if(showHelp==true)
		{
			showHelp=false;
			 //viewport.setObjectToTrack(sprites[currentSprite]);
			 viewport.setObjectToTrack(myPlayer);
		}
		else
		{
			//viewport.setObjectToTrack(sprites[2]);
			viewport.setObjectToTrack(sprites[1]);
			showHelp=true;
		}
    }*/
	
	if (keystate[SDLK_h]&& !keyCatch) 
	{
		keyCatch = true;
		if(showHUD==true)
		{
			showHUD=false;
			 //viewport.setObjectToTrack(sprites[currentSprite]);
		}
		else
		{
			//viewport.setObjectToTrack(sprites[2]);
			showHUD=true;
		}
    }
	/*
      if (keystate[SDLK_s] && !keyCatch) {
        keyCatch = true;
        clock.toggleSloMo();
      }*/
	  
	  if (keystate[SDLK_LEFT] && !showHelp && !explosion ) {
        if(bar.getcurrentLength())
		{
			static_cast<Player*>(myPlayer)->goLeft();
			//keyCatch = false; 
		}	
		else
		{
			static_cast<Player*>(myPlayer)->stopX();
			static_cast<Player*>(myPlayer)->stopY();
		}	
      }
	  if (keystate[SDLK_RIGHT]  && !showHelp && !explosion ) {
	  
		//std::cout<<bar.getcurrentLength();
		if(bar.getcurrentLength()>0)
		{
			//std::cout<<"Condition checked";
			static_cast<Player*>(myPlayer)->goRight();
		}
		else
		{
			//std::cout<<"Condition FALSE";
			static_cast<Player*>(myPlayer)->stopX();
			static_cast<Player*>(myPlayer)->stopY();
		}	
		
      }
	  
	  if (keystate[SDLK_UP]  && !showHelp && !explosion) {
	  
		//std::cout<<bar.getcurrentLength();
		if(bar.getcurrentLength()>0)
		{
			//std::cout<<"Condition checked";
			static_cast<Player*>(myPlayer)->goUp();
		}
		else
		{
		//	std::cout<<"Condition FALSE";
			static_cast<Player*>(myPlayer)->stopX();
			static_cast<Player*>(myPlayer)->stopY();
		}	
		
      }
	  
	  if (keystate[SDLK_DOWN] && !showHelp && !explosion ) {
	  
		//std::cout<<bar.getcurrentLength();
		if(bar.getcurrentLength()>0)
		{
		//	std::cout<<"Condition checked";
			static_cast<Player*>(myPlayer)->goDown();
		}
		else
		{
			//std::cout<<"Condition FALSE";
			static_cast<Player*>(myPlayer)->stopX();
			static_cast<Player*>(myPlayer)->stopY();
		}	
		
      }
	   
	  if (keystate[SDLK_f]&& !keyCatch ) {
	 
		  keyCatch = true;
	  //	std::cout<<" R PRESSED ";
		//keyCatch = false; 
		//keystate[SDLK_RIGHT] = 1;
        bar.reset();
      }
	  
	  if (keystate[SDLK_e]&& !keyCatch && explosion==false ) {
	 
			sound[1];
	 
			//std::cout<<"E pressed";
	 
		  keyCatch = true;
		
			explosion = true;
			
			blast->resetCurrentFrame();
			
			blast->setPosition(Vector2f(myPlayer->X(),myPlayer->Y()));
			
			blast->setVelocity(Vector2f(0,0));
			
			myPlayer = blast ;
		
      }
	  /*
	  if (keystate[SDLK_k]&& !keyCatch) {
	 
			sound[1];
	 
			//std::cout<<"E pressed";
	 
		  keyCatch = true;
		
			for(unsigned i=0; i<enemies.size(); i++)
			{
				enemyExploded[i] = true;
				
				enemyBlasts[i]->resetCurrentFrame();
				
				enemyBlasts[i]->setPosition(Vector2f(enemies[i]->X(),enemies[i]->Y()));
				
				enemyBlasts[i]->setVelocity(Vector2f(0,0));
				
				enemies[i] = enemyBlasts[i] ;
			}
      }*/
	  
	  
    }

	//std::sort(parachutes.begin(), parachutes.end(), ParachuteCompare()); 	
    draw();
	
	
	// **************** Draw Health Meter ********************
     
	 //io.printMessageCenteredAt("Press r to reset health meter", 10);
     currTicks = SDL_GetTicks();
	// currTicks = clock.getElapsedTicks();
     ticks = currTicks-prevTicks;
     prevTicks = currTicks;
	 
	 
	 if(showHUD)
	 {
		bar.draw();
		io.printMessageValueAt("FPS: ", clock.getAvgFps(), 10, 20);
		//string message = "Tracking: "+ sprites[currentSprite]->getName(); 
		//io.printMessageAt(message, 80, 20);
		//io.printMessageAt("F1: Help", 230, 20);
		io.printMessageAt("Fuel : ", 1290, 20);
		
		io.printMessageValueAt("Level: ", level, 250, 20);
		
		io.printMessageValueAt("Score: ", score, 600, 20);
		
		//io.printMessageValueAt("timeSinceLastExplosion: ", timeSinceLastExplosion, 500, 20);
		
		io.printMessageValueAt("Lives Remaining: ", livesRemaining, 900, 20);
		
		if(rand()%10<9 && bar.getcurrentLength()<150)
			io.printMessageAt("Re-Fuel!!", 1200, 20);
     }
	 
	 
	
	if(!showHelp)
	bar.update(ticks);
	 
		 
	SDL_Flip(screen);
	
    update();
  }
}
