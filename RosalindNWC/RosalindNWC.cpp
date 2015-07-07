#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#include "NeighbourListGraph.h"
#include "EdgeListGraph.h"
#include "BellmanFord.h"
#include "dfs-bfs.h"

#include <list>
#include <string>

std::list<std::string> ala();
void ela();

int main() {
#ifdef ZBYL
    freopen("rosalind_nwc.txt", "r", stdin);
    freopen("rosalind_nwc.out", "w", stdout);
#endif

    int T; // graph count
    std::cin >> T;

    for (int t = 0; t < T; ++t)
    {
        int N; // node count
        int M; // edge count

        std::cin >> N;
        std::cin >> M;

        NeighbourListGraph< WeightedEdge<int> > graph(N);

        for (int i = 0; i < M; ++i)
        {
            int p, k, w;
            std::cin >> p >> k >> w;
            p--;
            k--;

            graph.addDirectedEdge( WeightedEdge<int>(p, k, w) );
        }

        std::vector<bool> visited(graph.numberOfNodes);

        bool hasNegativeCycle = false;
        int nextNodeToVisit = 0;
        while (true)
        {
            while (nextNodeToVisit < graph.numberOfNodes)
            {
                if (!visited[nextNodeToVisit])
                    break;
                nextNodeToVisit++;
            }
            if (nextNodeToVisit >= graph.numberOfNodes)
            {
                break;
            }

            std::vector<int> dfsNodes = dfsFromNode(nextNodeToVisit, graph, visited, true);
            EdgeListGraph< WeightedEdge<int> > edgeListGraph(graph.numberOfNodes);
            for (auto node : dfsNodes)
            {
                for (auto edge : graph.neighbours[node])
                {
                    edgeListGraph.addEdge(edge);
                }
            }
            std::vector< NodeDistPrev<int> > distPrev = bellmanFord(edgeListGraph, nextNodeToVisit);
            if (distPrev.empty())
            {
                hasNegativeCycle = true;
                break;
            }
        }

        if (hasNegativeCycle)
            std::cout << "1 "; // input graph has a negative cycle
        else
            std::cout << "-1 ";
    }

    return 0;
}
