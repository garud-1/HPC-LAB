#include <iostream>
#include <omp.h>
#include <vector>
using namespace std;

int main() {
    int n;
    cout << "Enter number of terms for Fibonacci: ";
    cin >> n;

    vector<long long> fib(n);
    fib[0] = 0;
    fib[1] = 1;


    // Parallel computation with synchronization
    #pragma omp parallel for shared(fib) schedule(static)
    for (int i = 2; i < n; i++) {
        // Each thread must wait until fib[i-1] and fib[i-2] are computed
        #pragma omp critical
        {
            fib[i] = fib[i - 1] + fib[i - 2];
             cout << "Thread " << omp_get_thread_num() 
             << " computed fib[" << i << "] = " << fib[i] << endl;
        }
    }

    cout << "Fibonacci Series: ";
    for (int i = 0; i < n; i++) cout << fib[i] << " ";
    cout << endl;

    return 0;
}
