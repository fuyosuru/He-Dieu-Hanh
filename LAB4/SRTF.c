#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
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
int iRemaining;
} PCB;

void randprocess(int n, PCB P[])
{
    for (int i=0;i<n;i++) 
    {
        P[i].iPID=i+1;
        P[i].iArrival=rand()%21;
        P[i].iBurst=rand()%11+2;
        P[i].iRemaining=P[i].iBurst;
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
        P[i].iRemaining=P[i].iBurst;
    }
}
void printProcess(int n, PCB P[])
{
    printf("PID\t\tArrival\t\tBurst\t\tStart\t\tFinish\t\tWaiting\t\tResponse\tTaT\n");
    for (int i=0;i<n;i++) 
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",P[i].iPID,P[i].iArrival,P[i].iBurst,P[i].iStart,P[i].iFinish,P[i].iWaiting,P[i].iResponse,P[i].iTaT);
}
void exportGanttChart (int n, PCB P[], int Gantt[])
{
    printf("Gantt Chart:\n|");
    for (int i=0;i<n;i++) 
        if(i==0 || Gantt[i]!=Gantt[i-1])
            if(Gantt[i]!=0)
                printf(" P%d |",Gantt[i]);
            else
                printf("    |",Gantt[i]);
    printf("\n%-5d",0);
    for (int i=1;i<=n;i++)
        if(Gantt[i]!=Gantt[i-1]) 
            printf("%-5d",i);
    printf("\n");
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
int Nextid(int n, PCB P[], int curr)
{
    int res=-1;
    for (int i=0;i<n;i++) 
        if (P[i].iArrival<=curr && P[i].iRemaining>0) 
            if (res==-1 || P[i].iRemaining<P[res].iRemaining || (P[i].iRemaining==P[res].iRemaining && P[i].iArrival<P[res].iArrival)) 
                res=i;
    return res;
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
    int GanttChart[100];
    int iNumberOfProcess;
    srand(time(NULL));
    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);
    randprocess(iNumberOfProcess, Input);
    int curr=0, completed=0, n=0;
    while (completed<iNumberOfProcess) 
    {
        int index=Nextid(iNumberOfProcess, Input, curr);
        if (index==-1) 
        {
            GanttChart[n++]=0;
            curr++;
            continue;
        }
        if (Input[index].iRemaining==Input[index].iBurst) 
            Input[index].iStart=curr;
        Input[index].iRemaining--;
        GanttChart[n++]=Input[index].iPID;
        curr++;
        if (Input[index].iRemaining==0) 
        {
            Input[index].iFinish=curr;
            Input[index].iResponse=Input[index].iStart-Input[index].iArrival;
            Input[index].iWaiting=Input[index].iFinish-Input[index].iArrival-Input[index].iBurst;
            Input[index].iTaT=Input[index].iFinish-Input[index].iArrival;
            completed++;
        }
    }
    printf("\n===== SRTF Scheduling =====\n");
    exportGanttChart(n, Input, GanttChart);
    printProcess(iNumberOfProcess, Input);
    calculateAWT(iNumberOfProcess, Input);
    calculateATaT(iNumberOfProcess, Input);
    return 0;
}
