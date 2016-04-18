/*
You should use the statndard input/output

in order to receive a score properly.

Do not use file input and output

Please be very careful. 
*/

#include <iostream>
#include <utility>
#include <algorithm>

using namespace std;

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

        int N;
        cin >> N;

        int stairs[301];
        stairs[0] = 0;
        for (int i = 0; i < N; ++i)
        {
            cin >> stairs[i + 1];
        }

        int costTwo[301];
        int costOneOne[301];
        int costTwoOnes[301];

        costTwo[0] = 0;
        costOneOne[0] = 0;
        costTwoOnes[0] = 0;

        auto bestCost = [&](int s)
        {
            return std::max( std::max(costTwo[s], costOneOne[s]), costTwoOnes[s] );
        };

        for (int i = 1; i <= N; ++i)
        {
            if (i > 1)
                costTwo[i] = stairs[i] + bestCost(i-2);
            else
                costTwo[i] = -1000;
            costOneOne[i] = stairs[i] + costTwo[i-1];
            costTwoOnes[i] = stairs[i] + costOneOne[i-1];
        }

        /////////////////////////////////////////////////////////////////////////////////////////////
        int Answer = bestCost(N);
        
        // Print the answer to standard output(screen).
        cout << "Case #" << test_case+1 << endl;
        cout << Answer << endl;
    }

    return 0;//Your program should return 0 on normal termination.
}
