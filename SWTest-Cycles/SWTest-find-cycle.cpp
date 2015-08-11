
// Zadanie z SWTestu:
//
// Sprawd�, czy graf zawiera jakie� cykle. Je�li zawiera wi�cej ni� jeden, wybierz jeden i go wypisz.
// Dane:
// Ilo�� test case��w T=10
// Ilo�� wierzcho�k�w grafu 5 <= N <= 100
// Ilo�� kraw�dzi 0 <= M <= 1000
// 
// Graf jest skierowany.
// Nie istniej� dwie kraw�dzie o takim samym pocz�tku i ko�cu.
// 
// Input:
// N M
// E1P E1K E2P E2K...
// 
// Czyli ka�dy test case sk�ada si� z dw�ch linii. W pierwszej jest N i M, a w drugiej linii s� kraw�dzie. Ka�d� z kraw�dzi opisuj� dwie cyfry. E1P � pocz�tek kraw�dzi 1, E1K � koniec kraw�dzi 1, etc. 
// 
// Output:
// #X lista wierzcho�k�w tworz�cych wybrany cykl
// #X lista wierzcho�k�w tworz�cych wybrany cykl
// Gdzie X to numer test case�a
// Lista wierzcho�k�w tworz�cych cykl ma by� posortowana rosn�co. Numery wierzcho�k�w rozdzielone spacjami.
// Je�li graf nie zawiera cyklu, zamiast listy wierzcho�k�w wypisz 0.


#include<iostream>

using namespace std;

int N, M;
int AnswerN;
bool AnswerSorted[100];

struct Node
{
    Node()
    {
        clear();
    }

    void clear()
    {
        numNeighbours = 0;
        visited = false;
        onCurrentPath = false;
    }

    void AddEdge(int other)
    {
        neighbours[numNeighbours] = other;
        numNeighbours++;
    }

    int numNeighbours;
    bool visited;
    bool onCurrentPath;
    int neighbours[1000];
};

Node graph[100];

int dfsFromNode(int nodeIdx, int from)
{
    Node& node = graph[nodeIdx];

    if (node.onCurrentPath) // we have a cycle
    {
        return nodeIdx;
    }

    if (node.visited)
    {
        return -1;
    }

    node.visited = true;
    node.onCurrentPath = true;

    for (int i = 0; i < node.numNeighbours; ++i)
    {
        int other = node.neighbours[i];
        int stopOnNode = dfsFromNode(other, nodeIdx);
        if (stopOnNode == -2)
        {
            return -2;
        }

        if (stopOnNode >= 0)
        {
            AnswerSorted[nodeIdx] = true;
            AnswerN++;

            if (stopOnNode == nodeIdx)
                return -2;

            return stopOnNode;
        }
    }

    node.onCurrentPath = false;

    return -1;
}

bool fullDfs()
{
    int nextToDfs = 0;
    while (true)
    {
        while (graph[nextToDfs].visited)
        {
            nextToDfs++;
            if (nextToDfs >= N)
                return false;
        }

        int result = dfsFromNode(nextToDfs, nextToDfs);
        if (result != -1)
        {
            return true;
        }
    }
}

int main(int argc, char** argv)
{
    int test_case;
    /* 
    The freopen function below opens input.txt in read only mode and 
    sets your standard input to work with the opened file. 
    When you test your code with the sample data, you can use the function
    below to read in from the sample data file instead of the standard input.
    So. you can uncomment the following line for your local test. But you
    have to comment the following line when you submit for your scores.
    */

#ifdef ZBYL
    freopen("SWTest-find-cycle-big-test.txt", "r", stdin);
    //freopen("SWTest-find-cycle-small-test.txt", "r", stdin);
#endif

    for(test_case = 1; test_case <= 10; ++test_case)
    {
        /*
            Read each test case from standard input.
        */
    
        for (int i = 0; i < 100; ++i)
            graph[i].clear();

        AnswerN = 0;
        for (int i = 0; i < 100; ++i)
            AnswerSorted[i] = false;

        cin >> N >> M;
        for(int i = 0; i < M; i++)
        {
            int s, t;
            cin >> s >> t;
            s--;
            t--;
            graph[s].AddEdge(t);
        }


        /////////////////////////////////////////////////////////////////////////////////////////////
        /*
            Please, implement your algorithm from this section.
        */
        /////////////////////////////////////////////////////////////////////////////////////////////

        bool result = fullDfs();

        // Print the answer to standard output(screen). 
        cout << "#" << test_case;
        if(AnswerN == 0)
        {
            cout << " 0" << endl;
        }
        else
        {
            for(int i = 0; i < 100; i++)
            {
                if (AnswerSorted[i])
                    cout << " " << i + 1;
            }
            cout << endl;
        }
    }

    return 0; //Your program should return 0 on normal termination.
}
