
#include <iostream>

const int MAX_N = 5;
const int MAX_H = 500;
const int MAX_V = 15;

extern int N, H, V;
extern int C1, R1, M1;
extern int C2, R2, M2;

extern int TunelCosts[MAX_V];


int computeResult1()
{
    int bestResult = INT_MAX;

    for (int i0 = 0; i0 < V; ++i0)
    {
        int result = TunelCosts[i0];

        if (result < bestResult)
            bestResult = result;
    }

    return bestResult;
}

int computeResult2()
{
    int bestResult = INT_MAX;

    for (int i0 = 0; i0 < V; ++i0)
        for (int i1 = i0 + 2; i1 < V; ++i1)
        {
            int result = TunelCosts[i0] + TunelCosts[i1];
            result += (M1 * M1 + M2 * M2) * (i1 - i0);

            if (result < bestResult)
                bestResult = result;
        }

    return bestResult;
}

int computeResult3()
{
    int bestResult = INT_MAX;

    for (int i0 = 0; i0 < V; ++i0)
        for (int i1 = i0 + 2; i1 < V; ++i1)
            for (int i2 = i1 + 2; i2 < V; ++i2)
            {
                int result = TunelCosts[i0] + TunelCosts[i1] + TunelCosts[i2];
                result += (M1 * M1 + M2 * M2) * (i2 - i0);

                if (result < bestResult)
                    bestResult = result;
            }

    return bestResult;
}

int computeResult4()
{
    int bestResult = INT_MAX;

    for (int i0 = 0; i0 < V; ++i0)
        for (int i1 = i0 + 2; i1 < V; ++i1)
            for (int i2 = i1 + 2; i2 < V; ++i2)
                for (int i3 = i2 + 2; i3 < V; ++i3)
                {
                    int result = TunelCosts[i0] + TunelCosts[i1] + TunelCosts[i2] + TunelCosts[i3];
                    result += (M1 * M1 + M2 * M2) * (i3 - i0);

                    if (result < bestResult)
                        bestResult = result;
                }

    return bestResult;
}

int computeResult5()
{
    int bestResult = INT_MAX;

    for (int i0 = 0; i0 < V; ++i0)
        for (int i1 = i0 + 2; i1 < V; ++i1)
            for (int i2 = i1 + 2; i2 < V; ++i2)
                for (int i3 = i2 + 2; i3 < V; ++i3)
                    for (int i4 = i3 + 2; i4 < V; ++i4)
                    {
                        int result = TunelCosts[i0] + TunelCosts[i1] + TunelCosts[i2] + TunelCosts[i3] + TunelCosts[i4];
                        result += (M1 * M1 + M2 * M2) * (i4 - i0);

                        if (result < bestResult)
                            bestResult = result;
                    }

    return bestResult;
}

int computeResultSimple()
{
    switch(N)
    {
        case 1: return computeResult1();
        case 2: return computeResult2();
        case 3: return computeResult3();
        case 4: return computeResult4();
        case 5: return computeResult5();
    }
}

