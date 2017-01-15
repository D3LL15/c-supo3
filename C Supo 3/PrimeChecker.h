//
//  PrimeChecker.h
//  C Supo 3
//
//  Created by Daniel Ellis on 15/01/2017.
//  Copyright © 2017 Daniel Ellis. All rights reserved.
//

#ifndef PrimeChecker_h
#define PrimeChecker_h

//#define ERRORMACRO _Pragma("GCC error \"error Number must be greater than or equal to 2\"")

#include <math.h>

namespace de300 {
    template <int n, int m> class IsPrime {
        public :
            enum { result = (n % m == 0)? 0 : IsPrime<n, m - 1>::result };
    };
    
    template <int n> class IsPrime<n, 1> {
        public :
            enum { result = 1 };
    };
    
    template <int n> class PrimeChecker {
        public:
            //enum { result = (n < 2)? _Pragma("GCC error \"error Number must be greater than or equal to 2\"") : IsPrime<n, n>::result };
            enum { result = (n >= 2)? IsPrime<n, n - 1>::result : 0};
    };
}

#endif /* PrimeChecker_h */
