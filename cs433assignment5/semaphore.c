#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

void *fun1();
void *fun2();

int shared=1; // shared variable

sem_t s; // semaphore variable

int main(){
    sem_init(&s, 0, 1); // initialize semaphore variable
        // 1st argument = address of variable
        // 2nd argument = number of processes sharing semaphore
        // 3rd argument = initial value of semaphore variable
    
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, fun1, NULL);
    pthread_create(&thread2, NULL, fun2, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("final value of shared is: %d\n", shared); // prints last updated value of shared variable
}

void *fun1() {
    int x;
    sem_wait(&s); // execute wait operation on s
    x = shared; // thread1 reads value of shared variable

    printf("thread1 reads the value of %d\n", x);
    x++;

    printf("local updation by thread1: %d\n", x);
    sleep(1); // thread1 is preempted by thread 2
    shared = x; // thread 1 updates the value of shared variable

    printf("value of shared variable updated by thread1: %d\n", shared);
    sem_post(&s);
}

void *fun2() {
    int y;
    sem_wait(&s); // execute wait operation on s
    y = shared; // thread1 reads value of shared variable

    printf("thread1 reads the value of %d\n", y);
    y--;

    printf("local updation by thread2: %d\n", y);
    sleep(1); // thread2 is preempted by thread 1
    shared = y; // thread 1 updates the value of shared variable

    printf("value of shared variable updated by thread2: %d\n", shared);
    sem_post(&s);
}