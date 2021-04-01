#include <vector>
#include <algorithm>

bool isOdd(int x) {
  return x % 2;
}

void removeOdds(std::vector<int> &v) {
  v.erase(std::remove_if(v.begin(), v.end(), [](int i) { return i % 2; }), v.end());
}

void removeOddsFunc(std::vector<int> &v) {
  v.erase(std::remove_if(v.begin(), v.end(), isOdd), v.end());
}

int main (){
  std::vector<int> v {1, 4, 3, 2, 5};
  removeOdds(v);
  std::vector<int> v2 {1, 4, 3, 2, 5};
  removeOddsFunc(v2);
  return 0;
}
