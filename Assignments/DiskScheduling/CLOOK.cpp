#include <iostream>
#include <algorithm>

using namespace std;

bool cmp(int a, int b) {
    return a > b;
}

int main() {
//    freopen("3.in", "r", stdin);
    int S, M, N;
    cin >> S >> M >> N;
    int *track = new int[N + 1];
    for (int i = 0; i < N; ++i) {
        cin >> track[i];
    }
    track[N] = S;
    sort(track, track + N + 1, cmp);
//    for (int i = 0; i < N + 1; ++i) {
//        cout << track[i]<<" ";
//    }
//    cout << endl;
    int index;
    for (index = 0; index < N + 1; ++index) {
        if (track[index] == S) {
            break;
        }
    }
    int x = index;
    int distance = 0;
    while (true) {
        cout << track[x] << " ";
        int x_next = (x + 1) % (N + 1);
        if (x_next == index) {
            break;
        }
        distance += abs(track[x] - track[x_next]);
        x = x_next;
    }
    cout << endl;
    cout << distance;
    delete[] track;
    return 0;
}
