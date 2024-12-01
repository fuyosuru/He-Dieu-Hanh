#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

typedef struct{
int iPID;
int iArrival, iBurst;
int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

void randprocess(int n, PCB P[])
{
    for (int i=0;i<n;i++) 
    {
        P[i].iPID=i+1;
        P[i].iArrival=rand()%21;
        P[i].iBurst=rand()%11+2;
        P[i].iStart=P[i].iFinish=P[i].iWaiting=P[i].iResponse=P[i].iTaT=0;
    }
} 

void inputProcess(int n, PCB P[])
{
    for (int i=0; i<n; i++) 
    {
        printf("Enter PID, arrival time and burst Time of process %d: ",i+1);
        scanf("%d%d%d",&P[i].iPID,&P[i].iArrival,&P[i].iBurst);
        P[i].iStart=P[i].iFinish=P[i].iWaiting=P[i].iResponse=P[i].iTaT=0;
    }
}
void printProcess(int n, PCB P[])
{
    printf("PID\t\tArrival\t\tBurst\t\tStart\t\tFinish\t\tWaiting\t\tResponse\tTaT\n");
    for (int i=0;i<n;i++) 
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",P[i].iPID,P[i].iArrival,P[i].iBurst,P[i].iStart,P[i].iFinish,P[i].iWaiting,P[i].iResponse,P[i].iTaT);
}
void exportGanttChart (int n, PCB P[])
{
    int timeline=0;
    printf("Gantt Chart:\n|");
    for (int i=0;i<n;i++) 
        {
            if(P[i].iArrival>timeline)
                printf("    |",P[i].iPID);
            printf(" P%d |",P[i].iPID);
            timeline=P[i].iFinish;
        }
    timeline=0;
    printf("\n%-5d",0);
    for(int i=0;i<n;i++)
        {
            if(P[i].iArrival>timeline)
                printf("%-5d",P[i].iArrival); 
            printf("%-5d",P[i].iFinish);
            timeline=P[i].iFinish;
        }
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
int Nextid(int n, PCB P[], int curr)
{
    int res=-1;
    for (int i=0;i<n;i++) 
        if (P[i].iArrival<=curr) 
            if (res==-1 || P[i].iBurst<P[res].iBurst) 
                res=i;
    return res;
}
int main()
{
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];
    PCB ans[10];
    int iNumberOfProcess;
    srand(time(NULL));
    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);
    int iRemain=iNumberOfProcess, iReady=0, iTerminated=0;
    randprocess(iNumberOfProcess, Input);
    for(int i=0;i<iNumberOfProcess;i++)
        ans[i]=Input[i];
    quickSort(Input, 0,iNumberOfProcess - 1,SORT_BY_ARRIVAL);
    int curr=Input[0].iArrival;
    while (iTerminated < iNumberOfProcess)
    {
        int index=Nextid(iRemain,Input,curr);
        if (index==-1) 
        {
            curr++;
            continue;
        }
        pushProcess(&iReady, ReadyQueue, Input[index]);
        removeProcess(&iRemain, index, Input);
        ReadyQueue[0].iStart=curr;
        ReadyQueue[0].iFinish=ReadyQueue[0].iStart+ReadyQueue[0].iBurst;
        ReadyQueue[0].iResponse=ReadyQueue[0].iStart-ReadyQueue[0].iArrival;
        ReadyQueue[0].iWaiting=ReadyQueue[0].iResponse;
        ReadyQueue[0].iTaT=ReadyQueue[0].iFinish-ReadyQueue[0].iArrival;
        ans[ReadyQueue[0].iPID-1].iStart=curr;
        ans[ReadyQueue[0].iPID-1].iFinish=ReadyQueue[0].iStart+ReadyQueue[0].iBurst;
        ans[ReadyQueue[0].iPID-1].iResponse=ReadyQueue[0].iStart-ReadyQueue[0].iArrival;
        ans[ReadyQueue[0].iPID-1].iWaiting=ReadyQueue[0].iResponse;
        ans[ReadyQueue[0].iPID-1].iTaT=ReadyQueue[0].iFinish-ReadyQueue[0].iArrival;
        curr=ReadyQueue[0].iFinish;
        pushProcess(&iTerminated,TerminatedArray,ReadyQueue[0]);
        removeProcess(&iReady,0,ReadyQueue);
    }
    printf("\n===== SJF Scheduling =====\n");
    exportGanttChart(iTerminated, TerminatedArray);
    quickSort(TerminatedArray,0,iTerminated - 1,SORT_BY_PID);
    printProcess(iTerminated, ans);
    calculateAWT(iTerminated,TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);
    return 0;
}
