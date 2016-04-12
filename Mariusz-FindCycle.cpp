#include<iostream>

using namespace std;

int N, M;
int V[101][1000];
int vN[101]; //edges count for vertex
bool visited[101];
bool verticesCycle[101];
bool visitedInCurrent[101];
int cycleVertexNum;
bool addVerticesToCycle;

//returns 0 if no cycle
//returns vertexNum where cycle found
bool dfsFindCycle(int vertexNum)
{
    if (visitedInCurrent[vertexNum])
    {
        verticesCycle[vertexNum] = true;
        cycleVertexNum = vertexNum;
        addVerticesToCycle = true;
        return true;
    }

    visited[vertexNum] = true;
    visitedInCurrent[vertexNum] = true;

    for (int i = 0; i < vN[vertexNum]; ++i)
    {
        if (dfsFindCycle(V[vertexNum][i]))
        {
            if (vertexNum == cycleVertexNum)
            {
                addVerticesToCycle = false;
            }

            if (addVerticesToCycle)
            {
                verticesCycle[vertexNum] = true;
            }
            return true;
        }
    }

    visitedInCurrent[vertexNum] = false;

    return 0;
}

int main(int argc, char** argv)
{
    ios_base::sync_with_stdio(false);

    int test_case;
    /*
    The freopen function below opens input.txt in read only mode and
    sets your standard input to work with the opened file.
    When you test your code with the sample data, you can use the function
    below to read in from the sample data file instead of the standard input.
    So. you can uncomment the following line for your local test. But you
    have to comment the following line when you submit for your scores.
    */

    freopen("sample_input.txt", "r", stdin);

    for (test_case = 1; test_case <= 10; ++test_case)
    {
        int i;

        /*
        Read each test case from standard input.
        */

        for (i = 0; i <= N; ++i)
        {
            vN[i] = 0;
            visited[i] = false;
            verticesCycle[i] = false;
            visitedInCurrent[i] = false;
        }
        cycleVertexNum = 0;
        addVerticesToCycle = false;

        cin >> N >> M;
        int x;
        int y;
        for (i = 0; i < M; i++)
        {
            //cin >> A[i] >> B[i];
            cin >> x >> y;
            V[x][vN[x]++] = y;
        }


        /////////////////////////////////////////////////////////////////////////////////////////////
        /*
        Please, implement your algorithm from this section.
        */
        /////////////////////////////////////////////////////////////////////////////////////////////

        bool foundCycle = false;
        for (i = 1; i <= N; ++i)
        {
            if (!visited[i] && vN[x] > 0)
            {
                if (dfsFindCycle(i /*vertex num*/))
                {
                    foundCycle = true;
                    break;
                }
                
            }
        }


        // Print the answer to standard output(screen). 
        cout << "#" << test_case;
        if (!foundCycle)
        {
            cout << " 0" << endl;
        }
        else
        {
            for (i = 1; i <= N; ++i)
            {
                if (verticesCycle[i])
                {
                    cout << " " << i;
                }
            }
            cout << endl;
        }
    }

    return 0;//Your program should return 0 on normal termination.
}
