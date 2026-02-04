#pragma once

#include <unordered_map>
#include <utility>
#include <vector>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include "Common.h"

using boost::multiprecision::cpp_int;
using boost::multiprecision::cpp_bin_float_50;

class Meissel_Lehmer
{
public:
    // --- 1. Small prime sieve ---
    std::vector<uint64_t> sieve(uint64_t limit)
    {
        std::vector<bool> is_prime(limit + 1, true);
        is_prime[0] = is_prime[1] = false;
        
        for (uint64_t i = 2; i * i <= limit; ++i)
        {
            if (is_prime[i])
            {
                for (uint64_t j = i * i; j <= limit; j += i)
                {
                    is_prime[j] = false;
                }
            }
        }
        
        std::vector<uint64_t> primes;
        for (uint64_t i = 2; i <= limit; ++i)
        {
            if (is_prime[i])
            {
                primes.push_back(i);
            }
        }
        
        return primes;
    }
    
    // Custom hash for std::pair<cpp_int, int>
    struct pair_hash
    {
        std::size_t operator()(const std::pair<cpp_int, int>& p) const
        {
            std::size_t h1 = static_cast<std::size_t>(p.first % UINT64_MAX); // reduce cpp_int to size_t
            std::size_t h2 = static_cast<std::size_t>(p.second);
            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2)); // boost-style combine
        }
    };
    
    // --- 2. Phi with memoization ---
    struct PhiMemo
    {
        std::unordered_map<std::pair<cpp_int, int>, cpp_int, pair_hash> cache;

        cpp_int phi(const cpp_int& n, int a, const std::vector<uint64_t>& primes)
        {
            if (a == 0) return n;

            auto key = std::make_pair(n, a);
            auto it = cache.find(key);
            if (it != cache.end()) return it->second;

            cpp_int result = phi(n, a - 1, primes) - phi(n / cpp_int(primes[a - 1]), a - 1, primes);
            cache[key] = result;
            return result;
        }
    };
    
    // --- 3. Integer nth root (floor) ---
    cpp_int integer_root(const cpp_int& n, int k)
    {
        cpp_bin_float_50 x = pow(cpp_bin_float_50(n), cpp_bin_float_50(1) / k);
        cpp_int res = cpp_int(floor(x));
        while (boost::multiprecision::pow(res + 1, k) <= n) ++res;
        while (boost::multiprecision::pow(res, k) > n) --res;
        return res;
    }
    
    std::vector<cpp_int> pi_small; // pi_small[i] = number of primes ≤ i

    void compute_pi_small(const std::vector<uint64_t>& primes, uint64_t limit)
    {
        pi_small.resize(limit + 1);
        uint64_t j = 0;
        for (uint64_t i = 0; i <= limit; ++i)
        {
            while (j < primes.size() && primes[j] <= i) ++j;
            pi_small[i] = j;
        }
    }
    
    // --- 4. Meissel-Lehmer recursive pi(n) ---
    cpp_int pi_lehmer(const cpp_int& n, const std::vector<uint64_t>& primes, PhiMemo& memo)
    {
        if (n < 2) return 0;

        if (n <= cpp_int(pi_small.size() - 1))
        {
            return pi_small[n.convert_to<uint64_t>()];
        }

        cpp_int n_sqrt = integer_root(n, 2);
        cpp_int n_cbrt = integer_root(n, 3);
        cpp_int n_quarter = integer_root(n, 4);

        int a = static_cast<int>(pi_lehmer(n_quarter, primes, memo).convert_to<uint64_t>());

        cpp_int result = memo.phi(n, a, primes) + a - 1;

        for (int i = 0; i < a; ++i)
        {
            cpp_int n_over_p = n / cpp_int(primes[i]);
            if (n_over_p <= cpp_int(pi_small.size() - 1))
            {
                result -= pi_small[n_over_p.convert_to<uint64_t>()];
            }
            else
            {
                result -= pi_lehmer(n_over_p, primes, memo);
            }
        }

        return result;
    }
    
    Meissel_Lehmer()
    {
        cpp_int n = 100000000000; // 10^11
        uint64_t sieve_limit = 1000000; // n^(2/3) heuristic
        auto primes = sieve(sieve_limit);

        compute_pi_small(primes, sieve_limit); // precompute small π(x)

        PhiMemo memo;
        cpp_int result = pi_lehmer(n, primes, memo);

        std::cout << "pi(" << n << ") = " << result << "\n";
    }
};
