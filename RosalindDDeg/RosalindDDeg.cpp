#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#include "NeighbourListGraph.h"

int main() {
#ifdef ZBYL
    freopen("rosalind_ddeg.txt", "r", stdin);
    freopen("rosalind_ddeg.out", "w", stdout);
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

    std::vector<size_t> degrees;
    for (auto neighbours : neighbourListGraph.neighbours)
    {
        size_t degSum = 0;

        for (auto outEdge : neighbours)
        {
            degSum += neighbourListGraph.neighbours[outEdge.v].size();
        }

        degrees.push_back(degSum);
    }

    //std::cout << "Node degrees: " << std::endl;
    for (auto d : degrees)
    {
        std::cout << d << " ";
    }
    std::cout << std::endl;

    return 0;
}
