#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#include "NeighbourListGraph.h"
#include "EdgeListGraph.h"
#include "BellmanFord.h"

int main() {
#ifdef ZBYL
    freopen("rosalind_bf.txt", "r", stdin);
    freopen("rosalind_bf.out", "w", stdout);
#endif

    int N; // node count
    int M; // edge count

    std::cin >> N;
    std::cin >> M;

    EdgeListGraph< WeightedEdge<int> > edgeListGraph(N);

    for (int i = 0; i < M; ++i)
    {
        int p, k, w;
        std::cin >> p >> k >> w;
        p--;
        k--;

        edgeListGraph.addEdge( WeightedEdge<int>(p, k, w) );
    }

    std::vector< NodeDistPrev<int> > distPrev = bellmanFord(edgeListGraph, 0);
    if (distPrev.empty())
    {
        std::cout << "Input graph has negative cycle!";
        return 1;
    }

    for (int i = 0; i < edgeListGraph.numberOfNodes; ++i)
    {
        int dist = pathCost(distPrev, i);
        if (dist == -1)
            std::cout << "x ";
        else
            std::cout << dist << " ";
    }

    return 0;
}
