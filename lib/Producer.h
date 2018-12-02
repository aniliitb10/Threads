#pragma once

#include <vector>
#include <string>
#include <condition_variable>

class Producer
{
public:
  using ContainerType = std::vector<int>;

  Producer(ContainerType& container_, std::condition_variable& cvEmpty_, std::condition_variable& cvFull_,
    std::mutex& mutex_, int maxSize_);

  /* Produces an element in the container and notifies via condition variable */
  void produce();

private:
  ContainerType&           _container;
  std::condition_variable& _cvEmpty;
  std::condition_variable& _cvFull;
  std::mutex&              _mutex;
  const int                _maxSize;
};