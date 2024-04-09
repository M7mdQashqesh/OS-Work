#include <iostream>
#include <random>
#include <chrono>
#include <vector>
#include <numeric>
#include <thread>
#include <ctime>

using namespace std;
using namespace chrono;

#define ll long long int
#define all(d) d.begin(),d.end()

std::mt19937 gen(static_cast<unsigned int>(time(nullptr)));
uniform_int_distribution<> distr(1, 100);

int GetARandomNumber() {
    return distr(gen);
}

void setVector(vector<ll>& d) {
    for (ll o = 0; o < d.size(); o++)
        d[o] = GetARandomNumber();
}

void sumVectorPart(const vector<ll>& d, ll start, ll end, int threadId) {
    auto startTime = high_resolution_clock::now();
    ll sum = accumulate(d.begin() + start, d.begin() + end, 0LL);
    auto endTime = high_resolution_clock::now();
    duration<double, milli> execTime = endTime - startTime;
    size_t segmentSize = end - start;
    cout << "Thread " << threadId << " - Segment Size: " << segmentSize
        << ", Sum: " << sum << ", Time: " << execTime.count() << " ms" << endl;
}

void multiThreadSum(const vector<ll>& d, ll n, bool symmetric = true) {
    vector<thread> threads;
    vector<size_t> divPoints(n + 1, 0);

    if (symmetric) {
        for (ll o = 0; o <= n; o++) {
            divPoints[o] = d.size() * o / n;
        }
    }
    else {
        ll totalParts = n * (n + 1) / 2;
        for (ll o = 0; o <= n; o++) {
            divPoints[o] = d.size() * (o * (o + 1) / 2) / totalParts;
        }
    }

    for (ll o = 0; o < n; o++) {
        threads.emplace_back(sumVectorPart, cref(d), divPoints[o], divPoints[o + 1], o + 1);
    }

    for (auto& t : threads) {
        t.join();
    }
}

int main() {
    vector<ll> d(1e6, 0);
    setVector(d);

    cout << "Single Thread:" << endl;
    multiThreadSum(d, 1);

    cout << "\nTwo Threads (Symmetric):" << endl;
    multiThreadSum(d, 2, true);

    cout << "\nTwo Threads (Asymmetric):" << endl;
    multiThreadSum(d, 2, false);

    cout << "\nFour Threads (Symmetric):" << endl;
    multiThreadSum(d, 4, true);

    cout << "\nFour Threads (Asymmetric):" << endl;
    multiThreadSum(d, 4, false);

    cout << "\nBashar Herbawi" << endl;
    cout << "Student ID:" << 237611 << endl;
    return 0;
}