#pragma once

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::cpp_int;

class PollardRho
{
public:
    // Modular multiplication: (a * b) % mod
    cpp_int mod_mul(const cpp_int& a, const cpp_int& b, const cpp_int& mod)
    {
        return (a * b) % mod;
    }

    // Modular exponentiation: (base^exp) % mod
    
    cpp_int mod_pow(cpp_int base, cpp_int exp, const cpp_int& mod)
    {
        cpp_int result = 1;
        base %= mod;
        while (exp > 0)
        {
            if ((exp & 1) != 0) result = (result * base) % mod;
            base = (base * base) % mod;
            exp >>= 1;
        }
        return result;
    }
    
    bool miller_rabin(const cpp_int& n, const std::vector<uint64_t>& bases)
    {
        if (n < 2) return false;
        if (n % 2 == 0) return n == 2;

        cpp_int d = n - 1;
        unsigned int s = 0;
        while ((d & 1) == 0)
        {
            d >>= 1;
            ++s;
        }

        for (uint64_t a_base : bases)
        {
            if (a_base >= n) continue;
            cpp_int x = mod_pow(cpp_int(a_base), d, n);
            if (x == 1 || x == n - 1) continue;

            bool composite = true;
            for (unsigned int r = 1; r < s; ++r)
            {
                x = (x * x) % n;
                if (x == n - 1)
                {
                    composite = false;
                    break;
                }
            }
            if (composite) return false;
        }
        return true;
    }
    
    // f(x) = (x^2 + c) mod n
    cpp_int f(const cpp_int& x, const cpp_int& c, const cpp_int& n)
    {
        return (mod_mul(x, x, n) + c) % n;
    }

    // Pollard's Rho factorization
    cpp_int pollards_rho(const cpp_int& n)
    {
        if (n % 2 == 0) return 2;
        if (n == 1) return 1;
        
        std::srand(std::time(nullptr));
        cpp_int x = std::rand() % n + 1;
        cpp_int y = x;
        cpp_int c = std::rand() % n + 1;
        cpp_int d = 1;
        
        while (d == 1)
        {
            x = f(x, c, n);
            y = f(f(y, c, n), c, n);
            cpp_int diff = x > y ? x - y : y - x;
            d = boost::multiprecision::gcd(diff, n);
        }
        
        if (d == n)
        {
            return pollards_rho(n); // retry with different c
        }
        return d;
    }
    
    cpp_int brent_rho(const cpp_int& n)
    {
        if (n % 2 == 0) return 2;
        std::mt19937_64 gen(std::random_device{}());

        cpp_int y = cpp_int(gen()) % (n - 1) + 1;
        cpp_int c = cpp_int(gen()) % (n - 1) + 1;
        cpp_int m = cpp_int(128);

        cpp_int g = 1, r = 1, q = 1;
        cpp_int x, ys;

        while (g == 1)
        {
            x = y;
            for (cpp_int i = 0; i < r; ++i)
            {
                y = (y * y + c) % n;
            }

            cpp_int k = 0;
            while (k < r && g == 1)
            {
                ys = y;
                cpp_int test = r-k;
                cpp_int lim = std::min(m, test);
                for (cpp_int i = 0; i < lim; ++i)
                {
                    y = (y * y + c) % n;
                    cpp_int diff = x > y ? x - y : y - x;
                    q = (q * diff) % n;
                }
                g = boost::multiprecision::gcd(q, n);
                k += lim;
            }
            r <<= 1;
        }
        if (g == n)
        {
            do
            {
                ys = (ys * ys + c) % n;
                cpp_int diff = x > ys ? x - ys : ys - x;
                g = boost::multiprecision::gcd(diff, n);
            } while (g == 1);
        }
        return g;
    }
    
    void factorize(const cpp_int& n, std::vector<cpp_int>& factors)
    {
        if (n == 1) return;

        static const std::vector<uint64_t> bases64 =
        {
            2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
        };

        if (miller_rabin(n, bases64))
        {
            factors.push_back(n);
            return;
        }

        cpp_int d = brent_rho(n);
        factorize(d, factors);
        factorize(n / d, factors);
    }
    
    PollardRho()
    {
        cpp_int n = 1234567; // big composite
        std::vector<cpp_int> f;
        factorize(n, f);

        std::sort(f.begin(), f.end());
        for (auto& p : f) std::cout << p << "\n";
    }
};