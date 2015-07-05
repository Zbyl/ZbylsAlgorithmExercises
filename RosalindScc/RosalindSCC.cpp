#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#include "NeighbourListGraph.h"
#include "SCC-Tarjan.h"
#include "SCC-Kosaraju.h"

int main() {
#ifdef ZBYL
    freopen("rosalind_scc.txt", "r", stdin);
    freopen("rosalind_scc.out", "w", stdout);
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

        neighbourListGraph.addDirectedEdge( Edge(p, k) );
    }

    std::vector< std::vector<int> > sccGroupsT = SCCTarjan(neighbourListGraph);
    std::vector< std::vector<int> > sccGroupsK = SCCKosaraju(neighbourListGraph);

    assert(sccGroupsT.size() == sccGroupsK.size());

    std::cout << sccGroupsT.size() << std::endl;

    return 0;
}
