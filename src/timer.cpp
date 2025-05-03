#include "timer.h"
#include "SDL.h"

Timer::Timer():
  m_startTicks(0) {}

void Timer::start() {
  m_startTicks = SDL_GetTicks();
}

int Timer::getTicks() {
  int ticks = SDL_GetTicks() - m_startTicks;
  return ticks;
}
