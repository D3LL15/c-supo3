#ifndef FACTORIAL_H
#define FACTORIAL_H

namespace de300 {
    template <int n> class Factorial {
        enum { result = n * Factorial<n - 1>::result };
    }
    
    template <> class Factorial<0> {
        enum { result = 1 };
    }
}


#endif



