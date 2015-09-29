/*
You should use the statndard input/output

in order to receive a score properly.

Do not use file input and output

Please be very careful. 
*/

#include <iostream>

using namespace std;

int luckyNumber(int n, int m)
{
    int bottoms[10];

    int curBottom = 1;
    for (int i = 0; i < n; ++i)
    {
        bottoms[i] = curBottom;
        curBottom *= 2;
    }

    if (bottoms[n-1] > m)
        return 0;

    int validCases = 0;
    for (int k = bottoms[n - 1]; k <= m; ++k)
    {
        int curValue = k / 2;
        int curValidCases = 1;
        for (int i = n - 2; i >= 0; --i)
        {
            if (curValue < bottoms[i])
            {
                curValidCases = 0;
                break;
            }

            curValidCases *= (curValue - bottoms[i] + 1);
            curValue /= 2;
        }

        validCases += curValidCases;
    }

    return validCases;
}

int main(int argc, char** argv)
{
    int T, test_case;
    /*
       The freopen function below opens input.txt file in read only mode, and afterward,
       the program will read from input.txt file instead of standard(keyboard) input.
       To test your program, you may save input data in input.txt file,
       and use freopen function to read from the file when using cin function.
       You may remove the comment symbols(//) in the below statement and use it.
       Use #include<cstdio> or #include <stdio.h> to use the function in your program.
       But before submission, you must remove the freopen function or rewrite comment symbols(//).
     */	

#ifdef ZBYL
    freopen("sample_input.txt", "r", stdin);
#endif

    cin >> T;
    for(test_case = 0; test_case  < T; test_case++)
    {

        /////////////////////////////////////////////////////////////////////////////////////////////

        int N, M;
        cin >> N >> M;

        /////////////////////////////////////////////////////////////////////////////////////////////
        int Answer = luckyNumber(N, M);
        
        // Print the answer to standard output(screen).
        cout << "Case #" << test_case+1 << endl;
        cout << Answer << endl;
    }

    return 0;//Your program should return 0 on normal termination.
}
