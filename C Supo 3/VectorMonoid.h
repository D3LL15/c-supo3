//
//  VectorMonoid.h
//  C Supo 3
//
//  Created by Daniel Ellis on 15/01/2017.
//  Copyright © 2017 Daniel Ellis. All rights reserved.
//

#ifndef VectorMonoid_h
#define VectorMonoid_h

#include <vector>
#include "Monoid.hpp"

namespace de300 {
    template <class T> class VectorMonoid {
    public:
        static Monoid<std::vector<T>, 1> vectorAppend (Monoid<std::vector<T>, 1> a, Monoid<std::vector<T>, 1> b) {
            Monoid<std::vector<T>, 1> result = Monoid<std::vector<T>, 1>(a.append, a.identity);
            result.value.reserve(a.value.size() + b.value.size());
            
            result.value.insert(result.value.end(), a.value.begin(), a.value.end());
            result.value.insert(result.value.end(), b.value.begin(), b.value.end());
            
            return result;
        }
    };
}


#endif /* VectorMonoid_h */
