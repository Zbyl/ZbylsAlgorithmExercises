#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

int main() {
#ifdef ZBYL
    freopen("rosalind_deg.txt", "r", stdin);
    freopen("rosalind_deg.out", "w", stdout);
#endif

    int N; // node count
    int M; // edge count

    std::cin >> N;
    std::cin >> M;

    std::vector<int> degrees(N, 0);
    for (int i = 0; i < M; ++i)
    {
        int p, k;
        std::cin >> p >> k;
        p--;
        k--;

        degrees[p]++;
        degrees[k]++;
    }

    //std::cout << "Node degrees: " << std::endl;
    for (auto d : degrees)
    {
        std::cout << d << " ";
    }
    std::cout << std::endl;

    return 0;
}
