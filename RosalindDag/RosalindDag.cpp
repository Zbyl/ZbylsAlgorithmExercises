#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#include "TopSort.h"

int main() {
#ifdef ZBYL
    freopen("rosalind_dag.txt", "r", stdin);
    freopen("rosalind_dag.out", "w", stdout);
#endif

    int T; // graphs count

    std::cin >> T;

    for (int t = 0; t < T; ++t)
    {
        int N; // node count
        int M; // edge count

        std::cin >> N;
        std::cin >> M;

        NeighbourListGraph<Edge> neighbourListGraph(N);

        for (int i = 0; i < M; ++i)
        {
            int p, k;
            std::cin >> p >> k;
            p--;
            k--;

            neighbourListGraph.addDirectedEdge(Edge(p, k));
        }

        std::vector<int> sorted;
        bool isAcyclic = topSort(neighbourListGraph, sorted);
        
        std::cout << (isAcyclic ? 1 : -1) << " ";
    }

    return 0;
}
