#include <vector>
#include <algorithm>

void removeOdds(std::vector<int> &v) {
  v.erase(std::remove_if(v.begin(), v.end(), [](int i) { return i % 2; }), v.end());
}

int main (){
  std::vector<int> v {1, 4, 3, 2, 5};
  removeOdds(v);
  return 0;
}
