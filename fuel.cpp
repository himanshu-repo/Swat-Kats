#include "fuel.h"

Fuel::Fuel() :
  screen(IOManager::getInstance().getScreen()),
  start(Vector2f(1350, 25)), 
  totalLength(400), 
  currentLength(400), 
  thick(14), 
  increments(1),
  interval(100),
  deltaTime(0),
  RED( SDL_MapRGB(screen->format, 0x00, 0x00, 0xff) ),
  GRAY( SDL_MapRGB(screen->format, 0xff, 0x00, 0x00) ),
  BLACK( SDL_MapRGB(screen->format, 0x00, 0x00, 0x00) ),
  color(RED) {
}

Fuel::Fuel(int sx, int sy, int tl, int cl,
               float t, int inc, Uint32 c, float sp):
  screen(IOManager::getInstance().getScreen()),
  start(Vector2f(sx, sy)), 
  totalLength(tl), 
  currentLength(cl), 
  thick(t), 
  increments(inc),
  interval(sp),
  deltaTime(SDL_GetTicks()),
  RED( SDL_MapRGB(screen->format, 0xff, 0x00, 0x00) ),
  GRAY( SDL_MapRGB(screen->format, 0xff, 0xff, 0xff) ),
  BLACK( SDL_MapRGB(screen->format, 0x00, 0x00, 0x00) ),
    color(c) {
}

void Fuel::drawBox() const {

	//Draw_AALine(screen,0, 0,1280, 0, 100, BLACK);
	Draw_AALine(screen,0, 0,1920, 0, 100, BLACK);

  Draw_AALine(screen, start[0], start[1],  start[0]+totalLength, start[1], thick, GRAY);
 
  //Draw_AALine(screen, 1280-totalLength-100, start[1],  1280-100, start[1], thick, GRAY);
  
  
  // Two Horizontal lines
  Draw_AALine(screen, start[0], start[1]-8, 
                      start[0]+totalLength, start[1]-8, 
                      1.0, BLACK);
  Draw_AALine(screen, start[0], start[1]+8, 
                      start[0]+totalLength, start[1]+8, 
                      1.0, BLACK);
  // Two Vertical lines
  Draw_AALine(screen, start[0]-1, start[1]-8, 
                      start[0]-1, start[1]+8, 
                      2.0, BLACK);
  Draw_AALine(screen, start[0]+totalLength+1, start[1]-8, 
                      start[0]+totalLength+1, start[1]+8, 
                      2.0, BLACK);
}

void Fuel::draw() const {
  drawBox();
  
  Draw_AALine(screen, start[0], start[1], 
                      start[0]+currentLength, start[1], 
                      thick, color);
}
void Fuel::update(Uint32 ticks) {
  deltaTime += ticks;
  if ( currentLength > 0 && deltaTime > interval ) {
    deltaTime = 0;
    currentLength -= increments;
  }
}
