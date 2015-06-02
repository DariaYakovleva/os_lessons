#include <iostream>
#include <cstdio>

using namespace std;

int main() {
    freopen("large.txt", "w", stdout);
    for (int i = 0; i < 2000000; i++) {
        cout << "i=" << i << endl;
    }
}
