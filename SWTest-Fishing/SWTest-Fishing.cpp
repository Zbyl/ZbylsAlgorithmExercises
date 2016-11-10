// SWTest-Fishing.cpp : Defines the entry point for the console application.
//

#include <iostream>

const int MAX_N = 60;
const int MAX_F = 20;
int N;
int gates_pos[3];
int gates_guys[3];

void openGate(int pos, int numGuys, int preferredDirection, int* arr)
{
    int numGuysDone = 0;
    int radius = 0;
    while (numGuysDone < numGuys)
    {
        int preferredIdx = pos + radius * preferredDirection;
        int otherIdx = pos - radius * preferredDirection;

        if ( (preferredIdx >= 0) && (preferredIdx < N) && (arr[preferredIdx] == 0) )
        {
            arr[preferredIdx] = radius + 1;
            numGuysDone++;
            continue;
        }

        if ( (otherIdx >= 0) && (otherIdx < N) && (arr[otherIdx] == 0) )
        {
            arr[otherIdx] = radius + 1;
            numGuysDone++;
            continue;
        }

        radius++;
    }
}

int openGates(int gate0, int gate1, int gate2, int preferredDirection)
{
    int arr[MAX_N];

    for (int i = 0; i < N; ++i)
        arr[i] = 0;

    openGate(gates_pos[gate0], gates_guys[gate0], preferredDirection, arr);
    openGate(gates_pos[gate1], gates_guys[gate1], preferredDirection, arr);
    openGate(gates_pos[gate2], gates_guys[gate2], preferredDirection, arr);

    int result = 0;
    for (int i = 0; i < N; ++i)
        result += arr[i];

    return result;
}

int openGates()
{
    int bestResult = INT_MAX;

    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {
            if (x == y)
                continue;

            int z = 3 - x - y;

            for (int dir = -1; dir <= 1; ++dir)
            {
                if (dir == 0)
                    continue;

                int result = openGates(x, y, z, dir);
                if (result < bestResult)
                    bestResult = result;
            }
        }
    }

    return bestResult;
}

int main()
{
#ifdef ZBYL
    freopen("sample_input.txt", "r", stdin);
#endif

    int T;
    std::cin >> T;

    for (int t = 0; t < T; ++t)
    {
        std::cin >> N;
        for (int i = 0; i < 3; ++i)
        {
            std::cin >> gates_pos[i] >> gates_guys[i];
            gates_pos[i]--;
        }

        int minCost = openGates();

        std::cout << "#" << (t + 1) << " " << minCost << std::endl;
    }

    return 0;
}

