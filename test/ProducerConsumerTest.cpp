#include "../lib/Producer.h"
#include "../lib/Consumer.h"
#include <mutex>
#include <condition_variable>
#include <gtest/gtest.h>
#include <random>
#include <thread>

struct ProducerConsumerTest : public ::testing::Test
{
  std::condition_variable _cvEmpty;
  std::condition_variable _cvFull;
  std::mutex              _mutex;
};

TEST_F(ProducerConsumerTest, basicTest)
{
  const int MaxContainerSize{2};
  const int MaxThreadCount{100};

  Producer::ContainerType container;
  Producer producer{container, _cvEmpty, _cvFull, _mutex, MaxContainerSize};
  Consumer consumer{container, _cvEmpty, _cvFull, _mutex};

  int produceCount = 0;
  int consumeCount = 0;

  std::vector<std::thread> threadVec;
  std::mt19937 randomNumGenerator;
  randomNumGenerator.seed(std::random_device{}());

  std::uniform_int_distribution<std::mt19937::result_type> dist(0,1);

  for (int index = 0; index < MaxThreadCount; ++index)
  {
    if (dist(randomNumGenerator) == 0)
    {
      threadVec.emplace_back(&Consumer::consume, consumer);
      consumeCount++;
    }
    else
    {
      threadVec.emplace_back(&Producer::produce, producer);
      produceCount++;
    }
  }

  std::cout << "Log: produceCount: " << produceCount << ", consumeCount: " << consumeCount << std::endl;

  // intentionally sleeping, so that all other threads are done
  std::this_thread::sleep_for(std::chrono::seconds{1});

  const int overConsumed = (consumeCount - produceCount);
  if (overConsumed > 0)
  {
    EXPECT_TRUE(container.empty());
    std::cout << "Log: Adding  " << overConsumed  << " producer threads to let consumers finish" << std::endl;

    for (int index = 0; index < overConsumed; ++index)
    {
      threadVec.emplace_back(&Producer::produce, producer);
      produceCount++;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds{500});
  }
  else // if (produceCount > consumeCount)
  {
    const int overProduced = ((produceCount - consumeCount) - MaxContainerSize);
    if (overProduced > 0)
    {
      std::cout << "Log: Adding " << overProduced << " consumers, so that producers can finish" << std::endl;

      // Container must be at its max capacity
      EXPECT_EQ(container.size(), MaxContainerSize);

      for (int index = 0; index < overProduced; ++index)
      {
        threadVec.emplace_back(&Consumer::consume, consumer);
        consumeCount++;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds{500});
    }
    else
    {
      EXPECT_EQ(produceCount - consumeCount, container.size());
    }
  }

  for (auto& thread : threadVec)
  {
    thread.join();
  }

  int producersMoreThanConsumers = produceCount - consumeCount;
  EXPECT_EQ(((producersMoreThanConsumers) ? producersMoreThanConsumers : 0), container.size());
}

