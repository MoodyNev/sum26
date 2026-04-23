#define _GNU_SOURCE 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
void *fn(void *arg){
    printf("[%d] String: %s, sleeping 3s now...\n", gettid(), arg);
    sleep(3);
    return NULL;
} 

int main(int argc, char **argv){
    pthread_t worker;
    int ret; 
    printf("[%d] Creating worker...\n", gettid());
    ret = pthread_create(&worker, NULL, fn, argv[0]); 
    if (ret != 0) {
        perror("pthread_create failed\n");
        return EXIT_FAILURE;
    }
    
printf("[%d] Waiting for worker thread...\n", gettid()); 
pthread_join(worker, NULL);
printf("[%d] Joined worker thread\n", gettid());

}
