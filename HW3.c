#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For usleep()
#include <time.h>
#include <pthread.h> // For thread creation

#define TIME_SLOT 0.1 // Time slot in seconds (0.1 seconds)
#define WORK_A 10000
#define WORK_B 1000
#define WORK_C 7000
#define ID_A 0.8
#define ID_B 0.5
#define ID_C 0.75

// Global variables
pthread_mutex_t count_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t RRlock = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t count_nonzero = PTHREAD_COND_INITIALIZER;


typedef struct {
    void* (*func)(void* args);
    int work_needs;
    int work_done;
    int complete;
    double work_time;
} thread_t;

void* theA(void* args) {
    pthread_mutex_lock(&count_lock);
    int i=0;
    while (1) {
        printf("Thread A: %d\n",i);
        i++;
        sleep(ID_A);
        if (i>=WORK_A) {
            break;
        }
    }
    pthread_mutex_unlock(&count_lock);
}
void* theB(void* args) {
    pthread_mutex_lock(&count_lock);
    int i=0;
    while (1) {
        printf("Thread B: %d\n",i);
        i++;
        sleep(ID_B);
        if (i>=WORK_B) {
            break;
        }
    }
    pthread_mutex_unlock(&count_lock);
}
void* theC(void* args) {
    pthread_mutex_lock(&count_lock);
    int i=0;
    while (1) {
        printf("Thread C: %d\n",i);
        i++;
        sleep(ID_C);
        if (i>=WORK_C) {
            break;
        }
    }
    pthread_mutex_unlock(&count_lock);
}



double RR() {

    pthread_t tid0, tid1, tid2;

    clock_t start, end;
    double timeCalculator=0;
    double RRTimer=0;

    thread_t threadData[3] = {
        {theA,5,0,0,0},
    {theB,5,0,0,0},
    {theC,5,0,0,0}
    };
    start = clock();
    while (!(threadData[0].complete) &&
        (!threadData[1].complete) &&
        (!threadData[2].complete)){


        if (threadData[0].work_done<threadData[0].work_needs) {
            pthread_create(&tid0, NULL, theA, NULL); // send first to theA
            threadData[0].work_done++;  // ++ to counter
            pthread_join(tid0, NULL);
            if (threadData[0].work_done==threadData[0].work_needs) {
                end = clock();
                timeCalculator = ((double) (end - start)) / CLOCKS_PER_SEC;
                RRTimer = RRTimer + timeCalculator;
                threadData[0].complete = 1;
            }
        }
        if (threadData[1].work_done<threadData[1].work_needs) {
            pthread_create(&tid1, NULL, theB, NULL);
            threadData[1].work_done++;  // ++ to counter
            pthread_join(tid1, NULL);
            if (threadData[1].work_done==threadData[1].work_needs) {
                end = clock();
                timeCalculator = ((double) (end - start)) / CLOCKS_PER_SEC;
                RRTimer = RRTimer + timeCalculator;
                threadData[1].complete = 1;
            }

        }
        if (threadData[2].work_done<threadData[2].work_needs) {
            pthread_create(&tid2, NULL, theC, NULL);
            threadData[2].work_done++;  // ++ to counter
            pthread_join(tid2, NULL);
            if (threadData[2].work_done==threadData[2].work_needs) {
                end = clock();
                timeCalculator = ((double) (end - start)) / CLOCKS_PER_SEC;
                RRTimer = RRTimer + timeCalculator;
                threadData[2].complete = 1;
            }
        }
    }
    return  (RRTimer/3);
}


double SJF() {

    pthread_t tid[3];

    clock_t start, end;
    double timeCalculator=0;
    double SJFTimer=0;

    thread_t threadData[3] = {
        {theA,5,0,0,0},
    {theB,5,0,0,0},
    {theC,5,0,0,0}
    };
    double min =99999;
    int minIndx=0;

    threadData[0].work_time=ID_A*WORK_A;
    if (threadData[0].work_time<min){min=threadData[0].work_time; minIndx=0;}
    threadData[1].work_time=ID_B*WORK_B;
    if (threadData[1].work_time<min){min=threadData[1].work_time; minIndx=1;}
    threadData[2].work_time=ID_C*WORK_C;
    if (threadData[2].work_time<min){min=threadData[2].work_time; minIndx=2;}

    int otherIndx1 = (minIndx+1)%3;
    int otherIndx2 = (minIndx+2)%3;

    while (threadData[minIndx].work_done<threadData[minIndx].work_needs) {
        start = clock();
        pthread_create(&tid[minIndx], NULL, theA, NULL); // send first to theA
        threadData[minIndx].work_done++;  // ++ to counter
        pthread_join(tid[minIndx], NULL);
    }
    end = clock();
    threadData[minIndx].complete=1;
    timeCalculator = ((double) (end - start)) / CLOCKS_PER_SEC;
    SJFTimer = SJFTimer + timeCalculator;

    if (threadData[otherIndx1].work_time<threadData[otherIndx2].work_time) {
        minIndx=otherIndx1;
        otherIndx1 = otherIndx2;
    }
    else{minIndx=otherIndx2;}
    // last will always be otherIndx1
    while (threadData[minIndx].work_done<threadData[minIndx].work_needs) {
        //start = clock();
        pthread_create(&tid[minIndx], NULL, theA, NULL); // send first to theA
        threadData[minIndx].work_done++;  // ++ to counter
        pthread_join(tid[minIndx], NULL);
    }
    end = clock();
    threadData[minIndx].complete=1;
    timeCalculator = ((double) (end - start)) / CLOCKS_PER_SEC;
    SJFTimer = SJFTimer + timeCalculator;

    minIndx=0;
    while (threadData[otherIndx1].work_done<threadData[otherIndx1].work_needs) {
        //start = clock();
        pthread_create(&tid[otherIndx1], NULL, theA, NULL); // send first to theA
        threadData[otherIndx1].work_done++;  // ++ to counter
        pthread_join(tid[otherIndx1], NULL);
    }
    end = clock();
    threadData[otherIndx1].complete=1;

    timeCalculator = ((double) (end - start)) / CLOCKS_PER_SEC;
    SJFTimer = SJFTimer + timeCalculator;




    printf("TimerFor0 is %f\n", threadData[0].work_time);
    printf("TimerFor1 is %f\n", threadData[1].work_time);
    printf("TimerFor2 is %f\n", threadData[2].work_time);

    return  (SJFTimer/3);
}
double FIFO() {

    pthread_t tid[3];

    clock_t start, end;
    double timeCalculator=0;
    double FIFOtimer=0;

    thread_t threadData[3] = {
        {theA,5,0,0,0},
    {theB,5,0,0,0},
    {theC,5,0,0,0}
    };
    start = clock();

    for (int i=0; i<3; i++) {
        while (threadData[i].work_done<threadData[i].work_needs) {
            pthread_create(&tid[i], NULL, theA, NULL); // send first to theA
            threadData[i].work_done++;  // ++ to counter
            pthread_join(tid[i], NULL);
        }
        end = clock();
        threadData[i].complete=1;
        timeCalculator = ((double) (end - start)) / CLOCKS_PER_SEC;
        FIFOtimer = FIFOtimer + timeCalculator;
    }


    printf("TimerFor0 is %f\n", threadData[0].work_time);
    printf("TimerFor1 is %f\n", threadData[1].work_time);
    printf("TimerFor2 is %f\n", threadData[2].work_time);

    return  (FIFOtimer/3);
}

int main() {


    double RRtimer = RR();
    // double SJFtimer = SJF();
    // double FIFOtimer = FIFO();

    printf("RRTimer is %f\n", RRtimer);
    // printf("SJFtimer is %f\n", SJFtimer);
    // printf("FIFOtimer is %f\n", FIFOtimer);



    // Destroy mutex and condition variable
    pthread_mutex_destroy(&count_lock);
    pthread_cond_destroy(&count_nonzero);

    return 0;
}
