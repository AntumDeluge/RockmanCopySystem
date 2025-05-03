#include "servicelocator.h"
VideoService* ServiceLocator::m_videoService = 0;


VideoService& ServiceLocator::getVideoService() {
  return *ServiceLocator::m_videoService;
}

void ServiceLocator::provideVideoService(VideoService* a_videoService) {
  m_videoService = a_videoService;
}
