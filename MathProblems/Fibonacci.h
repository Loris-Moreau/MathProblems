#pragma once

#include "Common.h"

class Fibonacci
{
public:
    
    // Returns {F(n), F(n+1)}
    std::pair<uint64_t, uint64_t> fib_pair(uint64_t n)
    {
        if (n == 0)
        {
            return {0, 1};
        }
        
        auto [a, b] = fib_pair(n >> 1); // a = F(k), b = F(k+1)
        
        uint64_t c = a * (2 * b - a);   // F(2k)
        uint64_t d = a * a + b * b;     // F(2k+1)
        
        if (n & 1)
        {
            return {d, c + d};          // {F(2k+1), F(2k+2)}
        }
        else
        {
            return {c, d};              // {F(2k), F(2k+1)}
        }
    }
    
    uint64_t fibonacci(uint64_t n)
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
