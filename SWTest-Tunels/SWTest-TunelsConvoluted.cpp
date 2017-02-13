
#include <iostream>

const int MAX_N = 5;
const int MAX_H = 500;
const int MAX_V = 15;

extern int N, H, V;
extern int C1, R1, M1;
extern int C2, R2, M2;

extern int TunelCosts[MAX_V];

int computeResultConvoluted()
{
    int bestResult = INT_MAX;

    for (int i0 = 0; i0 < V; ++i0)
    {
        int result0 = TunelCosts[i0];

        if (N > 1)
        for (int i1 = i0 + 2; i1 < V; ++i1)
        {
            int result1 = result0 + TunelCosts[i1];
            result1 += (M1 * M1 + M2 * M2) * (i1 - i0);

            if (N > 2)
            for (int i2 = i0 + 2; i2 < V; ++i2)
            {
                int result2 = result1 + TunelCosts[i2];
                result2 += (M1 * M1 + M2 * M2) * (i2 - i1);

                if (N > 3)
                for (int i3 = i0 + 2; i3 < V; ++i3)
                {
                    int result3 = result2 + TunelCosts[i3];
                    result3 += (M1 * M1 + M2 * M2) * (i3 - i2);

                    if (N > 4)
                    for (int i4 = i0 + 2; i4 < V; ++i4)
                    {
                        int result4 = result3 + TunelCosts[i4];
                        result4 += (M1 * M1 + M2 * M2) * (i4 - i3);

                        if (result4 < bestResult)
                            bestResult = result4;
                    }
                    else
                    if (result3 < bestResult)
                        bestResult = result3;
                }
                else
                if (result2 < bestResult)
                    bestResult = result2;
            }
            else
            if (result1 < bestResult)
                bestResult = result1;
        }
        else
        if (result0 < bestResult)
            bestResult = result0;
    }

    return bestResult;
}
