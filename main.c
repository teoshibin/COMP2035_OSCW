
/*
88""Yb 88""Yb  dP"Yb   dP""b8 888888 .dP"Y8 .dP"Y8     .dP"Y8  dP""b8 88  88 888888 8888b.  88   88 88     88 88b 88  dP""b8
88__dP 88__dP dP   Yb dP   `" 88__   `Ybo." `Ybo."     `Ybo." dP   `" 88  88 88__    8I  Yb 88   88 88     88 88Yb88 dP   `"
88"""  88"Yb  Yb   dP Yb      88""   o.`Y8b o.`Y8b     o.`Y8b Yb      888888 88""    8I  dY Y8   8P 88  .o 88 88 Y88 Yb  "88
88     88  Yb  YbodP   YboodP 888888 8bodP' 8bodP'     8bodP'  YboodP 88  88 888888 8888Y"  `YbodP' 88ood8 88 88  Y8  YboodP
*/

/* ------------------------------- References ------------------------------- */

//1. edureka!
//2. round robin using ready queue https://www.youtube.com/watch?v=-jFGYDfWkXI

/* ----------------------------------- lib ---------------------------------- */

#include <stdio.h>

/* ---------------------------------- macro --------------------------------- */

#define CPU_CAPABILITY 50
#define EMPTY -1 // number that is not in the bound of array

/* ----------------------------- data structure ----------------------------- */

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

/* -------------------------------- prototype ------------------------------- */

void rr(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess],
        int timeQuantum, int ContextSwitchingTime, double *averageWaitingTime, double *averageTurnaroundTime);

void fcfs(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess],
          double *averageWaitingTime, double *averageTurnaroundTime);

void sjf(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess],
         double *averageWaitingTime, double *averageTurnaroundTime);

void display(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess],
             double averageWaitingTime, double averageTurnaroundTime);

/* -------------------------------------------------------------------------- */
/*                                main function                               */
/* -------------------------------------------------------------------------- */

int main()
{
    // input variable
    int timeQuantum;
    int ContextSwitchingTime;
    int numberOfProcess, j;
    double averageWaitingTime = 0;
    double averageTurnaroundTime = 0;

    // input
    printf("\nEnter total number of processes:");
    scanf("%d", &numberOfProcess);

    int processData[NUM_PROCESSDATATYPE][numberOfProcess];

    for (int i = 0; i < numberOfProcess; i++)
    {
        processData[processID][i] = i + 1;
        printf("\nEnter Details of Process[%d]\n", processData[processID][i]);
        printf("Arrival Time:\t");
        scanf("%d", &processData[arrivalTime][i]);
        printf("Burst Time:\t");
        scanf("%d", &processData[burstTime][i]);
    }

    printf("\nEnter Time Quantum:\t");
    scanf("%d", &timeQuantum);

    printf("\nEnter context switching time:\t");
    scanf("%d", &ContextSwitchingTime);

    printf("\nProcess ID\t\tBurst Time\t Turnaround Time\t Waiting Time\n");

    // fcfs algorithms
    // fcfs(numberOfProcess, processData, &averageWaitingTime, &averageTurnaroundTime);
    // sjf(numberOfProcess, processData, &averageWaitingTime, &averageTurnaroundTime);
    rr(numberOfProcess, processData, timeQuantum, ContextSwitchingTime, &averageWaitingTime, &averageTurnaroundTime);

    // display function
    // display(numberOfProcess, processData, averageWaitingTime, averageTurnaroundTime);

    return 0;
}

/* -------------------------------------------------------------------------- */
/*                                  functions                                 */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                 round robin                                */
/* -------------------------------------------------------------------------- */

void rr(
    int numberOfProcess,
    int processData[NUM_PROCESSDATATYPE][numberOfProcess],
    int timeQuantum,
    int ContextSwitchingTime,
    double *averageWaitingTime,
    double *averageTurnaroundTime)
{

    /* -------------------------------- prototype ------------------------------- */

    void removeArrayElement(int array[], int index, int *end);
    void selectiveSortProcess(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], processDataType type);
    int deque(int size, int array[size], int *front, int rear, int *fill);
    void enque(int size, int array[size], int front, int *rear, int *fill, int value);
    int notContain(int size, int array[size], int value);
    int find(int array[], int value, int end);

    /* -------------------------------- variable -------------------------------- */

    //loop related
    int currentTimeFrame = 0;             // current time
    int previousTimeFrame = -timeQuantum; // previous current time

    int workingProcessIndex = EMPTY; // current working process index
    int previousWorkingIndex = 1;

    int arrivedProcessesCounter = 0;       // index of (int arrivedProcesses[numberOfProcess];) variable
    int arrivedProcesses[numberOfProcess]; // add or remove the index of process to list if arrive or done
    int arrivedProcessIndex = 0;

    int remainingProcesses = numberOfProcess;
    int reducingBurstTime[numberOfProcess]; // gradually reduce burst time to 0 and set it as done
    int doneProcessBoolean = 0;

    // queue realated variable
    int readyQueue[numberOfProcess];
    int fill = 0;  // queue length
    int front = 0; // queue head
    int rear = 0;  // queue tail

    //output related
    int totalTurnaroundTime = 0;
    int totalWaitingTime = 0;

    /* ------------------------------- initialize ------------------------------- */

    // sort by arrival time
    selectiveSortProcess(numberOfProcess, processData, arrivalTime);

    for (int i = 0; i < numberOfProcess; i++)
    {
        reducingBurstTime[i] = processData[burstTime][i]; // duplicate burst time
        processData[turnaroundTime][i] = 0;               //init turnaround time
        processData[waitingTime][i] = 0;                  // init waiting time
        readyQueue[i] = EMPTY;
    }

    /* ----------------------------- algorithm body ----------------------------- */

    // while there is still remaining job then continue looping
    while (remainingProcesses)
    {
        // add new arrival to ready queue
        for (int i = 0; i < numberOfProcess; i++)
        {
            if (processData[arrivalTime][i] > previousTimeFrame && processData[arrivalTime][i] <= currentTimeFrame)
            {
                arrivedProcesses[arrivedProcessesCounter++] = i;
                enque(numberOfProcess, readyQueue, front, &rear, &fill, i);

                // add initial time difference between current and arrival time
                processData[turnaroundTime][i] += currentTimeFrame - processData[arrivalTime][i];
                processData[waitingTime][i] += currentTimeFrame - processData[arrivalTime][i];
            }
        }

        // add old arrival to ready queue
        for (int i = 0; i < numberOfProcess; i++)
        {
            if (processData[arrivalTime][i] <= previousTimeFrame && reducingBurstTime[i] != 0 && notContain(numberOfProcess, readyQueue, i))
            {
                enque(numberOfProcess, readyQueue, front, &rear, &fill, i);
            }
        }

        previousTimeFrame = currentTimeFrame;

        // if no process in ready queue skip to the next time frame
        if (fill == 0)
        {
            currentTimeFrame++;
            continue;
        }

        // add context switching time
        if (workingProcessIndex != EMPTY)
        {
            previousWorkingIndex = workingProcessIndex;
        }
        workingProcessIndex = deque(numberOfProcess, readyQueue, &front, rear, &fill);
        if (previousWorkingIndex != workingProcessIndex && workingProcessIndex != 0)
        {
            currentTimeFrame += ContextSwitchingTime;
        }

        // if burst time smaller than time quantum
        if (reducingBurstTime[workingProcessIndex] <= timeQuantum)
        {
            // adjust time
            currentTimeFrame += reducingBurstTime[workingProcessIndex];
            // reduce burst time
            reducingBurstTime[workingProcessIndex] = 0;
            remainingProcesses--;
            doneProcessBoolean = 1;
        } // larger than time quantum
        else if (reducingBurstTime[workingProcessIndex] > timeQuantum)
        {
            // adjust time
            currentTimeFrame += timeQuantum;
            // reduce burst time
            reducingBurstTime[workingProcessIndex] -= timeQuantum;
        }

        // find index of working process in arrivedProcesses using working process index
        // arrivedProcesses is a container similar to RAM, currently running process will be stored in it
        arrivedProcessIndex = find(arrivedProcesses, workingProcessIndex, arrivedProcessesCounter);

        // add turnaround time to all arrived job
        for (int i = 0; i < arrivedProcessesCounter; i++)
        {
            processData[turnaroundTime][arrivedProcesses[i]] += currentTimeFrame - previousTimeFrame;
        }
        // add context switching time as waiting time for arrived working process
        if (workingProcessIndex != 0)
        {
            processData[waitingTime][arrivedProcesses[arrivedProcessIndex]] += ContextSwitchingTime;
        }
        // add waiting time and context switching time to all arrived and waiting processes
        for (int i = 0; i < arrivedProcessesCounter; i++)
        {
            if (i != arrivedProcessIndex)
            {
                processData[waitingTime][arrivedProcesses[i]] += currentTimeFrame - previousTimeFrame;
            }
        }

        if (doneProcessBoolean)
        {
            removeArrayElement(arrivedProcesses, arrivedProcessIndex, &arrivedProcessesCounter);
            // output value
            printf("\nProcess[%d]\t\t%d\t\t %d\t\t\t %d",
                   processData[processID][workingProcessIndex],
                   processData[burstTime][workingProcessIndex],
                   processData[turnaroundTime][workingProcessIndex],
                   processData[waitingTime][workingProcessIndex]);

            doneProcessBoolean = 0;
        }
    }

    /* ---------------------------- calculate output ---------------------------- */

    selectiveSortProcess(numberOfProcess, processData, processID);

    for (int i = 0; i < numberOfProcess; i++)
    {
        totalTurnaroundTime += processData[turnaroundTime][i];
        totalWaitingTime += processData[waitingTime][i];
    }

    *averageWaitingTime = (double)totalWaitingTime / numberOfProcess;
    *averageTurnaroundTime = (double)totalTurnaroundTime / numberOfProcess;

    printf("\n\nAverage Waiting Time:\t%.2f", *averageWaitingTime);
    printf("\nAverage Turnaround Time:\t%.2f\n", *averageTurnaroundTime);
}

/* -------------------------------------------------------------------------- */
/*                             shortest job first                             */
/* -------------------------------------------------------------------------- */

void sjf(
    int numberOfProcess,
    int processData[NUM_PROCESSDATATYPE][numberOfProcess],
    double *averageWaitingTime,
    double *averageTurnaroundTime)
{

    // prototype
    void selectiveSortProcess(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], processDataType type);

    // variable
    int smallestValueIndex;
    int temp;
    int j;

    // selection sort
    selectiveSortProcess(numberOfProcess, processData, burstTime);

    // algorithm
    fcfs(numberOfProcess, processData, averageWaitingTime, averageTurnaroundTime);
}

/* -------------------------------------------------------------------------- */
/*                           first come first serve                           */
/* -------------------------------------------------------------------------- */

void fcfs(
    int numberOfProcess,
    int processData[NUM_PROCESSDATATYPE][numberOfProcess],
    double *averageWaitingTime,
    double *averageTurnaroundTime)
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
        *averageTurnaroundTime += processData[turnaroundTime][i];
    }

    *averageWaitingTime /= numberOfProcess;
    *averageTurnaroundTime /= numberOfProcess;
}

/* -------------------------------------------------------------------------- */
/*                              display function                              */
/* -------------------------------------------------------------------------- */

void display(
    int numberOfProcess,
    int processData[NUM_PROCESSDATATYPE][numberOfProcess],
    double averageWaitingTime,
    double averageTurnaroundTime)
{

    printf("\nProcess\t\tStart Time\tBurst Time\tWaiting Time\tTurnaround Time");

    for (int k = 0; k < numberOfProcess; k++)
    {
        printf("\nP[%d]\t\t%d\t\t%d\t\t%d\t\t%d", processData[processID][k], processData[startTime][k], processData[burstTime][k], processData[waitingTime][k], processData[turnaroundTime][k]);
    }

    printf("\n\nAverage Waiting Time: %.2f \n", averageWaitingTime);
    printf("Average Turnaround Time: %.2f \n\n", averageTurnaroundTime);
    printf("CPU Usage: %.2f%% \n\n", ((double)numberOfProcess / CPU_CAPABILITY) * 100);
}

/* -------------------------------------------------------------------------- */
/*                               helper function                              */
/* -------------------------------------------------------------------------- */

// find index using value
int find(int array[], int value, int end)
{
    for (int i = 0; i < end; i++)
    {
        if (array[i] == value)
        {
            return i;
        }
    }
    return -1;
}

// remove element using index and do array shifting
void removeArrayElement(int array[], int index, int *end)
{
    for (int i = index; i < *end - 1; i++)
    {
        if ((i + 1) != *end)
        {
            array[i] = array[i + 1];
        }
    }
    array[*end - 1] = 0;
    *end -= 1;
}

// check if value is in an array (contain : return false; not contain : return true)
int notContain(int size, int array[size], int value)
{
    for (int i = 0; i < size; i++)
    {
        if (array[i] == value)
        {
            return 0;
        }
    }
    return 1;
}

// enqueue value into an array circularly
void enque(int size, int array[size], int front, int *rear, int *fill, int value)
{
    if (*fill < size)
    {
        array[*rear] = value;
        *rear += 1;
        *rear %= size;
        *fill += 1;
    }
}

// dequeue value from an array circularly
int deque(int size, int array[size], int *front, int rear, int *fill)
{
    int value = EMPTY;
    if (*fill)
    {
        value = array[*front];
        array[*front] = EMPTY;
        *front += 1;
        *front %= size;
        *fill -= 1;
    }
    return value;
}

// sort and recurisively insert smallest number from the back to the first element
void selectiveSortProcess(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], processDataType type)
{
    // prototype
    void swapProcessData(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], processDataType type, int index1, int index2);

    // variable
    int smallestValueIndex;
    int temp;

    // body
    for (int i = 0; i < numberOfProcess; i++)
    {
        smallestValueIndex = i;
        //comparing
        for (int j = i + 1; j < numberOfProcess; j++)
        {
            if (processData[type][j] < processData[type][smallestValueIndex])
            {
                smallestValueIndex = j;
            }
        }
        // swap if current burst time is not the smallest burst time
        if (smallestValueIndex != i)
        {
            swapProcessData(numberOfProcess, processData, NUM_PROCESSDATATYPE, i, smallestValueIndex);
        }
    }
}

// swap process data
void swapProcessData(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], processDataType type, int index1, int index2)
{
    // variable
    int temp = 0;

    switch (type)
    {
    // swap all
    case NUM_PROCESSDATATYPE:
        for (int i = 0; i < NUM_PROCESSDATATYPE; i++)
        {
            temp = processData[i][index2];
            processData[i][index2] = processData[i][index1];
            processData[i][index1] = temp;
        }
        break;
    // swap certain type only
    default:
        temp = processData[type][index2];
        processData[type][index2] = processData[type][index1];
        processData[type][index1] = temp;
        break;
    }
}