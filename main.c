
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
#include <string.h>

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

typedef enum
{
    att = 0,
    awt = 1,
    art = 2,
    cu = 3,
    score = 4,
    NUM_RESULTTYPE
} resultType;

typedef enum
{
    firstComeFirstServe = 0,
    shortestJobFirst = 1,
    roundRobin = 2,
    NUM_ALGORITHMSTYPE
} algorithmsType;

/* -------------------------------- prototype ------------------------------- */

void rr(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], int timeQuantum, int contextSwitchingTime, int *contextSwitches);
void fcfs(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], int overheadTime, int *overheadCount);
void sjf(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], int contextSwitchingTime, int *contextSwitches);
void display(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], int contextSwitches);
void reset(int size, int array[NUM_PROCESSDATATYPE][size]);
void calculateAverage(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], double result[NUM_RESULTTYPE]);
char* returnBestAlgorithms(double fcfsResult[NUM_RESULTTYPE], double sjfResult[NUM_RESULTTYPE], double rrResult[NUM_RESULTTYPE]);

/* -------------------------------------------------------------------------- */
/*                                main function                               */
/* -------------------------------------------------------------------------- */

int main()
{
    // input variable
    int timeQuantum;
    int contextSwitchingTime;
    int numberOfProcess, j;
    int contextSwitches = 0;
    double fcfsResult[NUM_RESULTTYPE];
    double sjfResult[NUM_RESULTTYPE];
    double rrResult[NUM_RESULTTYPE];

    while (1)
    {
        printf("\n\n=== PROCESS SCHEDULING ===\n\n");

        do
        {
            printf("Enter total number of processes:");
            scanf("%d", &numberOfProcess);
        } while (numberOfProcess <= 0 ? printf("!! Invalid process number !!\n\n") : 0);

        int processData[NUM_PROCESSDATATYPE][numberOfProcess];
        int fcfsProcessData[NUM_PROCESSDATATYPE][numberOfProcess];
        int sjfProcessData[NUM_PROCESSDATATYPE][numberOfProcess];
        int rrProcessData[NUM_PROCESSDATATYPE][numberOfProcess];

        reset(numberOfProcess, processData);

        for (int i = 0; i < numberOfProcess; i++)
        {
            processData[processID][i] = i + 1;

            printf("\nEnter Details of Process[%d]\n", processData[processID][i]);

            do
            {
                printf("Arrival Time:\t");
                scanf("%d", &processData[arrivalTime][i]);
            } while (processData[arrivalTime][i] < 0 ? printf("!! Invalid arrival time !!\n\n") : 0);

            do
            {
                printf("Burst Time:\t");
                scanf("%d", &processData[burstTime][i]);
            } while (processData[burstTime][i] < 0 ? printf("!! Invalid burst time !!\n\n") : 0);
        }

        do
        {
            printf("\nEnter Time Quantum: \t");
            scanf("%d", &timeQuantum);
        } while (timeQuantum <= 0 ? printf("!! Please enter a number that is > 0 !!\n\n") : 0);

        do
        {
            printf("Enter context switching time:\t");
            scanf("%d", &contextSwitchingTime);
        } while (contextSwitchingTime < 0 ? printf("!! Please enter a number above 0 !!") : 0);

        memcpy(fcfsProcessData, processData, NUM_PROCESSDATATYPE * numberOfProcess * sizeof(int));
        memcpy(sjfProcessData, processData, NUM_PROCESSDATATYPE * numberOfProcess * sizeof(int));
        memcpy(rrProcessData, processData, NUM_PROCESSDATATYPE * numberOfProcess * sizeof(int));

        printf("\n\n- FIRST COME FIRST SERVE SCHEDULING ALGORITHMS -\n");
        fcfs(numberOfProcess, fcfsProcessData, contextSwitchingTime, &contextSwitches);
        display(numberOfProcess, fcfsProcessData, contextSwitches);
        contextSwitches = 0;

        printf("\n\n- SHORTEST JOB FIRST SCHEDULING ALGORITHMS -\n");
        sjf(numberOfProcess, sjfProcessData, contextSwitchingTime, &contextSwitches);
        display(numberOfProcess, sjfProcessData, contextSwitches);
        contextSwitches = 0;

        printf("\n\n- ROUND ROBIN SCHEDULING ALGORITHMS -\n");
        rr(numberOfProcess, rrProcessData, timeQuantum, contextSwitchingTime, &contextSwitches);
        display(numberOfProcess, rrProcessData, contextSwitches);
        contextSwitches = 0;

        calculateAverage(numberOfProcess, fcfsProcessData, fcfsResult);
        calculateAverage(numberOfProcess, sjfProcessData, sjfResult);
        calculateAverage(numberOfProcess, rrProcessData, rrResult);

        printf("\nThe Best Scheduling Algorithms for this workload is(are) : %s\n", returnBestAlgorithms(fcfsResult, sjfResult, rrResult));
    }
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
    int workingProcessIndex = EMPTY;      // current working process index
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
    int *contextSwitches)
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
    int *overheadCount)
{
    void selectiveSortProcess(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], processDataType type);

    //TODO completionTime = arrival time + turn time, arrivalTime

    selectiveSortProcess(numberOfProcess, processData, arrivalTime);

    processData[respondTime][0] = processData[arrivalTime][0];
    processData[completionTime][0] = processData[burstTime][0] + processData[arrivalTime][0];

    for (int i = 1; i < numberOfProcess; i++)
    {

        if (processData[completionTime][i - 1] > processData[arrivalTime][i])
        {
            processData[completionTime][i] = processData[completionTime][i - 1] + processData[burstTime][i];
        }
        else
        {
            processData[completionTime][i] = processData[arrivalTime][i] + processData[burstTime][i];
        }
        processData[completionTime][i] += overheadTime;
        processData[respondTime][i] = processData[completionTime][i - 1];
        *overheadCount += 1;
    }

    for (int i = 0; i < numberOfProcess; i++)
    {
        processData[turnaroundTime][i] = processData[completionTime][i] - processData[arrivalTime][i];
        processData[waitingTime][i] = processData[turnaroundTime][i] - processData[burstTime][i];
    }
}

/* -------------------------------------------------------------------------- */
/*                              display function                              */
/* -------------------------------------------------------------------------- */

void display(
    int numberOfProcess,
    int processData[NUM_PROCESSDATATYPE][numberOfProcess],
    int contextSwitches)
{

    int maxInt(int size, int array[size]);

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
    cpuUsage = (double)cpuUsage / maxInt(numberOfProcess, processData[completionTime]) * 100;

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

void reset(int size, int array[NUM_PROCESSDATATYPE][size])
{
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

int maxInt(int size, int array[size])
{
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

double max(int size, double array[size])
{
    double maxNum = array[0];

    for (int i = 1; i < size; i++)
    {
        if (array[i] > maxNum)
        {
            maxNum = array[i];
        }
    }
    return maxNum;
}

double min(int size, double array[size])
{
    double minNum = array[0];

    for (int i = 1; i < size; i++)
    {
        if (array[i] < minNum)
        {
            minNum = array[i];
        }
    }
    return minNum;
}

int minIndex(int size, int array[size])
{
    int minIndex = 0;

    for (int i = 1; i < size; i++)
    {
        if (array[i] < array[minIndex])
        {
            minIndex = i;
        }
    }
    return minIndex;
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

void shiftProcessData(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], int start, int end)
{
    for (int i = end; i > start; i--)
    {
        for (int j = 0; j < NUM_PROCESSDATATYPE; j++)
        {
            processData[j][i] = processData[j][i - 1];
        }
    }
}

void calculateAverage(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], double result[NUM_RESULTTYPE])
{
    int maxInt(int size, int array[size]);

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

    result[awt] = (double)averageWaitingTime / numberOfProcess;
    result[att] = (double)averageTurnaroundTime / numberOfProcess;
    result[art] = (double)averageRespondTime / numberOfProcess;
    result[cu] = (double)cpuUsage / maxInt(numberOfProcess, processData[completionTime]) * 100;
}

char* returnBestAlgorithms(double fcfsResult[NUM_RESULTTYPE], double sjfResult[NUM_RESULTTYPE], double rrResult[NUM_RESULTTYPE])
{
    void addScore(double temp[NUM_ALGORITHMSTYPE], double fcfsResult[NUM_ALGORITHMSTYPE], double sjfResult[NUM_RESULTTYPE], double rrResult[NUM_RESULTTYPE], double value);
    double max(int size, double array[size]);
    double min(int size, double array[size]);

    double temp[NUM_ALGORITHMSTYPE];
    double value;
    int outputBoolean[NUM_ALGORITHMSTYPE];
    char* msg = "";

    for (int i = 0; i < NUM_RESULTTYPE; i++)
    {
        for (int k = 0; k < NUM_ALGORITHMSTYPE; k++)
        {
            switch (k)
            {
            case firstComeFirstServe:
                temp[k] = fcfsResult[i];
                break;
            case shortestJobFirst:
                temp[k] = sjfResult[i];
                break;
            case roundRobin:
                temp[k] = rrResult[i];
                break;
            }
        }

        switch (i)
        {
        case cu:
            value = max(NUM_ALGORITHMSTYPE, temp);
            addScore(temp, fcfsResult, sjfResult, rrResult, value);
            break;
        case score:
            value = max(NUM_ALGORITHMSTYPE, temp);
            break;
        default:
            value = min(NUM_ALGORITHMSTYPE, temp);
            addScore(temp, fcfsResult, sjfResult, rrResult, value);
            break;
        }
    }

    for (int i = 0; i < NUM_ALGORITHMSTYPE; i++)
    {
        if (value == temp[i])
        {
            outputBoolean[i] = 1;
            switch (i)
            {
            case firstComeFirstServe:
                strcat(msg, " fcfs");
                break;
            case shortestJobFirst:
                strcat(msg, " sjf");
                break;
            case roundRobin:
                strcat(msg, " rr");
                break;
            }
        }
    } 

    return msg;
}

void addScore(double temp[NUM_ALGORITHMSTYPE], double fcfsResult[NUM_ALGORITHMSTYPE], double sjfResult[NUM_RESULTTYPE], double rrResult[NUM_RESULTTYPE], double value)
{
    for (int i = 0; i < NUM_ALGORITHMSTYPE; i++)
    {
        if (temp[i] == value)
        {
            switch (i)
            {
            case 0:
                fcfsResult[score] += 1;
                break;
            case 1:
                sjfResult[score] += 1;
                break;
            case 2:
                rrResult[score] += 1;
                break;
            }
        }
    }
}