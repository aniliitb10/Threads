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

  Producer(const Producer& ) = delete;
  Producer(Producer&& ) = delete;

  Producer& operator=(const Producer& ) = delete;
  Producer& operator=(Producer&& ) = delete;

  /* Produces an element in the container and notifies via condition variable */
  void produce();

  static size_t getNumOfInstance();

private:
  static size_t _instanceCounter;

  ContainerType&           _container;
  std::condition_variable& _cvEmpty;
  std::condition_variable& _cvFull;
  std::mutex&              _mutex;
  const int                _maxSize;
};