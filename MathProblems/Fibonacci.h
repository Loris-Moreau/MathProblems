#pragma once

#include <utility>
#include <boost/multiprecision/cpp_int.hpp>

#include "Common.h"

using boost::multiprecision::cpp_int;

class Fibonacci
{
public:
    // Returns {F(n), F(n+1)}
    std::pair<cpp_int, cpp_int> fib_pair(cpp_int n)
    {
        if (n == 0) return {0, 1};
        
        auto [a, b] = fib_pair(n >> 1);
        
        cpp_int c = a * (2 * b - a);
        cpp_int d = a * a + b * b;
        
        if (n & 1) return {d, c + d};
        else return {c, d};
    }
    cpp_int fibonacci(cpp_int n)
    {
        return fib_pair(n).first;
    }

    /* -- -- -- -- -- */
    
    // Fast doubling Fibonacci modulo m
    // Returns {F(n) % m, F(n+1) % m}
    std::pair<cpp_int, cpp_int> fib_pair_mod(cpp_int n, const cpp_int& m)
    {
        if (n == 0) return {0, 1};

        auto [a, b] = fib_pair_mod(n >> 1, m);

        cpp_int c = (a * ((2 * b - a + m) % m)) % m; // F(2k)
        cpp_int d = (a * a + b * b) % m;            // F(2k+1)

        if (n & 1) return {d, (c + d) % m};
        else return {c, d};
    }
    
    // Public API
    cpp_int fibonacci_mod(cpp_int n, const cpp_int& m)
    {
        return fib_pair_mod(n, m).first;
    }
    
    /* -- -- -- -- -- */
    
    Fibonacci(int amount, bool isMod = false)
    {
        if (isMod)
        {
            cpp_int n = 1'000'000'000'000ULL;   // very large n
            cpp_int m = 1'000'000'007;          // modulo
            std::cout << fibonacci_mod(n, m) << "\n";
        }
        else
        {
            for (cpp_int i = 0; i <= amount; ++i)
            {
                std::cout << "F(" << i << ") = " << fibonacci(i) << '\n';
            }
        }
    }
};
