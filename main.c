
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
    arrivalTime = 1,
    burstTime = 2,
    turnaroundTime = 3,
    waitingTime = 4,
    respondTime = 5,
    completionTime = 6,
    NUM_PROCESSDATATYPE
} processDataType;

/* -------------------------------- prototype ------------------------------- */

void rr(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], int timeQuantum, int contextSwitchingTime, int *contextSwitches);

// void fcfs(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess],
//           double *averageWaitingTime, double *averageTurnaroundTime);

void fcfs(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], int overheadTime, int* overheadCount);

void sjf(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], int contextSwitchingTime, int* contextSwitches);

void display(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess],
             double averageWaitingTime, double averageTurnaroundTime);

void display2(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], int contextSwitches);

void reset(int size, int array[NUM_PROCESSDATATYPE][size]);

/* -------------------------------------------------------------------------- */
/*                                main function                               */
/* -------------------------------------------------------------------------- */

int main()
{
    // input variable
    int timeQuantum;
    int contextSwitchingTime;
    int numberOfProcess, j;
    double averageWaitingTime = 0;
    double averageTurnaroundTime = 0;
    int contextSwitches = 0;

    // input
    printf("\nEnter total number of processes:");
    scanf("%d", &numberOfProcess);

    int processData[NUM_PROCESSDATATYPE][numberOfProcess];

    reset(numberOfProcess, processData);

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
    scanf("%d", &contextSwitchingTime);

    // printf("\nProcess ID\t\t Burst Time\t Turnaround Time\t Waiting Time\n");
    // printf("\nProcess ID\t\tArrival Time\t Burst Time\t Turnaround Time\t Waiting Time\n");

    // fcfs algorithms
    // fcfs(numberOfProcess, processData, contextSwitchingTime, &contextSwitches);
    sjf(numberOfProcess, processData, contextSwitchingTime, &contextSwitches);
    // rr(numberOfProcess, processData, timeQuantum, contextSwitchingTime, &contextSwitches);

    // display function
    // display(numberOfProcess, processData, averageWaitingTime, averageTurnaroundTime);
    display2(numberOfProcess, processData, contextSwitches);

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
    int contextSwitchingTime,
    int *contextSwitches)
{

    /* -------------------------------- prototype ------------------------------- */

    void selectiveSortProcess(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], processDataType type);
    int deque(int size, int array[size], int *front, int rear, int *fill);
    void enque(int size, int array[size], int front, int *rear, int *fill, int value);
    int notContain(int size, int array[size], int value);

    /* -------------------------------- variable -------------------------------- */

    //loop related
    int currentTimeFrame = 0;             // current time
    int previousTimeFrame = -timeQuantum; // previous current time
    int workingProcessIndex = EMPTY; // current working process index
    int previousWorkingIndex = 1;
    int remainingProcesses = numberOfProcess;
    int reducingBurstTime[numberOfProcess]; // gradually reduce burst time to 0 and set it as done
    int doneProcessBoolean = 0;

    // queue realated variable
    int readyQueue[numberOfProcess];
    int fill = 0;  // queue length
    int front = 0; // queue head
    int rear = 0;  // queue tail

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
    *contextSwitches = 0;

    /* ----------------------------- algorithm body ----------------------------- */

    // while there is still remaining job then continue looping
    while (remainingProcesses)
    {
        // add new arrival to ready queue
        for (int i = 0; i < numberOfProcess; i++)
        {
            if (processData[arrivalTime][i] > previousTimeFrame && processData[arrivalTime][i] <= currentTimeFrame)
            {
                enque(numberOfProcess, readyQueue, front, &rear, &fill, i);
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

        // start counting current time frame
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
        if (previousWorkingIndex != workingProcessIndex && currentTimeFrame != processData[arrivalTime][0])
        {
            currentTimeFrame += contextSwitchingTime;
            *contextSwitches += 1;
        }

        if (reducingBurstTime[workingProcessIndex] == processData[burstTime][workingProcessIndex])
        {
            processData[respondTime][workingProcessIndex] = currentTimeFrame - processData[arrivalTime][workingProcessIndex];
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

        // if a process is done
        if (doneProcessBoolean)
        {
            processData[completionTime][workingProcessIndex] = currentTimeFrame;
            doneProcessBoolean = 0;
        }
    }

    /* ---------------------------- calculate output ---------------------------- */

    selectiveSortProcess(numberOfProcess, processData, processID);

    for (int i = 0; i < numberOfProcess; i++)
    {
        processData[turnaroundTime][i] = processData[completionTime][i] - processData[arrivalTime][i];
        processData[waitingTime][i] = processData[turnaroundTime][i] - processData[burstTime][i];
    }
}

/* -------------------------------------------------------------------------- */
/*                             shortest job first                             */
/* -------------------------------------------------------------------------- */

void sjf(
    int numberOfProcess,
    int processData[NUM_PROCESSDATATYPE][numberOfProcess],
    int contextSwitchingTime,
    int* contextSwitches
)
{

    // prototype
    void selectiveSortProcess(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], processDataType type);

    // selection sort
    selectiveSortProcess(numberOfProcess, processData, burstTime);

    // algorithm
    fcfs(numberOfProcess, processData, contextSwitchingTime, contextSwitches);
}

/* -------------------------------------------------------------------------- */
/*                           first come first serve                           */
/* -------------------------------------------------------------------------- */

void fcfs(
    int numberOfProcess,
    int processData[NUM_PROCESSDATATYPE][numberOfProcess],
    int overheadTime,
    int* overheadCount
    )
{
    void selectiveSortProcess(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], processDataType type);

    //TODO completionTime = arrival time + turn time, arrivalTime
 
    selectiveSortProcess(numberOfProcess, processData, arrivalTime);

    processData[respondTime][0] = processData[arrivalTime][0];
    processData[completionTime][0] = processData[burstTime][0] + processData[arrivalTime][0];

    for (int i = 1; i < numberOfProcess; i++){     
       
        if(processData[completionTime][i-1] > processData[arrivalTime][i]){
            processData[completionTime][i] = processData[completionTime][i-1] + processData[burstTime][i];
        } else {
            processData[completionTime][i] = processData[arrivalTime][i] + processData[burstTime][i];
        }     
        processData[completionTime][i] += overheadTime;
        processData[respondTime][i] = processData[completionTime][i-1];
        *overheadCount += 1;
    }

    for (int i = 0; i < numberOfProcess; i++)
    {
        processData[turnaroundTime][i] = processData[completionTime][i] - processData[arrivalTime][i];
        processData[waitingTime][i] = processData[turnaroundTime][i] - processData[burstTime][i];
    }

    //------------------------------------------
     
    // processData[waitingTime][0] = 0;

    // for (int i = 1; i < numberOfProcess; i++)
    // {
    //     processData[waitingTime][i] = 0;
    //     for (int j = 0; j < i; j++)
    //         processData[waitingTime][i] += processData[burstTime][j];
    // }

    // for (int i = 0; i < numberOfProcess; i++)
    // {
    //     processData[respondTime][i] = i == 0 ? 0 : processData[turnaroundTime][i - 1];
    //     processData[turnaroundTime][i] = processData[burstTime][i] + processData[waitingTime][i];
    //     processData[completionTime][i] = processData[turnaroundTime][i] + processData[arrivalTime][i];
    // }

    

}

/* -------------------------------------------------------------------------- */
/*                              display function                              */
/* -------------------------------------------------------------------------- */

void display2(
    int numberOfProcess,
    int processData[NUM_PROCESSDATATYPE][numberOfProcess],
    int contextSwitches)
{

    int max(int size, int array[size]);

    double averageTurnaroundTime = 0;
    double averageWaitingTime = 0;
    double averageRespondTime = 0;
    double cpuUsage = 0;

    // add all together
    for (int i = 0; i < numberOfProcess; i++)
    {
        averageTurnaroundTime += processData[turnaroundTime][i];
        averageWaitingTime += processData[waitingTime][i];
        averageRespondTime += processData[respondTime][i];
        cpuUsage += processData[burstTime][i];
    }

    averageWaitingTime = (double)averageWaitingTime / numberOfProcess;
    averageTurnaroundTime = (double)averageTurnaroundTime / numberOfProcess;
    averageRespondTime = (double)averageRespondTime / numberOfProcess;
    cpuUsage = (double)cpuUsage / max(numberOfProcess, processData[completionTime]) * 100;

    //TODO printf("Algorithms: %s", )
    printf("\nProcess ID\tArrival Time\t Burst Time\t Turnaround Time\t Waiting Time\t Respond Time\t Completion Time");

    for (int i = 0; i < numberOfProcess; i++)
    {
        printf("\nProcess[%d]\t%d\t\t %d\t\t %d\t\t\t %d\t\t %d\t\t %d",
               processData[processID][i],
               processData[arrivalTime][i],
               processData[burstTime][i],
               processData[turnaroundTime][i],
               processData[waitingTime][i],
               processData[respondTime][i],
               processData[completionTime][i]);
    }

    printf("\n\nAverage Turnaround Time:\t%.2f", averageTurnaroundTime);
    printf("\nAverage Waiting Time:\t\t%.2f", averageWaitingTime);
    printf("\nAverage Respond Time:\t\t%.2f", averageRespondTime);
    printf("\n\nContext Switches:\t\t%d", contextSwitches);
    printf("\nCPU Utilization:\t\t%.2f %%\n\n", cpuUsage);
}



void display(
    int numberOfProcess,
    int processData[NUM_PROCESSDATATYPE][numberOfProcess],
    double averageWaitingTime,
    double averageTurnaroundTime)
{

    printf("\nProcess\t\tStart Time\tBurst Time\tWaiting Time\tTurnaround Time");

    for (int k = 0; k < numberOfProcess; k++)
    {
        printf("\nP[%d]\t\t%d\t\t%d\t\t%d\t\t%d", processData[processID][k], processData[respondTime][k], processData[burstTime][k], processData[waitingTime][k], processData[turnaroundTime][k]);
    }

    printf("\n\nAverage Waiting Time: %.2f \n", averageWaitingTime);
    printf("Average Turnaround Time: %.2f \n\n", averageTurnaroundTime);
    printf("CPU Usage: %.2f%% \n\n", ((double)numberOfProcess / CPU_CAPABILITY) * 100);
}

void reset(int size, int array[NUM_PROCESSDATATYPE][size]){
    for (int i = 0; i < NUM_PROCESSDATATYPE; i++)
    {
        for (int j = 0; j < size; j++)
        {
            array[i][j] = 0;
        }
    }
}

/* -------------------------------------------------------------------------- */
/*                               helper function                              */
/* -------------------------------------------------------------------------- */

int max(int size, int array[size]){
    int maxNum = array[0];

    for (int i = 1; i < size; i++)
    {
        if (array[i] > maxNum)
        {
            maxNum = array[i];
        }
    }
    return maxNum;
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
    void shiftProcessData(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], int start, int end);

    // variable
    int smallestValueIndex;
    int temp[NUM_PROCESSDATATYPE];

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
            for (int k = 0; k < NUM_PROCESSDATATYPE; k++)
            {
                temp[k] = processData[k][smallestValueIndex];
            }
            shiftProcessData(numberOfProcess, processData, i, smallestValueIndex);
            for (int k = 0; k < NUM_PROCESSDATATYPE; k++)
            {
                processData[k][i] = temp[k];
            }
            
        }
    }
}

void shiftProcessData(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], int start, int end){
    for (int i = end; i > start; i--)
    {
        for (int j = 0; j < NUM_PROCESSDATATYPE; j++)
        {
            processData[j][i] = processData[j][i-1];
        }
    }    
}