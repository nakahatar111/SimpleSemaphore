#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <chrono>
#include <math.h> 
#include <ctype.h>
#include <cassert>
using namespace std;
using namespace std::chrono;
int count = 0;
pthread_mutex_t mutexCount;
pthread_cond_t condCount;

void *increase(void*){
  pthread_mutex_lock(&mutexCount);
  count ++;
  cout << "count+: "<< count << endl;
  pthread_mutex_unlock(&mutexCount);
  pthread_cond_broadcast(&condCount);
  pthread_exit(NULL);
}

void *decrease(void*){
  pthread_mutex_lock(&mutexCount);
  while(count <= 0){
    pthread_cond_wait(&condCount, &mutexCount);
  }
  count--;
  cout << "count-: "<< count << endl;
  pthread_mutex_unlock(&mutexCount);
  pthread_exit(NULL);
}

int main (int argc, char *argv[]){
  if(argc == 3 && isdigit(*argv[1]) && isdigit(*argv[2])){
    int NUMINCREASE = stoi(argv[1]);
    int NUMDECREASE = stoi(argv[2]);

    auto start = high_resolution_clock::now();
    pthread_cond_init(&condCount, NULL);
    pthread_mutex_init(&mutexCount, NULL);
    pthread_t inc_thread[NUMINCREASE];
    pthread_t dec_thread[NUMDECREASE];

    for(int i = 0; i < NUMDECREASE; i++){
      int err = pthread_create(&dec_thread[i], NULL, decrease, NULL);
      if(err){
        cout << "Error code "<< err << endl; 
        exit(-1);
      }
    }
  
    for(int i = 0; i < NUMINCREASE; i++){
      int err = pthread_create(&inc_thread[i], NULL, increase, NULL);
      if(err){
        cout << "Error code "<< err << endl; 
        exit(-1);
      }
    }

    for (int t = 0; t < NUMDECREASE; t++){
      int retval = pthread_join(dec_thread[t], NULL);
      if(retval != 0){
        printf("Error occurred when joining the thread\n");
        exit(-1);
      }
    }
    for (int t = 0; t < NUMINCREASE; t++){
      int retval = pthread_join(inc_thread[t], NULL);
      if(retval != 0){
        printf("Error occurred when joining the thread\n");
        exit(-1);
      }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time: " << duration.count() << " microseconds" << endl;

    assert(pthread_cond_destroy(&condCount)==0);
    assert(pthread_mutex_destroy(&mutexCount)==0);
    pthread_exit(NULL); //make sure no threads are still running before terminating
  }
  else{
    cout << "Argument is not an int" << endl;
  }
}