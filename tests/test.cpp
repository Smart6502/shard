#include "ptest.hpp"

TEST("ShardWMTests", "TestBuild") {
  #include <cstdlib>

  std::system("make");
  
  return 0;
} END_TEST;

int main() {
  return RUN_ALL_TESTS;
}
