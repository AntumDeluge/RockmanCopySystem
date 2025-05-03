#ifndef TIMER_H
#define TIMER_H

class Timer {
  private:
  int m_startTicks;

  public:
  Timer();
  int getTicks();
  void start();
};

#endif
