#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#include "NeighbourListGraph.h"
#include "SCC-Tarjan.h"

int main() {
#ifdef ZBYL
    freopen("rosalind_cc.txt", "r", stdin);
    freopen("rosalind_cc.out", "w", stdout);
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

        neighbourListGraph.addBidirectionalEdge( Edge(p, k) );
    }

    std::vector< std::vector<int> > sccGroupsT = SCCTarjan(neighbourListGraph);

    std::cout << sccGroupsT.size() << std::endl;

    return 0;
}
