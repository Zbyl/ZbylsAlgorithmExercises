#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

#include "NeighbourListGraph.h"


/// @brief Starts DFS from given node.
/// @returns 4 if found cycle of length == 4, 3 if found cycle of length 3, 0 otherwise.
template<typename EdgeType>
int findCyclesFromNode(int node, const NeighbourListGraph<EdgeType>& graph, std::vector<bool>& visited, std::vector<int>& levels, std::vector<int>& possibles)
{
    assert(levels.size() == graph.numberOfNodes);
    assert(visited.size() == graph.numberOfNodes);

    if (visited[node])
        return 0;

    visited[node] = true;

    int bestCycle = 0;

    for (size_t i = 0; i < graph.neighbours[node].size(); ++i)
    {
        EdgeType edge = graph.neighbours[node][i];

        if (levels[edge.v] == 0) // we're not processing next node
        {
            if (possibles[edge.v] == levels[node]) // but we have found cycle of length 3, and we're extending it to 4
            {
                // we have found our cycle
                return 4;
            }
        }

        if (levels[edge.v])
        {
            // We have found a cycle

            int cycleLength = levels[node] - levels[edge.v] + 1;

            // If this cycle is of length 4 then we're done
            if (cycleLength == 4)
                return 4;

            // If this cycle is of length 3 then we might be able to improve it later.
            // This will happen if we will later reach our parent (node's parent) with levels equal to 1 more than it has. (But after leaving parent in this DFS.)
            //           .
            //          /| \ <- we're processing this edge
            //         / |  o <- node
            //        . -. /
            if (cycleLength == 3)
                bestCycle = 3;

            // Otherwise there's no chance this cycle will have length 4. (We're processing simple graphs! No loops, no duplicated edges.)
        }
        else
        {
            levels[edge.v] = levels[node] + 1;
            int cycle = findCyclesFromNode(edge.v, graph, visited, levels, possibles);
            if (cycle == 4)
                return 4;
            
            if (cycle == 3)
            {
                possibles[node] = levels[node] + 1;
            }

            levels[edge.v] = 0;
        }
    }

    return bestCycle;
}

/// @brief Sorts graph topologically.
///        Edge from p to k means that p must be done before k.
/// @note This algorithm doesn't use graph edge costs.
template<typename EdgeType>
bool findCycles(const NeighbourListGraph<EdgeType>& graph)
{
    std::vector<bool> visited(graph.numberOfNodes);
    std::vector<int> levels(graph.numberOfNodes);     // nodes that are *currently* being processed (on stack); levels is equal to DFS depth
    std::vector<int> possibles(graph.numberOfNodes);  // level that we need to reach node on, to extend found cycle of length 3 to cycle of length 4

    int nextToVisit = 0;
    while (nextToVisit < graph.numberOfNodes)
    {
        // if we are in visited node - skip it
        if (visited[nextToVisit])
        {
            nextToVisit++;
            continue;
        }

        levels[nextToVisit] = 1;
        int cycle = findCyclesFromNode(nextToVisit, graph, visited, levels, possibles);
        if (cycle == 4)
            return true;
    }

    return false;
}

int main() {
#ifdef ZBYL
    freopen("rosalind_sq.txt", "r", stdin);
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

        NeighbourListGraph<Edge> neighbourListGraph(N);

        for (int i = 0; i < M; ++i)
        {
            int p, k;
            std::cin >> p >> k;
            p--;
            k--;

            neighbourListGraph.addBidirectionalEdge(Edge(p, k));
        }

        bool hasSquare = findCycles(neighbourListGraph);

        std::cout << (hasSquare ? 1 : -1) << " ";
    }

    return 0;
}
