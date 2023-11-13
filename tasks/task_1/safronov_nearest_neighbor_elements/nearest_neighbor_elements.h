#include <vector>

class VectorProcessor {
public:
  VectorProcessor();
  void findClosestNeighbors(const std::vector<int>& inputVector);

private:
  int rank;
  int size;
};