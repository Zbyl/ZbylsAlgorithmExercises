#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#include "IncidenceMatrixGraph.h"

/// @brief Returns true if graph contains 4 edge cycle containing edge u-v.
bool findCycle(IncidenceMatrix& graph, int u, int v)
{
    for (int i = u + 1; i < graph.numberOfNodes; ++i)
        for (int j = v + 1; j < graph.numberOfNodes; ++j)
        {
            if (
                   graph.hasDirectedEdge(i, v)
                && graph.hasDirectedEdge(i, j)
                && graph.hasDirectedEdge(j, u)
                //&& graph.hasDirectedEdge(u, v) - always true here
                )
                return true;
        }

    return false;
}

bool findCycles(IncidenceMatrix& graph)
{
    for (int i = 1; i < graph.numberOfNodes; ++i)
        for (int j = i + 1; j < graph.numberOfNodes; ++j)
        {
            if (graph.hasDirectedEdge(i, j))
                if (findCycle(graph, i, j))
                    return true;
        }
    return false;
}

int main() {
#ifdef ZBYL
    freopen("rosalind_sq2.txt", "r", stdin);
    freopen("rosalind_sq.out", "w", stdout);
#endif

    int T; // graphs count

    std::cin >> T;

    for (int t = 0; t < T; ++t)
    {
        int N; // node count
        int M; // edge count

        std::cin >> N;
        std::cin >> M;

        IncidenceMatrix graph(N);

        for (int i = 0; i < M; ++i)
        {
            int p, k;
            std::cin >> p >> k;
            p--;
            k--;

            graph.addBidirectionalEdge(Edge(p, k));
        }

        bool hasSquare = findCycles(graph);

        std::cout << (hasSquare ? 1 : -1) << " ";
    }

    return 0;
}
