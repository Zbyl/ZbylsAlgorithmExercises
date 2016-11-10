// SWTest-Tunels.cpp : Defines the entry point for the console application.
//

#include <iostream>

const int MAX_N = 5;
const int MAX_H = 500;
const int MAX_V = 15;

int N, H, V;
int C1, R1, M1;
int C2, R2, M2;
int S[MAX_V][MAX_H];

int TunelCosts[MAX_V];

// Complexity: O(n^2)
int computeTunnelCostNaive(int v)
{
    int bestCost = INT_MAX;
    for (int meeting = 0; meeting <= H; ++meeting)
    {
        int dist1 = meeting;
        int dist2 = H - meeting;

        int penalty = 0;
        if (dist2 > dist1)
            penalty = (dist2 - dist1 - 1) * R2;
        else
        if (dist2 < dist1)
            penalty = (dist1 - dist2 - 1) * R2;

        int hardness0 = 0;
        for (int i = 0; i < meeting; ++i)
            hardness0 += S[v][i];

        int hardness1 = 0;
        for (int i = meeting; i < H; ++i)
            hardness1 += S[v][i];

        int totalCost = hardness0 * C1 + hardness1 * C2 + penalty;
        if (totalCost < bestCost)
            bestCost = totalCost;
    }

    return bestCost;
}

// Complexity: O(n)
int computeTunnelCost(int v)
{
    int sumHardness = 0;
    for (int i = 0; i < H; ++i)
        sumHardness += S[v][i];

    int bestCost = INT_MAX;
    int hardness0 = 0;
    int hardness1 = sumHardness;
    for (int meeting = 0; meeting <= H; ++meeting)
    {
        int dist1 = meeting;
        int dist2 = H - meeting;

        int penalty = 0;
        if (dist2 > dist1)
            penalty = (dist2 - dist1 - 1) * R2;
        else
        if (dist2 < dist1)
            penalty = (dist1 - dist2 - 1) * R2;

        int totalCost = hardness0 * C1 + hardness1 * C2 + penalty;
        if (totalCost < bestCost)
            bestCost = totalCost;

        if (meeting < H)
        {
            hardness0 += S[v][meeting];
            hardness1 -= S[v][meeting];
        }
    }

    return bestCost;
}


int chosen[MAX_N];

int doDfs(int depth, int minAllowed)
{
    if (depth == N)
    {
        int result = 0;
        for (int i = 0; i < N; ++i)
            result += TunelCosts[chosen[i]];

        result += (M1 * M1 + M2 * M2) * (chosen[N-1] - chosen[0]);

    #if 0
        std::cout << "Order: ";
        for (int i = 0; i < N; ++i)
            std::cout << chosen[i] << " ";

        std::cout << "result: " << result << std::endl;
    #endif

        return result;
    }

    int bestResult = INT_MAX;
    for (int i = minAllowed; i < V; ++i)
    {
        chosen[depth] = i;
        int result = doDfs(depth + 1, i + 2);
        if (result < bestResult)
            bestResult = result;
    }

    return bestResult;
}

int computeResultDfs()
{
    return doDfs(0, 0);
}

int computeResultSimple();
int computeResultConvoluted();

// cost of excavation = S[v][h] * C#
// 1 area per day
// using excavator 2 times in a row costs a penalty: R#
// cost for moving excavators: (M1^2 + M2^2) * D, D - vertical distance must be >= 2

int main()
{
#ifdef ZBYL
    freopen("sample_input.txt", "r", stdin);
#endif

    int T;
    std::cin >> T;

    for (int t = 0; t < T; ++t)
    {
        std::cin >> N >> H >> V;
        std::cin >> C1 >> R1 >> M1;
        std::cin >> C2 >> R2 >> M2;

        for (int v = 0; v < V; ++v)
            for (int h = 0; h < H; ++h)
                std::cin >> S[v][h];

        for (int v = 0; v < V; ++v)
        {
            int cost0 = computeTunnelCost(v);
            int cost1 = computeTunnelCostNaive(v);
            TunelCosts[v] = cost0;

            if (cost0 != cost1)
                std::cout << "Bug: " << v << std::endl;
        }

        int minCost0 = computeResultConvoluted();
        int minCost1 = computeResultSimple();
        int minCost2 = computeResultDfs();

        if (minCost0 != minCost1)
            std::cout << "Bug Simple" << std::endl;

        if (minCost0 != minCost2)
            std::cout << "Bug Dfs" << std::endl;

        std::cout << "#" << (t + 1) << " " << minCost0 << std::endl;
    }

    return 0;
}

