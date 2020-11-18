// include
#include <stdio.h>

//types
#define CPU_CAPABILITY 50

typedef enum
{
    processID = 0,
    startTime = 1,
    burstTime = 2,
    waitingTime = 3,
    turnaroundTime = 4,
    arrivalTime = 5,
    NUM_PROCESSDATATYPE
} processDataType;

// function prototype
void fcfs(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess],
          int *averageWaitingTime, int *averageTurnAroundTime);
          
void sjf(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess],
         int *averageWaitingTime, int *averageTurnAroundTime);

void display(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess],
             int averageWaitingTime, int averageTurnAroundTime);

// main function
int main()
{
    // set input
    int numberOfProcess, i, j;
    int averageWaitingTime = 0, averageTurnaroundTime = 0;

    printf("\nEnter total number of processes:");
    scanf("%d", &numberOfProcess);

    int processData[NUM_PROCESSDATATYPE][numberOfProcess];

    printf("\nEnter Process Burst Time: \nNumberOfProcess: \n");
    for (i = 0; i < numberOfProcess; i++)
    {
        processData[processID][i] = i + 1;
        printf("P[%d]:", processData[processID][i]);
        scanf("%d", &processData[burstTime][i]);
    }

    // fcfs algorithms
    // fcfs(numberOfProcess, processData, &averageWaitingTime, &averageTurnaroundTime);
    // sjf(numberOfProcess, processData, &averageWaitingTime, &averageTurnaroundTime);

    // display function
    display(numberOfProcess, processData, averageWaitingTime, averageTurnaroundTime);

    return 0;
}

void rr(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess],
        int timeQuantum, int *averageWaitingTime, int *averageTurnaroundTime){

    typedef enum{
        NA = 0,
        WAIT = 1,
        WORK = 2,
        OVHD = 3,
    }status;

    int quantumStatus[numberOfProcess];

    for 
}

// shortest job first
void sjf(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess],
         int *averageWaitingTime, int *averageTurnaroundTime)
{

    int smallestValueIndex;
    int temp;
    int j;

    // selection sort
    for (int i = 0; i < numberOfProcess; i++)
    {
        smallestValueIndex = i;

        // compare i to j
        for (int j = i + 1; j < numberOfProcess; j++)
        {
            if (processData[burstTime][j] < processData[burstTime][i])
                smallestValueIndex = j;
        }

        // swap if current burst time is not the smallest burst time
        if (smallestValueIndex != i)
        {
            temp = processData[burstTime][i];
            processData[burstTime][i] = processData[burstTime][smallestValueIndex];
            processData[burstTime][smallestValueIndex] = temp;

            temp = processData[processID][i];
            processData[processID][i] = processData[processID][smallestValueIndex];
            processData[processID][smallestValueIndex] = temp;
        }
    }

    fcfs(numberOfProcess, processData, averageWaitingTime, averageTurnaroundTime);

}

// first come first serve
void fcfs(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess],
          int *averageWaitingTime, int *averageTurnAroundTime)
{

    processData[waitingTime][0] = 0;

    for (int i = 1; i < numberOfProcess; i++)
    {
        processData[waitingTime][i] = 0;
        for (int j = 0; j < i; j++)
            processData[waitingTime][i] += processData[burstTime][j];
    }

    for (int i = 0; i < numberOfProcess; i++)
    {
        processData[startTime][i] = i == 0 ? 0 : processData[turnaroundTime][i - 1];
        processData[turnaroundTime][i] = processData[burstTime][i] + processData[waitingTime][i];
        *averageWaitingTime += processData[waitingTime][i];
        *averageTurnAroundTime += processData[turnaroundTime][i];
    }

    *averageWaitingTime /= numberOfProcess;
    *averageTurnAroundTime /= numberOfProcess;
}

// main display function
void display(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess],
             int averageWaitingTime, int averageTurnaroundTime)
{

    printf("\nProcess\t\tStart Time\tBurst Time\tWaiting Time\tTurnaround Time");

    for (int k = 0; k < numberOfProcess; k++)
    {
        printf("\nP[%d]\t\t%d\t\t%d\t\t%d\t\t%d", processData[processID][k], processData[startTime][k], processData[burstTime][k], processData[waitingTime][k], processData[turnaroundTime][k]);
    }

    printf("\n\nAverage Waiting Time: %d \n", averageWaitingTime);
    printf("Average Turnaround Time: %d \n\n", averageTurnaroundTime);
    printf("CPU Usage: %.2f%% \n\n", ((double)numberOfProcess / CPU_CAPABILITY) * 100);
}



//Reference
//1. edureka!