#include <iostream>
#include <omp.h>
#include <queue>
#include <unistd.h>   
using namespace std;

#define BUFFER_SIZE 5
queue<int> bufferQ;

omp_lock_t lockVar;

int main() {
    int items = 10;    
    int producers = 2, consumers = 2;

    omp_init_lock(&lockVar);

    cout << "Producer-Consumer Problem using OpenMP" << endl;
 
    #pragma omp parallel num_threads(producers + consumers)
    {
        int tid = omp_get_thread_num();
 
        if (tid < producers) {
            #pragma omp critical(produce_section)
            {
                cout << "Producer " << tid << " started" << endl;
            }

            for (int i = 1; i <= items; i++) {
                while (true) {
                    omp_set_lock(&lockVar);  

                    if (bufferQ.size() < BUFFER_SIZE) {
                        bufferQ.push(i);
                        cout << "Producer " << tid << " produced item " << i
                             << " | Buffer size: " << bufferQ.size() << endl;

                        omp_unset_lock(&lockVar);  
                        break;
                    }

                    omp_unset_lock(&lockVar);
                    usleep(100000); 
                }
            }
        }
 
        else {
            int consumerId = tid - producers;  

            #pragma omp critical(consume_section)
            {
                cout << "Consumer " << consumerId << " started" << endl;
            }

            for (int i = 1; i <= items; i++) {
                while (true) {
                    omp_set_lock(&lockVar);  

                    if (!bufferQ.empty()) {
                        int item = bufferQ.front();
                        bufferQ.pop();
                        cout << "Consumer " << consumerId << " consumed item " << item
                             << " | Buffer size: " << bufferQ.size() << endl;

                        omp_unset_lock(&lockVar);  
                        break;
                    }

                    omp_unset_lock(&lockVar);
                    usleep(100000);  
                }
            }
        }

 
        #pragma omp barrier
    }

    omp_destroy_lock(&lockVar);
    return 0;
}
