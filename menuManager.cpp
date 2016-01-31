#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include "menuManager.h"

#include "manager.h"


MenuManager::MenuManager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  screen( IOManager::getInstance().getScreen() ),
  clock( Clock::getInstance() ),
  menuBackgroundSprite("menupagebackground"),
  backColor(),
  menu(),
  numberOfSprites(2)
{ 
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  backColor.r = Gamedata::getInstance().getXmlInt("backRed");
  backColor.g = Gamedata::getInstance().getXmlInt("backGreen");
  backColor.b = Gamedata::getInstance().getXmlInt("backBlue");
  atexit(SDL_Quit); 
}

void MenuManager::drawBackground() const {
  SDL_FillRect( screen, NULL, 
    SDL_MapRGB(screen->format, backColor.r, backColor.g, backColor.b) );
  SDL_Rect dest = {0, 0, 0, 0};
  
  menuBackgroundSprite.draw();
  SDL_BlitSurface( screen, NULL, screen, &dest );
  
  
  
}

void MenuManager::getNumberOfSprites() {
  IOManager& io = IOManager::getInstance().getInstance();
  SDL_Event event;
  bool done = false;
  bool nameDone = false;
  //const string msg("How many sprites: ");
  const string msg("Level ( 1 - Easy  2 - Medium  3 - Difficult) : ");
  io.clearString();
  while ( not done ) {
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if ( SDL_PollEvent(&event) )
    switch (event.type) {
      case SDL_KEYDOWN: {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
        }
        if (keystate[SDLK_RETURN]) {
          nameDone = true;
        }
        io.buildString(event);
      }
    }
    drawBackground();
    io.printStringAfterMessage(msg, 720, 420);
    if ( nameDone ) {
      std::string number = io.getString();
      std::stringstream strm;
      strm << number;
      strm >> numberOfSprites;
      strm.clear(); // clear error flags
      strm.str(std::string()); // clear contents
      //strm << "Okay -- you'll see " << numberOfSprites << " stars";
	  
	  if(numberOfSprites ==1 || numberOfSprites ==2 || numberOfSprites ==3)
	  strm << "Okay!!";
	  else
      strm << "Naaah!!";
	  
	  io.printMessageAt(strm.str(), 840, 460);
      SDL_Flip(screen);
      SDL_Delay(1000);
      done = true;
    }
    if ( !done ) {
      SDL_Flip(screen);
    }
  }
}


void MenuManager::displayHelp() {
  IOManager& io = IOManager::getInstance().getInstance();
  SDL_Event event;
  bool done = false;
  //bool nameDone = false;
  //const string msg("How many sprites: ");
  const string msgaim("Dodge the enemy Jets or light em up!");
  const string msg("Press up or down arrows to move the Jet.");
  const string msg1("Press r to shoot missiles.");
  const string msg2("Press f to re-fuel the Jet.");
  io.clearString();
  
  
  drawBackground();
  io.printMessageAt(msgaim, 745, 440);
  
  io.printMessageAt(msg, 730, 490);
  io.printMessageAt(msg1, 780, 520);
  io.printMessageAt(msg2, 780, 550);
    SDL_Flip(screen);
  
  while ( not done ) {
  
	//io.printMessageAt(msg, 750, 400);
    //SDL_Flip(screen);
  
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if ( SDL_PollEvent(&event) )
    switch (event.type) {
      case SDL_KEYDOWN: {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
        }/*
        if (keystate[SDLK_RETURN]) {
          nameDone = true;
        }*/
        io.buildString(event);
      }
    }
    //drawBackground();
    /*
    if ( nameDone ) {  
	  io.printMessageAt(msg, 750, 400);
      SDL_Flip(screen);
      SDL_Delay(500);
      done = true;
    }
    if ( !done ) {
      SDL_Flip(screen);
    }*/
  }
}





void MenuManager::play() {
  bool keyCatch = false; // To get only 1 key per keydown
  SDL_Event event;
  bool done = false;

  // Here, we need to create an instance of the Manager,
  // the one that manages the game:
   
   Manager manager;
   manager.pause();

   //Manager *manager;
   
   //manager = new Manager();
   
   bool firstTime = true;
   
   //manager->pause();
   
   //drawBackground();
   
  while ( not done ) {

  if(firstTime)
  {
	  //firstTime = false;
	  
	  //menuBackgroundSprite.X(0);
	  
	  drawBackground();
  }
  else
  {
	//menuBackgroundSprite.X(manager->getPlayerX() - (Gamedata::getInstance().getXmlInt("viewWidth")/4) + (Gamedata::getInstance().getXmlInt("JetRightWidth")/2 ));
	menuBackgroundSprite.X(manager.getPlayerX() - (Gamedata::getInstance().getXmlInt("viewWidth")/4) + (Gamedata::getInstance().getXmlInt("JetRightWidth")/2 ));
    drawBackground();
  }
    
	menu.draw();
    	
	
	SDL_Flip(screen);

    SDL_PollEvent(&event);
    if (event.type ==  SDL_QUIT) { break; }
    if(event.type == SDL_KEYDOWN) {
      switch ( event.key.keysym.sym ) {
        case SDLK_ESCAPE :
        case SDLK_q : {
          done = true;
          break;
        }
        case SDLK_RETURN : {
          if ( !keyCatch ) {
            menu.lightOn();
			
			//drawBackground();
			
			menu.draw();
		SDL_Flip(screen);
		SDL_Delay(250);
						
			if ( menu.getIconClicked() == "Start Game" ) {
			
			firstTime = false;
			
              // Here is where we call the play() function in Manager:
              std::cout << "Starting game ..." << std::endl;
			  
			  
				//manager->setLevel( numberOfSprites );
				manager.setLevel( numberOfSprites );
			  
				
               manager.unpause();
               manager.play();
               /*manager.pause();*/
			   /*
			   manager->unpause();
               manager->play();
			   */
			   
			   
               //if( manager->ifgameOver() )
			   if( manager.ifgameOver() )
			   {	
				  done = true;
				  
				  //delete manager;
			   }
			   else
			   {
				//manager->pause();
				manager.pause();
				}
			   
			   
			   
            }
			
            
            if ( menu.getIconClicked() == "Difficulty" ) {
              
			  getNumberOfSprites();
			  while(numberOfSprites!=1 && numberOfSprites!=2 && numberOfSprites!=3)
			  {getNumberOfSprites();}
              // After we get the number, we must pass it to Manager:
              // manager.setNumberOfSprites( numberOfSprites );
			   manager.setLevel( numberOfSprites );
			  // manager->setLevel( numberOfSprites );
            }
            if ( menu.getIconClicked() == "Help" ) {
              // Here is where we explain how to play the game"
             // std::cout << "Give help  ..." << std::endl;
			 displayHelp();
            }
            if ( menu.getIconClicked() == "Exit" ) {
              drawBackground();
              
			  menu.draw();
              SDL_Flip(screen);
              SDL_Delay(250);
              done = true;
			  
			  //delete manager;
			  
            }
          }
          break;
        }
        case SDLK_DOWN   : {
          if ( !keyCatch ) {
            menu.increment();
          }
          break;
        }
        case SDLK_UP   : {
          if ( !keyCatch ) {
            menu.decrement();
          }
          break;
        }
        default          : break;
      }
      keyCatch = true;
    }
    if(event.type == SDL_KEYUP) { 
      keyCatch = false; 
      menu.lightOff();
    }
  }
}
