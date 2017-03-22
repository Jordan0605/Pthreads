#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <thread>

int thread_count = std :: thread :: hardware_concurrency(); ;
int number_of_tosses;
double sum = 0.0;
pthread_mutex_t mutex;

void* Shoot(void* rank);
double fRand(double fMin, double fMax);

int main(int argc, char* argv[]){
    long thread;
    srand(time(NULL));
    pthread_t* thread_handles;

    number_of_tosses = strtol(argv[1],NULL,10);
    if(argv[2] != NULL)
    	thread_count = strtol(argv[2],NULL,10);
    else{
        printf("thread_count default is %d\(follow by how many cores\)\n",thread_count);
        printf("if you want to edit thread_count, please input argv[2]\n");    
    }

    if(thread_count > number_of_tosses){
        thread_count = number_of_tosses;
        printf("warning: your threads are bigger than number of tosses\n");
        printf("system edit thread_count = number_of_tosses automatically\n");
    }

    thread_handles = static_cast<pthread_t*>(malloc(thread_count * sizeof(pthread_t)));
    pthread_mutex_init(&mutex,NULL);
    
    for(thread = 0;thread<thread_count;thread++){
        pthread_create(&thread_handles[thread], NULL, Shoot, (void*)thread);
    }
    
    for(thread = 0;thread<thread_count;thread++){
        pthread_join(thread_handles[thread],  NULL);
    }

    sum = (4.0 * sum) / (double)number_of_tosses;
    printf("%d thread shoot %d times\n",thread_count,number_of_tosses);
    printf("pi = %f\n",sum);
    
    pthread_mutex_destroy(&mutex);
    free(thread_handles);

    return 0;

}

void* Shoot(void* rank){
    long my_rank = (long)rank;
    long long my_tosses = number_of_tosses / thread_count;
    double my_sum = 0.0;

    long long i;
    double x,y;
    double distance_square;
    for(i = 0;i<my_tosses;i++){
        x = fRand(-1.0,1.0);
        y = fRand(-1.0,1.0);
        //printf("\( %f, %f\)\n",x,y);
        distance_square = x * x + y * y;
        if(distance_square < 1 || distance_square == 1)
            my_sum++;
    }
    pthread_mutex_lock(&mutex);                                                    
    sum += my_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

double fRand(double fMin, double fMax){
    
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}
