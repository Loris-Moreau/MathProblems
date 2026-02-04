#pragma once

#include <iostream>
#include <random>

using namespace std;

class Common
{
public:
    static bool isEven(int n)
    {
        if (n % 2 == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    int randomInt()
    {
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator

        // For no range put : std::uniform_int_distribution<> distr(0, RAND_MAX);
        std::uniform_int_distribution<> distrInt(-10, 10); // define the range Int
        int randInt = distrInt(gen); // Generate random Int in range

        return randInt;
    }

    double randDouble()
    {
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator

        std::uniform_real_distribution<> distrDouble(-10.5, 10.5); // define the range Double
        double randDouble = distrDouble(gen); // Generate random Double in range
        
        return randDouble;
    }

    inline uint32_t XOrShift32(uint32_t *state)
    {
        uint32_t x = *state;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        *state = x;
        return x;
    }

    static float roundFloat(float value, int digits = 2)
    {
        int y = (int)pow(10, digits);
        float rounded_down = floorf(value * y) / y;

        return rounded_down;
    }
};
