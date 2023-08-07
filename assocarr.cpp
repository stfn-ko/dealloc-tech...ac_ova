
// How delete[] know how many objects to delete?

#include <iostream>
#include <unordered_map>

constexpr int n = 10;

class Base
{
public:
    int b_var;
};

static std::unordered_map<Base*, int> um;

int main()
{
    //Base *bp = new Base[n];
    Base* bp = (Base *) operator new[](n * sizeof(Base));
    
    for (size_t i; i < n; ++i) 
    { 
        new(bp+i) Base();
    }
    
    um[bp] = n;


    //delete[] bp;
    size_t n = um[bp];
    
    while (n-- != 0)
    {
        (bp + n)->~Base();
    }

    operator delete[] (bp);

    return 0;
}