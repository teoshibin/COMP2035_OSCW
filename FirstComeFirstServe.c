// include
#include <stdio.h>

// function prototype
void display();

// main function
int main()
{
    //TODO
    //#startTime
    //#CPU usage
    int numberOfProcess, i, j;
    int burstTime[20], waitingTime[20], turnaroundTime[20], averageWaitingTime = 0, averageTurnaroundTime = 0;

    printf("\nEnter total number of processes(maximum 20):");
    scanf("%d", &numberOfProcess);

    printf("\nEnter Process Burst Time: \nNumberOfProcess: \n");
    for (i = 0; i < numberOfProcess; i++)
    {
        printf("P[%d]:", i + 1);
        scanf("%d", &burstTime[i]);
    }

    waitingTime[0] = 0;

    // calculate waiting time
    for (i = 1; i < numberOfProcess; i++)
    {
        waitingTime[i] = 0;
        for (j = 0; j < i; j++)
            waitingTime[i] += burstTime[j];
    }

    printf("\nProcess\t\tBurst Time\tWaiting Time\tTurnaround Time");

    for (i = 0; i < numberOfProcess; i++)
    {
        turnaroundTime[i] = burstTime[i] + waitingTime[i];
        averageWaitingTime += waitingTime[i];
        averageTurnaroundTime += turnaroundTime[i];
        printf("\nP[%d]\t\t%d\t\t%d\t\t%d", i + 1, burstTime[i], waitingTime[i], turnaroundTime[i]);
    }

    averageWaitingTime /= i;
    averageTurnaroundTime /= i;

    printf("\n\nAverage Waiting Time: %d \n", averageWaitingTime);
    printf("Average Turnaround Time: %d \n\n", averageTurnaroundTime);

    // void display();

    return 0;
}

// void display(){
//     printf("\numberOfProcess\nAverage Waiting Time:%d", averageWaitingTime);
//     printf("\nAverage Turnaround Time:%d \numberOfProcess", averageTurnaroundTime);
//     printf("\nProcess\t\tBurst Time\tWaiting Time\tTurnaround Time");
// }