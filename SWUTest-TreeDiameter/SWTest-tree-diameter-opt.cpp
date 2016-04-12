// Attempted low-level optimization of original solution.
// Maybe few % faster...

#include <iostream>

const int MAX_NODES = 2000;

struct Edge
{
    Edge()
    {}
    Edge(int p, int k, int cost)
        : p(p)
        , k(k)
        , cost(cost)
    {
    }
    int p, k, cost;
};

struct Neighbour
{
    Neighbour()
    {}

    Neighbour(int eidx, int k, int cost)
        : eidx(eidx)
        , k(k)
        , cost(cost)
    {
    }
    int eidx, k, cost;
};

struct Node
{
    int numNeighbours;
    Neighbour* neighbours;
};

int numNodes;
struct Nums
{
    int numNeighbours;
};
Nums numNeighboursForNode[MAX_NODES];

int numEdges;
Edge edges[MAX_NODES-1];
Node nodes[MAX_NODES];
int numGlobalNeighbours;
Neighbour globalNeighbours[MAX_NODES * MAX_NODES];

struct NodeLevel
{
    int level;
    int node;
};

NodeLevel dfsLowestNode(int node, int level, int forbiddenEdge, int edgeToParent)
{
    NodeLevel nLowestLev;
    nLowestLev.node = node;
    nLowestLev.level = level;

    Node nodeRef = nodes[node];
#if 0
    Neighbour* nP = nodeRef.neighbours;
    while(nP->eidx != -1) 
    {
        Neighbour& nRef = *nP;
        nP++;
#else
    for (int i = 0; i < nodeRef.numNeighbours; ++i)
    {
        Neighbour& nRef = nodeRef.neighbours[i];
#endif
        if (nRef.eidx == edgeToParent)
            continue;
        if (nRef.eidx == forbiddenEdge)
            continue;
        NodeLevel nLev = dfsLowestNode(nRef.k, level + nRef.cost, forbiddenEdge, nRef.eidx);
        if (nLev.level > nLowestLev.level)
        {
            nLowestLev = nLev;
        }
    }

    return nLowestLev;
}

int radius(int fromNode, int forbiddenEdge)
{
    NodeLevel starter = dfsLowestNode(fromNode, 0, forbiddenEdge, -1);
    NodeLevel result = dfsLowestNode(starter.node, 0, forbiddenEdge, -1);
    return result.level;
}

int costForEdge(int eidx)
{
    Edge edge = edges[eidx];

    int costLeft = radius(edge.p, eidx);
    int costRight = radius(edge.k, eidx);

    int result = costLeft + edge.cost + costRight;
    return result;
}

int costTotal()
{
    int maxCost = 0;

    for (int i = 0; i < numEdges; ++i)
    {
        int cost = costForEdge(i);
        if (cost > maxCost)
            maxCost = cost;
    }

    return maxCost;
}

int main(int argc, char** argv)
{
    /* 
    The freopen function below opens sample_input.txt in read only mode and 
    sets your standard input to work with the opened file. 
    When you test your code with the sample data, you can use the function
    below to read in from the sample data file instead of the standard input.
    So. you can uncomment the following line for your local test. But you
    have to comment the following line when you submit for your scores.
    */
#ifdef ZBYL
    freopen("sample_input.txt", "r", stdin);
#endif
    
    /*
    The following code is inserted for increasing input/output processing performance.
    */
    std::ios::sync_with_stdio(false);

    /*
       Read each test case from standard input.
    */
    int T;
    std::cin >> T;
    for(int test_case = 1; test_case <= T; ++test_case)
    {
        /////////////////////////////////////////////////////////////////////////////////////////////
        /*
            Please, implement your algorithm from this section.
        */
        /////////////////////////////////////////////////////////////////////////////////////////////

        int N;
        std::cin >> N;

        numGlobalNeighbours = 0;
        numNodes = N;
        numEdges = N - 1;

        for (int i = 0; i < N; ++i)
        {
            numNeighboursForNode[i].numNeighbours = 0;
        }

        for (int i = 0; i < N - 1; ++i)
        {
            int cost, p, k;
            std::cin >> cost >> p >> k;
            p--;
            k--;

            edges[i] = Edge(p, k, cost);
            numNeighboursForNode[p].numNeighbours++;
            numNeighboursForNode[k].numNeighbours++;
        }

        int runningCount = 0;
        for (int i = 0; i < N; ++i)
        {
            int num = numNeighboursForNode[i].numNeighbours;

            Node& nodeP = nodes[i];
            nodeP.numNeighbours = 0;
            nodeP.neighbours = &globalNeighbours[runningCount];

            runningCount += num;
#if 0
            runningCount += num + 1;
            globalNeighbours[runningCount - 1].eidx = -1;
#endif
        }

        for (int i = 0; i < N - 1; ++i)
        {
            Edge edge = edges[i];

            Node& nodeP = nodes[edge.p];
            Node& nodeK = nodes[edge.k];

            nodeP.neighbours[nodeP.numNeighbours] = Neighbour(i, edge.k, edge.cost);
            nodeK.neighbours[nodeK.numNeighbours] = Neighbour(i, edge.p, edge.cost);
            nodeP.numNeighbours++;
            nodeK.numNeighbours++;
        }

        int answer = costTotal();

        // Print the answer to standard output(screen).
        std::cout << "#" << test_case << " " << answer << std::endl;
    }

    return 0;//Your program should return 0 on normal termination.
}

