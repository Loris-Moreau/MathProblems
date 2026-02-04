#pragma once

#include "Common.h"

class TSP
{
public:
    
    // Func that calculates the total distance of a route (Brute Force)
    static int calculateDistance(const vector<vector<int>>& dist_matrix, const vector<int>& route)
    {
        int totalDistance = 0;
        for (int i = 0; i < (int)route.size() - 1; i++)
        {
            if (dist_matrix[route[i]][route[i + 1]] == INT_MAX) // Check for unreachable path
            {
                return INT_MAX; // Unreachable (stupidly high num)
            }
            totalDistance += dist_matrix[route[i]][route[i + 1]];
        }
        // Check return path
        if (dist_matrix[route[route.size() - 1]][route[0]] == INT_MAX) 
        {
            return INT_MAX;
        }
        totalDistance += dist_matrix[route[route.size() - 1]][route[0]];
        return totalDistance;
    }

    TSP()
    {
        // INT_MAX = unreachable paths
        const vector<vector<int>> dist_matrix =
            {
            {0, 2, 2, INT_MAX, INT_MAX}, // A
            {2, 0, 3, 3, 1},            // B
            {2, 3, 0, 1, 3},           // C
            {INT_MAX, 3, 1, 0, 3},    // D
            {INT_MAX, 1, 3, 3, 0}    // E
            };

        vector<int> cities = {0, 1, 2, 3, 4}; // Cities represented by indices (1=A, 2=B, 3=C, 4=D, 5=E)
        const vector<char> cityNames = {'A', 'B', 'C', 'D', 'E'};
    
        int minDistance = INT_MAX;
        vector<int> bestRoute;

        // Try every possible route by generating permutations
        do
        {
            const int current_distance = calculateDistance(dist_matrix, cities);
            if (current_distance < minDistance)
            {
                minDistance = current_distance;
                bestRoute = cities;
            }
        }
        while (next_permutation(cities.begin() + 1, cities.end())); // Fix the first city to reduce duplicate routes

        // Output the results
        cout << "Minimum distance : " << minDistance << '\n';
        cout << "Best route : ";
        for (const int city : bestRoute)
        {
            cout << cityNames[city] << " ";
        }
        cout << cityNames[bestRoute[0]] << '\n'; // Return to starting city
    }
};
