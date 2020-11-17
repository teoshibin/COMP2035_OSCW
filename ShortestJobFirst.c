#include<stdio.h>

//from Edureka!
 int main()
{
    int burstTime[20],process[20],waitingTime[20],turnaroundTime[20],i,j,numberOfProcess,totalWaitingTime=0,smallestValueIndex,temp;
    float averageWaitingTime,averageTurnaroundTime;
    printf("Enter number of process:");
    scanf("%d",&numberOfProcess);
  
    printf("\nEnter Burst Time:\n");
    for(i=0;i<numberOfProcess;i++)
    {
        printf("process%d:",i+1);
        scanf("%d",&burstTime[i]);
        process[i]=i+1;         
    }
  
    //sorting of burst times
    for(i=0;i<numberOfProcess;i++)
    {
        smallestValueIndex=i;

        //comparing
        for(j=i+1; j<numberOfProcess; j++)
        {
            if(burstTime[j] < burstTime[smallestValueIndex])
                smallestValueIndex=j;
        }

        if(i!=smallestValueIndex){
            int temp = burstTime[i];
            burstTime[i] = burstTime[smallestValueIndex];
            burstTime[smallestValueIndex] = temp;
        }
  
        //swapping
        temp=burstTime[i];
        burstTime[i]=burstTime[smallestValueIndex];
        burstTime[smallestValueIndex]=temp;
  
        temp=process[i];
        process[i]=process[smallestValueIndex];
        process[smallestValueIndex]=temp;
    }
   
    waitingTime[0]=0;            
  
   
    for(i=1;i<numberOfProcess;i++)
    {
        waitingTime[i]=0;
        for(j=0;j<i;j++)
            waitingTime[i]+=burstTime[j];
  
        totalWaitingTime+=waitingTime[i];
    }
  
    averageWaitingTime=(float)totalWaitingTime/numberOfProcess;      
    totalWaitingTime=0;
  
    printf("\nProcess\t    Burst Time    \tWaiting Time\tTurnaround Time");
    for(i=0;i<numberOfProcess;i++)
    {
        turnaroundTime[i]=burstTime[i]+waitingTime[i];   
        totalWaitingTime+=turnaroundTime[i];
        printf("\n[P%d]\t\t  %d\t\t    %d\t\t\t%d",process[i],burstTime[i],waitingTime[i],turnaroundTime[i]);
    }
  
    averageTurnaroundTime=(float)totalWaitingTime/numberOfProcess;    
    printf("\n\nAverage Waiting Time=%f",averageWaitingTime);
    printf("\nAverage Turnaround Time=%f\n",averageTurnaroundTime);
}