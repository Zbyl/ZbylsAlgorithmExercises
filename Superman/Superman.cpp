#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

int main() {
#ifdef ZBYL
    freopen("superman.txt", "r", stdin);
#endif

    int N; // number of buildings
    int H; // building height
    int I; // building change penalty

    std::cin >> N;
    std::cin >> H;
    std::cin >> I;

    std::vector< std::vector<int> > P(H); // people on floors: P[floor][building]
    std::vector< std::vector<int> > R(H); // people that can be rescued: R[height][building]

    for (int h = 0; h < H; ++h)
    {
        P[h].resize(N);
        R[h].resize(N);
    }

    for (int b = 0; b < N; ++b)
    {
        int U; // number of people in building
        std::cin >> U;
        for (int u = 0; u < U; ++u)
        {
            int f; // floor on which person resides
            std::cin >> f;
            f--;
            P[f][b]++;
        }
    }

    for (int h = H - 1; h >= 0; --h)
    {
        int bestFromOtherBuildings = 0;
        if (h + I < H)
        for (int b = 0; b < N; ++b)
        {
            bestFromOtherBuildings = std::max(bestFromOtherBuildings, R[h + I][b]);
        }

        for (int b = 0; b < N; ++b)
        {
            int bestFromThisBuilding = (h + 1 < H) ? R[h + 1][b] : 0;
            bestFromThisBuilding += P[h][b];
            R[h][b] = std::max(bestFromThisBuilding, bestFromOtherBuildings + P[h][b]);
        }
    }

    int result = 0;
    for (int b = 0; b < N; ++b)
    {
        result = std::max(result, R[0][b]);
    }

    std::cout << "Rescued: " << result;

    return result;
}
