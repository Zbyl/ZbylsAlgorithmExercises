#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

int fib(int n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    int fm2 = 0; // f(i - 2)
    int fm1 = 1; // f(i - 1)
    for (int i = 2; i <= n; ++i)
    {
        int f = fm1 + fm2; // f(i)
        fm2 = fm1;
        fm1 = f;
    }

    return fm1;
}

int main() {
#ifdef ZBYL
    freopen("rosalind_fibo.txt", "r", stdin);
    freopen("rosalind_fibo.out", "w", stdout);
#endif

    int N; // find n-th fib number

    std::cin >> N;

    int result = fib(N);

    std::cout << result;

    return result;
}
