#pragma once

#include <vector>
#include <condition_variable>
#include <mutex>

class Consumer
{
public:
  using ContainerType = std::vector<int>;

  Consumer(ContainerType& container_, std::condition_variable& cvEmpty_, std::condition_variable& cvFull_, std::mutex& mutex_);

  Consumer(const Consumer& ) = delete;
  Consumer(Consumer&& ) = delete;

  Consumer& operator=(const Consumer& ) = delete;
  Consumer& operator=(Consumer&& ) = delete;

  // Consumes the last element from the container
  void consume();

private:
  ContainerType&           _container;
  std::condition_variable& _cvEmpty;
  std::condition_variable& _cvFull;
  std::mutex&              _mutex;
};
