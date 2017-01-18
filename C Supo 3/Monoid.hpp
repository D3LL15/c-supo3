//
//  Monoid.hpp
//  C Supo 3
//
//  Created by Daniel Ellis on 15/01/2017.
//  Copyright © 2017 Daniel Ellis. All rights reserved.
//

#ifndef Monoid_hpp
#define Monoid_hpp

#include <stdio.h>
#include <vector>

namespace de300 {
    template <class T, int index> class Monoid {
    public:
        
        Monoid<T,index> (*append) (Monoid<T,index>, Monoid<T,index>);
        T identity;
        T value;
        
        Monoid<T,index> (Monoid<T,index> (*app) (Monoid<T,index>, Monoid<T,index>), T ident) {
            append = app;
            identity = ident;
            value = T(ident);
        }
        
        Monoid<T,index> concat (std::vector<Monoid<T,index>> v) {
            typename std::vector<Monoid<T,index>>::iterator viter = v.begin();
            Monoid<T,index> result = *viter;
            viter++;
            for (; viter < v.end(); ++viter) {
                result = append(result, *viter);
            }
            return result;
        }
        
    };
}




#endif /* Monoid_hpp */
