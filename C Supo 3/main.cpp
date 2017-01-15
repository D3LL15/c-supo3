//
//  main.cpp
//  C Supo 3
//
//  Created by Daniel Ellis on 15/01/2017.
//  Copyright © 2017 Daniel Ellis. All rights reserved.
//

#include <iostream>
#include "Factorial.h"
#include "PrimeChecker.h"
#include "Monoid.hpp"

Monoid<int, 1> intAddApp (Monoid<int, 1> a, Monoid<int, 1> b) {
    Monoid<int, 1> result = Monoid<int, 1> (a.append, a.identity);
    result.value = a.value + b.value;
    return result;
}

Monoid<int, 2> intMultApp (Monoid<int, 2> a, Monoid<int, 2> b) {
    Monoid<int, 2> result = Monoid<int, 2> (a.append, a.identity);
    result.value = a.value * b.value;
    return result;
}

Monoid<double, 1> doubleAddApp (Monoid<double, 1> a, Monoid<double, 1> b) {
    Monoid<double, 1> result = Monoid<double, 1> (a.append, a.identity);
    result.value = a.value + b.value;
    return result;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Factorials of 5, 0 and 10:\n";
    std::cout << de300::Factorial<5>::result << "\n";
    std::cout << de300::Factorial<0>::result << "\n";
    std::cout << de300::Factorial<10>::result << "\n";
    
    std::cout << "Check primes of 5 and 10:\n";
    std::cout << de300::PrimeChecker<5>::result << "\n";
    std::cout << de300::PrimeChecker<10>::result << "\n";
    
    std::cout << "Monoid check with (int,+) :\n";
    Monoid<int, 1> (*append) (Monoid<int, 1> a, Monoid<int, 1> b);
    append = &intAddApp;
    Monoid<int, 1> addition1 = Monoid<int, 1>(append, 0);
    addition1.value = 2;
    Monoid<int, 1> addition2 = Monoid<int, 1>(append, 0);
    addition2.value = 3;
    Monoid<int, 1> addition3 = Monoid<int, 1>(append, 0);
    addition3 = addition1.append(addition1, addition2);
    std::cout << addition3.value << "\n";
    
    std::cout << "Monoid check with (double,+) :\n";
    Monoid<double, 1> (*append2) (Monoid<double, 1> a, Monoid<double, 1> b);
    append2 = &doubleAddApp;
    Monoid<double, 1> addition21 = Monoid<double, 1>(append2, 0.0);
    addition21.value = 2.2;
    Monoid<double, 1> addition22 = Monoid<double, 1>(append2, 0.0);
    addition22.value = 6.5;
    Monoid<double, 1> addition23 = Monoid<double, 1>(append2, 0.0);
    addition23 = addition21.append(addition21, addition22);
    std::cout << addition23.value << "\n";
    
    std::cout << "Monoid check with (int,x) :\n";
    Monoid<int, 2> (*append3) (Monoid<int, 2> a, Monoid<int, 2> b);
    append3 = &intMultApp;
    Monoid<int, 2> mult1 = Monoid<int, 2>(append3, 1);
    mult1.value = 2;
    Monoid<int, 2> mult2 = Monoid<int, 2>(append3, 1);
    mult2.value = 3;
    Monoid<int, 2> mult3 = Monoid<int, 2>(append3, 1);
    mult3 = mult1.append(mult1, mult2);
    std::cout << mult3.value << "\n";
    
    
    

    
    return 0;
}


