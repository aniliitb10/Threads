#include <random>

struct TestUtil
{
  static size_t getNumberBetween0And1()
  {
    static std::mt19937 randomNumGenerator;
    randomNumGenerator.seed(std::random_device{}());

    static std::uniform_int_distribution<std::mt19937::result_type> dist(0,1);
    return dist(randomNumGenerator);
  }
};