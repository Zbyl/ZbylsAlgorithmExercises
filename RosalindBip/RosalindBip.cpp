#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#include "NeighbourListGraph.h"

bool isBipartite(const NeighbourListGraph<Edge>& neighbourListGraph)
{
    std::vector<int> part(neighbourListGraph.numberOfNodes);   //  0 - not set, 1 - first group, 2 -second group

    // DFS
    int nextToVisit = 0;
    while (nextToVisit < neighbourListGraph.numberOfNodes)
    {
        if (part[nextToVisit] > 0)
        {
            nextToVisit++;
            continue;
        }

        std::vector<int> queue;
        queue.push_back(nextToVisit);
        part[nextToVisit] = 1;

        while (!queue.empty())
        {
            int node = queue.back();
            queue.pop_back();

            for (auto edge : neighbourListGraph.neighbours[node])
            {
                int other = edge.v;
                if (part[other] == 0)
                {
                    part[other] = 3 - part[node];
                    queue.push_back(other);
                }
                else
                if (part[other] == part[node])
                {
                    return false;
                }
            }
        }
    }

    return true;
}

int main() {
#ifdef ZBYL
    freopen("rosalind_bip.txt", "r", stdin);
    freopen("rosalind_bip.out", "w", stdout);
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

            neighbourListGraph.addBidirectionalEdge(Edge(p, k));
        }

        bool isBip = isBipartite(neighbourListGraph);
        
        std::cout << (isBip ? 1 : -1) << " ";
    }

    return 0;
}
