#include <stdio.h>
#include <time.h>
#include <unistd.h>


#define WORK_A 10000
#define WORK_B 1000
#define WORK_C 7000
#define ID_A 0.8
#define ID_B 0.5
#define ID_C 0.75

int iOFa=0;
int iOFb=0;
int iOFc=0;
int flagA=0;
int flagB=0;
int flagC=0;

void* theA(){
    while(1){
        printf("Thread A: %d\n",iOFa);
        iOFa++;
        sleep(ID_A);

        if(iOFa>WORK_A){
            iOFa=0;
            flagA=0;
            return 0;
        }
        if (flagB||flagC) {
            return 0;
        }
    }
}

void* theB(){
    while(1){
        printf("Thread B: %d\n",iOFb);
        iOFb++;
        sleep(ID_B);

        if(iOFb>WORK_B){
            iOFb=0;
            flagB=0;
            return 0;
        }
        if (flagA||flagC) {
            return 0;
        }
    }
}

void* theC(){
    while(1){
        printf("Thread C: %d\n",iOFc);
        iOFc++;
        sleep(ID_C);

        if(iOFc>WORK_C){
            iOFc=0;
            flagC=0;
            return 0;
        }
        if (flagA||flagB) {
            return 0;
        }
    }
}

double FCFS(){
    clock_t start, end;
    double time_used;
    double MTA=0;
    start = clock();
    theA();
    end = clock();
    time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    MTA=MTA+time_used;
    printf("Thread A: time=%f\n",time_used);
    theB();
    end = clock();
    time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    MTA=MTA+time_used;
    printf("Thread B: time=%f\n",time_used);
    theC();
    end = clock();
    time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    MTA=MTA+time_used;
    printf("Thread C: time=%f\n",time_used);
    printf("Sum Time=%f\n",MTA);
    return(MTA/3);
}

double RR() {
    flagA=1;
    flagB=1;
    flagC=1;
    clock_t start, end;
    start = clock();
    double time_used = 0;
    double RRcount=0;
    while (flagA||flagB||flagC) {
        if (flagA) {
            theA();
            if (!flagA) { // Entered A, Finished A in theA, so flagA=0;
                end=clock();
                time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                RRcount=RRcount+time_used;
            }
        }
        if (flagB) {
            theB();
            if (!flagB) {
                end=clock();
                time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                RRcount=RRcount+time_used;
            }
        }
        if (flagC) {
            theC();
            if (!flagC) {
                end=clock();
                time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                RRcount=RRcount+time_used;
            }
        }
        printf("%d, %d, %d\n",flagA,flagB,flagC);
    }
    return (RRcount)/3;
}
double SJF(){
    clock_t start, end;
    double time_used;
    double SJF=0;
    start = clock();
    theB();
    end = clock();
    time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    SJF=SJF+time_used;
    printf("Thread B: time=%f\n",time_used);
    theC();
    end = clock();
    time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    SJF=SJF+time_used;
    printf("Thread C: time=%f\n",time_used);
    theA();
    end = clock();
    time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    SJF=SJF+time_used;
    printf("Thread A: time=%f\n",time_used);
    printf("Sum Time=%f\n",SJF);
    return(SJF/3);
}







void main(){
    double FCFSmta,RRmta,SJFmta;
    FCFSmta=FCFS();
    RRmta=RR();
    SJFmta=SJF();
    printf("FCFS: %f\n",FCFSmta);
    printf("RR: %f\n",RRmta);
    printf("SJF: %f\n",SJFmta);
    return;
}