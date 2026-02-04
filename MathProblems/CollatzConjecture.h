#pragma once

#include "Common.h"

class CollatzConjecture
{
public:
    static int CalculateCollatzConjecture(int n)
    {
        if (Common::isEven(n))
        {
            n = n / 2;
        }
        else
        {
            n = 3 * n + 1;
        }

        return n;
    }

    CollatzConjecture()
    {
        int num = 2;
        int i = 0;
        cout << "Enter the starting number : ";
        cin >> num;
    
        while (num != 1 && num != -5 && num != -1 && num != -17) // Stop Loops from happening
        {
            cout << num << '\n';
            num = CalculateCollatzConjecture(num);
            i++;
        }
    
        cout << num << '\n';
        cout << "Number of steps : " << i << '\n';
    }
};
