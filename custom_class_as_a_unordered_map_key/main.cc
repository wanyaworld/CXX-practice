#include <unordered_map>
#include <iostream>

using namespace std;

struct Key
{
  std::string first;
  std::string second;
  int         third;

  bool operator==(const Key &other) const
  { return (first == other.first
            && second == other.second
            && third == other.third);
  }
};

namespace std {

  template <>
  struct hash<Key>
  {
    std::size_t operator()(const Key& k) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      // Compute individual hash values for first,
      // second and third and combine them using XOR
      // and bit shifting:

      return ((hash<string>()(k.first)
               ^ (hash<string>()(k.second) << 1)) >> 1)
               ^ (hash<int>()(k.third) << 1);
    }
  };

}

int main() {
	struct Key key;
	struct Key dummy;
	key.first = "f";
	key.second = "s";
	key.third = 3;

	unordered_map<struct Key, bool> mymap;
	mymap[key] = true;
	cout << mymap[key] << endl;
	cout << mymap[dummy] << endl;

	return 0;
}
