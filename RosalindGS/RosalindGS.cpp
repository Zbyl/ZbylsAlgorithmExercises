#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#include "SP-DistanceProduct.h"

/// @brief For given weight matrix returns a node from which all other nodes are reachable, or -1 if no such node exists.
template<typename C>
int findGeneralSink(const WeightMatrix<C>& weightMatrix)
{
    for (int i = 0; i < weightMatrix.numberOfNodes; ++i)
    {
        bool allNodesReachable = true;
        for (int j = 0; j < weightMatrix.numberOfNodes; ++j)
        {
            if (weightMatrix.getWeight(i, j) == std::numeric_limits<C>::max())
            {
                allNodesReachable = false;
                break;
            }
        }

        if (allNodesReachable)
        {
            return i;
        }
    }

    return -1;
}

int main() {
#ifdef ZBYL
    freopen("rosalind_gs.txt", "r", stdin);
    freopen("rosalind_gs.out", "w", stdout);
#endif

    int T; // graph count
    std::cin >> T;

    for (int t = 0; t < T; ++t)
    {
        int N; // node count
        int M; // edge count

        std::cin >> N;
        std::cin >> M;

        WeightMatrix<int> weightMatrix(N);

        for (int i = 0; i < M; ++i)
        {
            int p, k;
            std::cin >> p >> k;
            p--;
            k--;

            weightMatrix.setWeight(p, k, 1);
        }

        /// @note This is a stupid solution.
        ///       Better one: fin all strongly connected components.
        ///       Run dfs from all of them.
        ///       O(|V| * (|V| + |M|)) instead of O(|V|^3)
        ///       I'm sure we can do better.
        WeightMatrix<int> allDistsFloyd = allDistancesFloydWarshall(weightMatrix);

        //WeightMatrix<int> allDistsNaive = allDistancesNaive(weightMatrix);
        //assert(allDistsNaive.weightMatrix == allDistsFloyd.weightMatrix);

        int generalSink = findGeneralSink(allDistsFloyd);
        std::cout << ( (generalSink == -1) ? -1 : (generalSink + 1) ) << " ";
    }

    return 0;
}
