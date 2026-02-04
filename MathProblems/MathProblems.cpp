#include <iostream>

#include "TSP.h"
#include "CollatzConjecture.h"
#include "AndrewsMonotoneChain.h"
#include "Fibonacci.h"
#include "Meissel-Lehmer.h"
#include "Pollard-Rho.h"

using namespace std;

int main(int argc, char* argv[])
{
    int i = 1, c = 25;

    cout << "Available Functions : " << '\n' 
        << "1: TSP" << '\n' 
        << "2: Collatz Conjecture" << '\n' 
        << "3: Fibonacci Sequence" << '\n' 
        << "3: Meissel Lehmer Algorithm" << '\n' 
        << "5: Pollard's Rho" << '\n' ;
    cout << "Choose : ";
    cin >> i;
    
    switch (i)
    {
    case 1:
        TSP();
        return 0;
    case 2:
        CollatzConjecture();
        return 0;
    case 3:
        cout << "Amount of conjecture : ";
        cin >> c;
        Fibonacci(c, false); 
        return 0;
    case 4:
        Meissel_Lehmer();
        return 0;
    case 5:
        PollardRho();
        return 0;
    default:
        return 0;
    }
    
    return 0;
}
