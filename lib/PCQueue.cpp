#include "PCQueue.h"

PCQueue::PCQueue(size_t size_):
_size(size_)
{
  _container.reserve(_size);
}

void PCQueue::consume()
{
  {
    std::unique_lock<std::mutex> lock{_mutex};
    while(_container.empty())
    {
      _cvEmpty.wait(lock);
    }

    _container.erase(std::prev(std::end(_container)));
  }

  _cvFull.notify_all();
}

void PCQueue::produce()
{
  {
    std::unique_lock<std::mutex> lock{_mutex};
    while (_container.size() >= _size)
    {
      _cvFull.wait(lock);
    }

    _container.push_back(0);
  }

  _cvEmpty.notify_all();
}

size_t PCQueue::maxSize() const
{
  return _size;
}

size_t PCQueue::currentSize()
{
  std::unique_lock<std::mutex> lock{_mutex};
  return _container.size();
}
