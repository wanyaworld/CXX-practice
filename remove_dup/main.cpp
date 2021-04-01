#include <vector>
#include <algorithm>

template <typename T>
void removeDup(std::vector<T>& v) {
  auto end = v.end();
  for (auto it = v.begin() ; it != v.end() ; it++) {
    end = std::remove(it + 1, end, *it);
  }
  v.erase(end, v.end());
}

int main() {
  std::vector<int> v {1, 2, 2, 3, 4};
  removeDup(v);
  return 0;
}
