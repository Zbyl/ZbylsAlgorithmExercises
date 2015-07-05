#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#include "SCC-Tarjan.h"
#include "SCC-Graph.h"

int main() {
#ifdef ZBYL
    freopen("rosalind_sc.txt", "r", stdin);
    freopen("rosalind_sc.out", "w", stdout);
#endif

    int T; // graph count
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

        std::vector< std::vector<int> > sccGroupsT = SCCTarjan(neighbourListGraph);
        IncidenceMatrix sccGraph = SCCGraph(neighbourListGraph, sccGroupsT);
        bool semiConnected = isSemiConnectedGraph(neighbourListGraph, sccGraph);

        std::cout << (semiConnected ? 1 : -1 ) << " ";
    }

    return 0;
}
