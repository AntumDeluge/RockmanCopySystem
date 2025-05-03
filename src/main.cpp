#include "globals.h"
#include "gamestatemanager.h"
#include "servicelocator.h"
#include "timer.h"
#include <stdlib.h>
#ifdef PSP
  #include <pspdisplay.h>
#endif

// Frame regulating code inspired by:
// Aaron (http://forums.wolfire.com/viewtopic.php?f=1&t=16539)
// Disch (http://forums.nesdev.com/viewtopic.php?f=3&t=6107)

#ifdef WIN32
int main(int argc, char* args[]) {
#endif
#ifdef PSP
extern "C" int main(int argc, char* args[]) {
#endif
  GameEngine* gameEngine = new GameEngine();
  ServiceLocator::provideVideoService(gameEngine);
  GameStateManager* pGameStateManager = new GameStateManager();
#ifdef WIN32
  const int knMillisecondsPerSecond = 1000;
  const int knFrameDurations = 3;
  int iCurrentFrameDuration = 0;
  int frameDurations[knFrameDurations] = { 17, 17, 16 };
  Uint32 nextFrameTicks = SDL_GetTicks();
  int nFramesPerSecond = 0;
  Timer framesPerSecondTimer;
  framesPerSecondTimer.start();
#endif
  do {
    gameEngine->handleEvents();
    if (!gameEngine->paused()) {
      Controls controls = gameEngine->getControls();
      pGameStateManager->update(controls);
    }
    pGameStateManager->draw();
#ifdef PSP
    sceDisplayWaitVblankStart();
#endif
    if (gameEngine->flipScreen() == -1) {
      return 1;
    }
#ifdef WIN32
    nextFrameTicks += frameDurations[iCurrentFrameDuration];
    iCurrentFrameDuration++;
    if (iCurrentFrameDuration == knFrameDurations) {
      iCurrentFrameDuration = 0;
    }
    nFramesPerSecond++;
    if (framesPerSecondTimer.getTicks() >= knMillisecondsPerSecond) {
      //char framesPerSecondText[2] = { 0, 0 };
      //itoa(nFramesPerSecond, framesPerSecondText, 10);
      //SDL_WM_SetCaption(framesPerSecondText, 0);
      nFramesPerSecond = 0;
      framesPerSecondTimer.start();
    }
    Uint32 currentTicks = SDL_GetTicks();
    if (currentTicks < nextFrameTicks) {
      do {
        SDL_Delay(1);
        currentTicks = SDL_GetTicks();
      }
      while (currentTicks < nextFrameTicks);
    }
    else {
      nextFrameTicks = SDL_GetTicks();
    }
#endif
  }
  while (!gameEngine->quitRequested());
  delete pGameStateManager;
  return 0;
}
