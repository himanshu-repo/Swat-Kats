#include <SDL.h>

#include "ioManager.h"
#include "menu.h"
#include "clock.h"

#include "sprite.h"

class MenuManager {
public:
  MenuManager ();
  void play();
  int getStars() const { return numberOfSprites; }

private:
	
 	

  bool env;
  SDL_Surface *screen;
  const Clock& clock;

  Sprite menuBackgroundSprite;
  
  SDL_Color backColor;
  Menu menu;
  int numberOfSprites;

    
  void drawBackground() const;
  MenuManager(const MenuManager&);
  MenuManager& operator=(const MenuManager&);
  void getNumberOfSprites();
  void displayHelp();
};
