#pragma once

#include <condition_variable>
#include <mutex>
#include <vector>

class PCQueue
{
public:
  explicit PCQueue(size_t size_);

  void consume();
  void produce();
  size_t maxSize() const;
  size_t currentSize();

private:
  const size_t            _size;
  std::condition_variable _cvEmpty{};
  std::condition_variable _cvFull{};
  std::mutex              _mutex;
  std::vector<int>        _container;
};
