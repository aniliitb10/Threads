#include "Producer.h"

Producer::Producer(ContainerType& container_, std::condition_variable& cvEmpty_, std::condition_variable& cvFull_,
                   std::mutex& mutex_, int maxSize_):
  _container(container_),
  _cvEmpty(cvEmpty_),
  _cvFull(cvFull_),
  _mutex(mutex_),
  _maxSize(maxSize_){}

void Producer::produce()
{
  {
    std::unique_lock<std::mutex> lock(_mutex);
    while(_container.size() >= _maxSize)
    {
      _cvFull.wait(lock);
    }

    _container.push_back(0);
  }
  _cvEmpty.notify_all();
}