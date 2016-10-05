#include <pthread.h>
#include <iostream>

using namespace std;

#include "pieces.h"
#include "abstractthread.h"

void * AbstractThread::Callback ( void * arg ) {
    AbstractThread * copy = (AbstractThread *)arg;
    void * status = copy->ThreadWorker((void *)"");
    delete copy;
    pthread_exit ( status );
}

void AbstractThread::Start ( int n ) {
    int limit = (n + threadCounter);
    for(int i = threadCounter; i < limit; i++) {
        if(i == MAX) { break; }
        pthread_create ( &threads[i], NULL, Callback, (void*)this->Copy() );
        threadCounter++;
    }
}

void ** AbstractThread::Wait ( ) {
    void * status = NULL;
    void ** threadResults = new void*[threadCounter];
    for(int i = 0; i < threadCounter; i++) {
        pthread_join ( threads[i], &status );
        threadResults[i] = status;
    }
    return threadResults;
}
