#include <iostream>
#include <string>

using namespace std;

class BaseClass {
public:
    BaseClass() {};
    BaseClass(int x) { a = x; }
    BaseClass(int x, std::string s) { a = x; }
    void Do(int x) {};
    int a, b, c;
};

class DerivedClass : public BaseClass {
public:
    template <typename ...Params>
    DerivedClass(Params&&... params) : BaseClass(std::forward<Params>(params)...) {}
};


int main()
{
    DerivedClass dc(10, std::string("hi"));
    std::cout << dc.a << std::endl;
    return 0;
}
