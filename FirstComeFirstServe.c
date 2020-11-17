// include
#include <stdio.h>

// function prototype
void display(int numberOfProcess, int startTime[numberOfProcess], int burstTime[numberOfProcess], 
            int waitingTime[numberOfProcess], int turnaroundTime[numberOfProcess], int averageWaitingTime, 
            int averageTurnAroundTime);
// main function
int main()
{
    //TODO
    //#CPU usage
    
    int numberOfProcess, i, j;
    int averageWaitingTime = 0, averageTurnaroundTime = 0;

    printf("\nEnter total number of processes:");
    scanf("%d", &numberOfProcess);

    int burstTime[numberOfProcess], waitingTime[numberOfProcess], turnaroundTime[numberOfProcess], startTime[numberOfProcess];

    printf("\nEnter Process Burst Time: \nNumberOfProcess: \n");
    for (i = 0; i < numberOfProcess; i++)
    {
        printf("P[%d]:", i + 1);
        scanf("%d", &burstTime[i]);
    }

    //// fcfs algorithm
    // calculate waiting time

    waitingTime[0] = 0;
    startTime[0] = 0;
    
    for (i = 1; i < numberOfProcess; i++)
    {
        waitingTime[i] = 0;
        for (j = 0; j < i; j++)
            waitingTime[i] += burstTime[j];
    }

    for (i = 0; i < numberOfProcess; i++)
    {
        startTime[i] = turnaroundTime[i-1];
        turnaroundTime[i] = burstTime[i] + waitingTime[i];
        averageWaitingTime += waitingTime[i];
        averageTurnaroundTime += turnaroundTime[i];
    }

    averageWaitingTime /= i;
    averageTurnaroundTime /= i;

    //display
    display(numberOfProcess, startTime, burstTime, waitingTime, turnaroundTime, averageWaitingTime, averageTurnaroundTime);

    return 0;
}


// main display function
void display(int numberOfProcess, int startTime[numberOfProcess], int burstTime[numberOfProcess], 
            int waitingTime[numberOfProcess], int turnaroundTime[numberOfProcess], int averageWaitingTime, 
            int averageTurnAroundTime){

    printf("\nProcess\t\tStart Time\tBurst Time\tWaiting Time\tTurnaround Time");      

    for (int k = 0; k < numberOfProcess; k++)
    {
        printf("\nP[%d]\t\t%d\t\t%d\t\t%d\t\t%d", k + 1, startTime[k], burstTime[k], waitingTime[k], turnaroundTime[k]);
    }
    
    printf("\n\nAverage Waiting Time: %d \n", averageWaitingTime);
    printf("Average Turnaround Time: %d \n\n", averageTurnAroundTime);
}

