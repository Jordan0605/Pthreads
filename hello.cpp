#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;

void* Hello(void* rank);

int main(int argc, char* argv[]){
    long thread;
    pthread_t* thread_handles;

    thread_count = strtol(argv[1],NULL,10);

    thread_handles = static_cast<pthread_t*>(malloc(thread_count * sizeof(pthread_t)));

    for(thread = 0;thread<thread_count;thread++){
        pthread_create(&thread_handles[thread], NULL, Hello, (void*)thread);
    }
    
    for(thread = 0;thread<thread_count;thread++){
        pthread_join(thread_handles[thread],  NULL);
    }
    printf("Hello from the main thread\n");
    free(thread_handles);
    return 0;

}

void* Hello(void* rank){
    long my_rank = (long)rank;
    printf("Hello from thread %1d of %d\n", my_rank, thread_count);

    return NULL;
}
