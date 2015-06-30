#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

bool isQueenAllowed(const std::vector<int>& queens, int nextColumn)
{
    for (int i = 0; i < queens.size(); ++i)
    {
        if (queens[i] == nextColumn)
            return false;

        int diffRows = queens.size() - i;
        int diffCols = nextColumn - queens[i];

        if (diffRows == diffCols)
            return false;
        if (diffRows == -diffCols)
            return false;
    }

    return true;
}

int main() {

    int Q = 8; // size of the board

    int numSolutions = 0;
    std::vector<int> queens; // queens[i] = pozycja królowej w i-tym wierszu; -1 - nie postawiona
    int nextColumnToTry = 0;
    while (nextColumnToTry < Q)
    {
        bool goodSolution = false;
        for (; nextColumnToTry < Q; ++nextColumnToTry)
        {
            if (isQueenAllowed(queens, nextColumnToTry))
            {
                goodSolution = true;
                break;
            }
        }

        if (goodSolution)
        {
            // good solution - put next queen
            queens.push_back(nextColumnToTry);
            nextColumnToTry = 0;

            if (queens.size() == Q)
            {
                numSolutions++;
                goodSolution = false; // do backtrack
            }
        }

        if (!goodSolution)
        {
            // backtrack

            nextColumnToTry = Q;
            while (nextColumnToTry >= Q)
            {
                if (queens.empty())
                    break;

                nextColumnToTry = queens.back() + 1;
                queens.pop_back();
            }
        }
    }

    std::cout << "Solutions: " << numSolutions;

    return numSolutions;
}
