#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#include "NeighbourListGraph.h"
#include "Dijkstra.h"

int main() {
#ifdef ZBYL
    freopen("rosalind_bfs.txt", "r", stdin);
    freopen("rosalind_bfs.out", "w", stdout);
#endif

    int N; // node count
    int M; // edge count

    std::cin >> N;
    std::cin >> M;

    NeighbourListGraph< WeightedEdge<int> > neighbourListGraph(N);

    for (int i = 0; i < M; ++i)
    {
        int p, k;
        std::cin >> p >> k;
        p--;
        k--;

        neighbourListGraph.addDirectedEdge( WeightedEdge<int>(p, k, 1) );
    }

    std::vector< DijkstraDistPrev<int> > distances = dijkstra(neighbourListGraph, 0, -1);

    for (auto dp : distances)
    {
        std::cout << ((dp.dist != std::numeric_limits<int>::max()) ? dp.dist : -1) << " ";
    }

    return 0;
}
