#ifndef SERVICELOCATOR_H
#define SERVICELOCATOR_H

#include "videoservice.h"

class ServiceLocator {
  private:
  static VideoService* m_videoService;

  public:
  static VideoService& getVideoService();
  static void provideVideoService(VideoService* a_videoService);
};

#endif
