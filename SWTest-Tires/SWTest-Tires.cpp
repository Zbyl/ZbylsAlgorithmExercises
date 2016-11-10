// SWTest-Tires.cpp : Defines the entry point for the console application.
//

#include <iostream>

const int MAX_N = 8;

int N;
int K;
int inflates[MAX_N];
int deflates[MAX_N];

int perm[MAX_N];
bool taken[MAX_N];

int testPermutation()
{
    int maxPressure = 0;
    int minPressure = 0;
    int currentPressure = 0;

    for (int i = 0; i < N; ++i)
    {
        int idx = perm[i];

        currentPressure += inflates[idx];
        if (currentPressure > maxPressure)
            maxPressure = currentPressure;

        currentPressure -= deflates[idx];
        if (currentPressure < minPressure)
            minPressure = currentPressure;
    }

    if (maxPressure - minPressure > K)
        return INT_MAX;

    return -minPressure;
}

int bestResult;

void permutationDfs(int depth)
{
    if (depth == N)
    {
        int result = testPermutation();

    #if 0
        std::cout << "Permutation: ";
        for (int i = 0; i < N; ++i)
            std::cout << perm[i] << " ";

        std::cout << "result: " << result << std::endl;
    #endif

        if ((result != -1) && (result < bestResult))
            bestResult = result;

        return;
    }

    for (int i = 0; i < N; ++i)
    {
        if (taken[i])
            continue;

        taken[i] = true;
        perm[depth] = i;
        permutationDfs(depth + 1);
        taken[i] = false;
    }
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
        std::cin >> N >> K;

        for (int i = 0; i < N; ++i)
            std::cin >> inflates[i];

        for (int i = 0; i < N; ++i)
            std::cin >> deflates[i];

        bestResult = INT_MAX;
        for (int i = 0; i < N; ++i)
            taken[i] = false;

        permutationDfs(0);

        if (bestResult == INT_MAX)
            std::cout << "#" << (t + 1) << " " << -1 << std::endl;
        else
            std::cout << "#" << (t + 1) << " " << bestResult << std::endl;
    }

    return 0;
}

