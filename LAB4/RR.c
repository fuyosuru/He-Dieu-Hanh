#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

typedef struct{
int iPID;
int iArrival, iBurst, ibaseBurst;
int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

void randprocess(int n, PCB P[])
{
    for (int i=0;i<n;i++) 
    {
        P[i].iPID=i+1;
        P[i].iArrival=rand()%21;
        P[i].iBurst=rand()%11+2;
        P[i].ibaseBurst=P[i].iBurst;
        P[i].iStart=P[i].iFinish=P[i].iWaiting=P[i].iResponse=P[i].iTaT=0;
    }
} 

void inputProcess(int n, PCB P[])
{
    for (int i=0; i<n; i++) 
    {
        printf("Enter PID, Arrival Time, Burst Time to process %d: ",i+1);
        scanf("%d%d%d",&P[i].iPID,&P[i].iArrival,&P[i].iBurst);
        P[i].iStart=P[i].iFinish=P[i].iWaiting=P[i].iResponse=P[i].iTaT=0;
    }
}
void printProcess(int n, PCB P[])
{
    printf("PID\t\tArrival\t\tBurst\t\tStart\t\tFinish\t\tWaiting\t\tResponse\tTaT\n");
    for (int i=0;i<n;i++) 
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",P[i].iPID,P[i].iArrival,P[i].ibaseBurst,P[i].iStart,P[i].iFinish,P[i].iWaiting,P[i].iResponse,P[i].iTaT);
}
void exportGanttChart (int n, int Granttime[],int Grantid[], int endtime)
{
    printf("Gantt Chart:\n|");
    for (int i=0;i<n;i++) 
        if(i==0 || Grantid[i]!=Grantid[i-1])
            if(Grantid[i]!=0)
                printf(" P%d |",Grantid[i]);
            else
                printf("    |",Grantid[i]);
    printf("\n");
    for(int i=0;i<n;i++)
        if(i==0 || Grantid[i]!=Grantid[i-1] || i==n-1) 
            printf("%-5d",Granttime[i]);
    printf("%-5d",endtime);
    printf("\n");
}
void pushProcess(int *n, PCB P[], PCB Q)
{
    P[(*n)++] = Q;
}
void removeProcess(int *n, int index, PCB P[]) 
{
    for (int i=index;i<*n-1;i++) 
        P[i]=P[i+1];
    (*n)--;
}
int swapProcess(PCB *P, PCB *Q)
{
    PCB temp=*P;
    *P=*Q;
    *Q=temp;
    return 1;
}
int partition (PCB P[], int low, int high, int iCriteria)
{
    PCB pivot=P[high];
    int i=low-1;
    for (int j=low;j<high;j++) 
    {
        bool cmp=0;
        if (iCriteria==SORT_BY_ARRIVAL) 
            cmp=P[j].iArrival<pivot.iArrival;
        if (iCriteria==SORT_BY_PID) 
            cmp=P[j].iPID<pivot.iPID;
        if (iCriteria==SORT_BY_BURST) 
            cmp=P[j].iBurst<pivot.iBurst;
        if (iCriteria==SORT_BY_START) 
            cmp=P[j].iStart<pivot.iStart;
        if (cmp) 
            swapProcess(&P[++i],&P[j]);
    }
    swapProcess(&P[i+1],&P[high]);
    return i+1;
}
void quickSort(PCB P[],int low,int high,int iCriteria)
{
    if (low<high) 
    {
        int pi=partition(P,low,high,iCriteria);
        quickSort(P,low,pi-1,iCriteria);
        quickSort(P,pi+1,high,iCriteria);
    }
}
void calculateAWT(int n, PCB P[]) 
{
    float res=0;
    for (int i=0;i<n;i++) 
        res+=P[i].iWaiting;
    printf("AWT: %02f\n",res/n);
}
void calculateATaT(int n, PCB P[])
{
    float res=0;
    for (int i=0;i<n;i++) 
        res+=P[i].iTaT;
    printf("ATaT: %02f\n",res/n);
}
int main()
{
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];
    int iNumberOfProcess, quantum,Granttime[100], Grantid[100];
    srand(time(NULL));
    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);
    printf("Enter Time Quantum: ");
    scanf("%d", &quantum);
    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0, currentTime = 0, cGrant=0;
    randprocess(iNumberOfProcess, Input);
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);
    while (iTerminated < iNumberOfProcess)
    {
        while (iRemain > 0 && Input[0].iArrival <= currentTime)
        {
            pushProcess(&iReady, ReadyQueue, Input[0]);
            removeProcess(&iRemain, 0, Input);
        }
        if (iReady > 0)
        {
            if(cGrant>0 && Grantid[cGrant-1]==ReadyQueue[0].iPID)
                Granttime[cGrant-1]=Granttime[cGrant-1];
            else
            {
                Granttime[cGrant]=currentTime;
                Grantid[cGrant]=ReadyQueue[0].iPID;
                cGrant++;
            }
            ReadyQueue[0].iStart = (ReadyQueue[0].iStart == 0 && ReadyQueue[0].iFinish == 0) ? currentTime : ReadyQueue[0].iStart;
            int executionTime = (ReadyQueue[0].iBurst > quantum) ? quantum : ReadyQueue[0].iBurst;
            currentTime += executionTime;
            ReadyQueue[0].iBurst -= executionTime;
            while (iRemain > 0 && Input[0].iArrival <= currentTime)
            {
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
            }
            if (ReadyQueue[0].iBurst == 0)
            {
                ReadyQueue[0].iFinish = currentTime;
                ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
                ReadyQueue[0].iWaiting = ReadyQueue[0].iTaT - (ReadyQueue[0].iFinish - ReadyQueue[0].iStart);
                ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
                pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
                removeProcess(&iReady, 0, ReadyQueue);
            }
            else
                {
                    pushProcess(&iReady, ReadyQueue, ReadyQueue[0]);
                    removeProcess(&iReady, 0, ReadyQueue);
                }
        }
        else
        {
            if(cGrant>0 && Grantid[cGrant-1]==0)
                Granttime[cGrant-1]=Granttime[cGrant-1];
            else
            {
                Granttime[cGrant]=currentTime;
                Grantid[cGrant]=0;
                cGrant++;
            }
            currentTime = Input[0].iArrival;
        }
    }

    printf("\n===== Round Robin Scheduling =====\n");
    exportGanttChart(cGrant, Granttime, Grantid, currentTime);
    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);
    printProcess(iTerminated, TerminatedArray); 
    calculateAWT(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);
    return 0;
}
