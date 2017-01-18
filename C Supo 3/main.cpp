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
#include <vector>
#include "VectorMonoid.h"

using namespace de300;

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
    std::cout << Factorial<5>::result << "\n";
    std::cout << Factorial<0>::result << "\n";
    std::cout << Factorial<10>::result << "\n";
    
    
    std::cout << "Check primes of 5 and 10:\n";
    std::cout << PrimeChecker<5>::result << "\n";
    std::cout << PrimeChecker<10>::result << "\n";
    
    
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
    
    
    std::cout << "Monoid check with vector monoid :\n";
    VectorMonoid<int> integerVectorMonoid = VectorMonoid<int>();
    Monoid<std::vector<int>, 1> (*append4) (Monoid<std::vector<int>, 1> a, Monoid<std::vector<int>, 1> b);
    append4 = &integerVectorMonoid.vectorAppend;
    std::vector<int> identity1 = std::vector<int>();
    std::vector<int> identity2 = std::vector<int>();
    std::vector<int> identity3 = std::vector<int>();
    Monoid<std::vector<int>, 1> monoid1 = Monoid<std::vector<int>, 1>(append4, identity1);
    Monoid<std::vector<int>, 1> monoid2 = Monoid<std::vector<int>, 1>(append4, identity2);
    Monoid<std::vector<int>, 1> monoid3 = Monoid<std::vector<int>, 1>(append4, identity3);
    
    int i[] = {1,2,3,4,5};
    int j[] = {6,7,8,9,10};
    
    monoid1.value = std::vector<int>(&i[0], &i[5]);
    monoid2.value = std::vector<int>(&j[0], &j[5]);
    monoid3 = monoid1.append(monoid1, monoid2);
    
    for (std::vector<int>::iterator p = monoid3.value.begin(); p < monoid3.value.end(); ++p) {
        std::cout << *p;
    }
    std::cout<<"\n";
    
    
    std::cout << "Monoid concat check with (int,+) :\n";
    Monoid<int, 1> addition31 = Monoid<int, 1>(append, 0);
    addition31.value = 2;
    Monoid<int, 1> addition32 = Monoid<int, 1>(append, 0);
    addition32.value = 3;
    Monoid<int, 1> addition33 = Monoid<int, 1>(append, 0);
    addition33.value = 3;
    Monoid<int, 1> addition34 = Monoid<int, 1>(append, 0);
    
    Monoid<int,1> monoidArray[] = {addition31, addition32, addition33};
    std::vector<Monoid<int,1>> monoidVector = std::vector<Monoid<int,1>>(&monoidArray[0], &monoidArray[3]);
    addition34 = addition31.concat(monoidVector);
    std::cout << addition34.value << "\n";
    
    return 0;
}


