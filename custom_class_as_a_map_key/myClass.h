#include <map>

class myClass {
 public:
	myClass();
	~myClass();
	int age;
	char *name;
};
struct ClassCompare {
  bool operator() (const myClass &lhs, const myClass &rhs) const {
    if (lhs.age < rhs.age) return true;
    return false;
  }
};

