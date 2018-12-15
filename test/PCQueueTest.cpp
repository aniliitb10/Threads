#include "../lib/pcQueue/PCQueue.h"
#include "TestUtil.h"

#include <gtest/gtest.h>
#include <thread>
#include <vector>

struct PCQueueTest : public ::testing::Test
{
  void producerConsumerTest(size_t pcqSize_, size_t threadCount_)
  {
    PCQueue pcq{pcqSize_};

    size_t consumerCount = 0, producerCount = 0;
    std::vector<std::thread> threadVec;

    for (size_t index = 0; index < threadCount_; ++index)
    {
      if (TestUtil::getNumberBetween0And1() == 0)
      {
        threadVec.emplace_back(&PCQueue::produce, std::ref(pcq));
        producerCount++;
      }
      else
      {
        threadVec.emplace_back(&PCQueue::consume, std::ref(pcq));
        consumerCount++;
      }
    }

    // std::cout << "Producers: " << producerCount << ", Consumers: " << consumerCount << std::endl;

    if (consumerCount > producerCount)
    {
      const size_t overConsumed = consumerCount - producerCount;
      // std::cout << "Adding " << overConsumed << " producers to let waiting consumers consume" << std::endl;
      /*EXPECT_EQ(pcq.currentSize(), 0); this needs other threads to finish*/

      for (size_t index = 0; index < overConsumed; ++index)
      {
        threadVec.emplace_back(&PCQueue::produce, std::ref(pcq));
        producerCount++;
      }
    }
    else // if producerCount > consumerCount
    {
      const size_t overProduced = producerCount - consumerCount;
      if (overProduced > pcq.maxSize())
      {
        const size_t producersOverMaxSize = overProduced - pcq.maxSize();
        // std::cout << "Adding " << producersOverMaxSize << " consumers to let waiting producers produce" << std::endl;

        for (size_t index = 0;  index < producersOverMaxSize; ++index)
        {
          threadVec.emplace_back(&PCQueue::consume, std::ref(pcq));
          consumerCount++;
        }
      }
    }

    for (auto& thread : threadVec)
    {
      thread.join();
    }

    // std::cout << "Queue size after all operations: " << pcq.currentSize() << std::endl;
    const size_t overProduced = producerCount - consumerCount;
    EXPECT_EQ(overProduced, pcq.currentSize());
  }

};

TEST_F(PCQueueTest, BasicTest)
{
  producerConsumerTest(5, 10);
  producerConsumerTest(20, 100);
  producerConsumerTest(50, 1000);
}
