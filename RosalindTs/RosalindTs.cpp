#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#include "TopSort.h"

int main()
{
#ifdef ZBYL
    freopen("rosalind_ts.txt", "r", stdin);
    freopen("rosalind_ts.out", "w", stdout);
#endif

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
    bool canBeSorted = topSort(neighbourListGraph, sorted);

    if (!canBeSorted)
        std::cout << "Can't be sorted!";
    else
    for (auto v : sorted)
    {
        std::cout << v + 1 << " ";
    }

    return 0;
}
