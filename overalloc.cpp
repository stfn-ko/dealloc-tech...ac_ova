
// How delete[] know how many objects to delete?

#include <iostream>

size_t WORDSIZE; // sizeof(WORDSIZE) == 8
constexpr int n = 10;

class Base
{
public:
    int b_var;
};

int main()
{
    //Base *bp = new Base[n];
 
    char *tmp = (char *)operator new[](WORDSIZE + n * sizeof(Base)); // @Flow: 4 + 10 * 4 --= 44 --> tmp { 0..43 }; 
    
    // @Note: bp is our initial base class array
    Base *bp = (Base *)(tmp + WORDSIZE); // @Flow: Base *bp = (Base *)(tmp[0 + 4]); --> bp {4..43};
    
    *(size_t *)tmp = n; // @Flow: tmp is pointing to tmp[0], we cast it to a size_t and assign n as it's new value (10);

    // @Note: fill-in the bp array with base classes
    for (size_t i = 0; i < n; i++)
    {
        new (bp + i) Base();
    }

    //delete[] bp;

    // @Note: we fetch the rvalue from the address (bp (&x32b) - WORDSIZE (32b)), where we previously sotred n (size of bp arr)
    size_t n = *(size_t *)((char *)bp - WORDSIZE);

    while (n-- != 0)
    {
        (bp + n)->~Base(); 
    }
    operator delete[]((char *)bp - WORDSIZE);

    printf("size of WORDSIZE: %d", sizeof(WORDSIZE));

    return 0;
}