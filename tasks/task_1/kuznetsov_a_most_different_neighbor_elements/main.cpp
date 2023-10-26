#include <iostream>
#include "most_different.hpp"

int main(int argc, char** argv) {

  std::vector<int> elems{42, 1, 40, 1, 40};
  std::cout << findMostDifferent(elems, argc, argv) << '\n';

  return 0;
}