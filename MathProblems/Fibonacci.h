#pragma once

#include "Common.h"

#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::cpp_int;

class Fibonacci
{
public:
    // Returns {F(n), F(n+1)}
    std::pair<cpp_int, cpp_int> fib_pair(cpp_int n)
    {
        if (n == 0)
        {
            return {0, 1};
        }
        
        auto [a, b] = fib_pair(n >> 1);
        
        cpp_int c = a * (2 * b - a);
        cpp_int d = a * a + b * b;
        
        if (n & 1)
        {
            return {d, c + d};
        }
        else
        {
            return {c, d};
        }
    }
    
    cpp_int fibonacci(cpp_int n)
    {
        return fib_pair(n).first;
    }
    
    Fibonacci(int amount)
    {
        for (uint64_t i = 0; i <= amount; ++i)
        {
            std::cout << "F(" << i << ") = " << fibonacci(i) << '\n';
        }
        
    }
};
