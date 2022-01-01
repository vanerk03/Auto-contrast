#include <bits/stdc++.h>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;


int a[400000];

struct Timer
{
    string msg;
    system_clock::time_point start = high_resolution_clock::now();

    Timer(const string message)
    {
        msg = message;
    }

    void end()
    {
        system_clock::time_point end = high_resolution_clock::now();
        milliseconds duration = duration_cast<milliseconds>(end - start);
        cout << msg << "; Time: " << duration.count() << " ms"
             << "\n";
    }
};

int main() {
    Timer main = Timer("Entire program");
    long long n = 1000008;
    double coef = 0.1;
    long long right = n * (1.0 - 0.1); 
    cout << right << "\n";
    main.end();
    
    return 0;
}