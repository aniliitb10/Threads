#include <iostream>
#include <thread>

#include "Consumer.h"

Consumer::Consumer(ContainerType& container_, std::condition_variable& cvEmpty_, std::condition_variable& cvFull_, std::mutex& mutex_):
_container(container_),
_cvEmpty(cvEmpty_),
_cvFull(cvFull_),
_mutex(mutex_) {}

void Consumer::consume()
{
  {
    std::unique_lock<std::mutex> lock{_mutex};

    while (_container.empty())
    {
      _cvEmpty.wait(lock);
    }

    _container.erase(std::prev(std::end(_container)));
  }
  _cvFull.notify_all();
}
