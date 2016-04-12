// Zadanie z SWTestu:
//
// Dane jest drzewo. Ka�da kraw�d� ma koszt.
// Wolno przepi�� jedn� kraw�d� w dowolny spos�b, pod warnukiem, �e wynikowy graf b�dzie nadal drzewem.
// Poda� maksymalny koszt �cie�ki prostej (bez powtarzaj�cych si� wierzcho�k�w) jaki jest mo�liwy w grafie oryginalnym lub z przepi�t� kraw�dzi�.

// Dane:
// Liczba test case��w
// Liczba wierzcho�k�w drzewa 2 <= N <= 2000
// N-1 kraw�dzi: koszt, v0, v1 (0 < koszt < 10000)

// Rozwi�zanie:
//
// Dla ka�dej kraw�dzi musimy sprawdzi�, jak� maksymaln� �rednic� mo�emy wygenerowa�, przepinaj�c t� kraw�d�:
// maxDiameter = 0
// for each e in edges
//    maxDiameterThroughE = maxDiameter(lewe poddrzewo e) + |e| + maxDiameter(prawe poddrzewo e)
//    maxDiameter = max(maxDiameter, maxDiameterThroughE)
//
// �rednica drzewa: Robimy DFS i znajdujemy A - najg��bszy wierzcho�ek grafu (koszt kraw�dzi traktujemy jako jej d�ugo��).
//                  Robimy DFS z A i znajdujemy najg��bszy wierzcho�ek - czyli najd�u�sz� �cie�k�. To jest jedna ze �rednic drzewa.

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
    Node()
    {}

    void clear()
    {
        numNeighbours = 0;
    }

    void addNeighbour(int eidx, int k, int cost)
    {
        neighbours[numNeighbours] = Neighbour(eidx, k, cost);
        numNeighbours++;
    }

    int numNeighbours;
    Neighbour neighbours[MAX_NODES-1];
    int pad0, pad1;
};

int numNodes;
Node nodes[MAX_NODES];
int numEdges;
Edge edges[MAX_NODES];

void clear()
{
    for (int i = 0; i < numNodes; ++i)
    {
        nodes[i].clear();
    }
    numNodes = 0;
    numEdges = 0;
}

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

    Node& nodeRef = nodes[node];
    for (int i = 0; i < nodeRef.numNeighbours; ++i)
    {
        Neighbour& nRef = nodeRef.neighbours[i];
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
    freopen("SWTest-tree-diameter.txt", "r", stdin);
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

        numNodes = N;
        clear();
        numNodes = N;
        numEdges = N - 1;

        for (int i = 0; i < N - 1; ++i)
        {
            int cost, p, k;
            std::cin >> cost >> p >> k;
            p--;
            k--;

            edges[i] = Edge(p, k, cost);
            nodes[p].addNeighbour(i, k, cost);
            nodes[k].addNeighbour(i, p, cost);
        }
        
        int answer = costTotal();

        // Print the answer to standard output(screen).
        std::cout << "#" << test_case << " " << answer << std::endl;
    }

    return 0;//Your program should return 0 on normal termination.
}
