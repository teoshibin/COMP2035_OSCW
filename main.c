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
void rr(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess],
        int timeQuantum, int overHead, int *averageWaitingTime, int *averageTurnaroundTime);

void fcfs(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess],
          int *averageWaitingTime, int *averageTurnAroundTime);
          
void sjf(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess],
         int *averageWaitingTime, int *averageTurnAroundTime);

void display(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess],
             int averageWaitingTime, int averageTurnAroundTime);

// main function
int main()
{
    // input variable
    int timeQuantum;
    int overHead;
    int numberOfProcess, j;
    int averageWaitingTime = 0;
    int averageTurnaroundTime = 0;

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
 
    printf("\nEnter overhead:\t");
    scanf("%d", &overHead);

    printf("\nProcess ID\t\tBurst Time\t Turnaround Time\t Waiting Time\n");


    // fcfs algorithms
    // fcfs(numberOfProcess, processData, &averageWaitingTime, &averageTurnaroundTime);
    // sjf(numberOfProcess, processData, &averageWaitingTime, &averageTurnaroundTime);
    rr(numberOfProcess, processData, timeQuantum, overHead, &averageWaitingTime, &averageTurnaroundTime);

    // display function
    display(numberOfProcess, processData, averageWaitingTime, averageTurnaroundTime);

    return 0;
}

void rr(int numberOfProcess, int processData[NUM_PROCESSDATATYPE][numberOfProcess], int timeQuantum, int overHead, int* averageWaitingTime, int* averageTurnaroundTime){

    //-----------------------------------------private method--------------------------------------------
    void removeArrayElement(int array[], int index, int* end){
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

    //--------------------------private variables initialization--------------------------
    int currentTimeFrame = 0; // current time
    int previousTimeFrame = -timeQuantum; // previous current time
    int remainingProcesses = numberOfProcess;
    int rrIndex = 0;// round robin value
    int workingProcessIndex = -1;// current working process index
    int arrivedProcessesCounter = 0;// index of (int arrivedProcessesIndex[numberOfProcess];) variable
    int arrivedProcessesIndex[numberOfProcess];// add or remove the index of process to list if arrive or done
    int reducingBurstTime[numberOfProcess];// gradually reduce burst time to 0 and set it as done
    int doneProcessBoolean = 0;
    int totalTurnaroundTime = 0;
    int totalWaitingTime = 0;
    //--------------------------------------------RR Algorithms-------------------------------------------

    // copy burst time to temp array
    for (int i = 0; i < numberOfProcess; i++){
        reducingBurstTime[i] = processData[burstTime][i];
    }

    // initialize all arrived process index
    for (int i = 0; i < numberOfProcess; i++)
    {
        processData[turnaroundTime][i] = 0;
        processData[waitingTime][i] = 0;
    }
    
    // while there is still remaining job then continue looping
    while (remainingProcesses)
    {
        /// check for arrival and update arrival list
        for (int i = 0; i < numberOfProcess; i++)
        {
            // if arrival comes after previous time and before or equql to current time then register new arrival
            if (processData[arrivalTime][i] > previousTimeFrame && processData[arrivalTime][i] <= currentTimeFrame)
            {
                //store process index of the arrived job
                arrivedProcessesIndex[arrivedProcessesCounter] = i;
                arrivedProcessesCounter++;
            }
        }

        previousTimeFrame = currentTimeFrame;

        /// round robin
        // if only one job and same job as previous quantum time
        // if more than one job and it is not the initial time frame
        // then do not do round robin and do not add overhead
        if (((arrivedProcessesCounter == 1) && (arrivedProcessesIndex[rrIndex] == workingProcessIndex))|| 
           (arrivedProcessesCounter > 1 && currentTimeFrame == 0))           
        {
            rrIndex = 0;
            workingProcessIndex = arrivedProcessesIndex[rrIndex];
        } // previous process not equal to current process then add overhead
        else if (arrivedProcessesIndex[rrIndex] != workingProcessIndex)
        {
            rrIndex = 0;
            workingProcessIndex = arrivedProcessesIndex[rrIndex];
            currentTimeFrame += overHead;
        } // if more than one job and not initial time frame then add overhead and start round robin
        else if (arrivedProcessesCounter > 1 && currentTimeFrame != 0)
        {      
            // only increament rrIndex if same numbers of process is running in previous iteration
            // bcs when rrIndex is larger than counter meaning a process is removed from the arrivedProcess list
            // if we increament and mod it with different numbers of jobs than last time it will not go back to 0
            // when rrindex equal to coutner, mod(rrindex, counter) will turn rrindex back to 0
            rrIndex++;
            rrIndex %= arrivedProcessesCounter;// circular round robin 0,1,2,0,1,2
            workingProcessIndex = arrivedProcessesIndex[rrIndex];
            currentTimeFrame += overHead;
        }

        /// processing
        // if there is atleast one arrived job
        if (arrivedProcessesCounter > 0) 
        {           
            // if burst time smaller than time quantum
            if (reducingBurstTime[workingProcessIndex] <= timeQuantum)
            {
                // adjust time
                currentTimeFrame += reducingBurstTime[workingProcessIndex];

                // reduce burst time
                reducingBurstTime[workingProcessIndex] = 0;

                // remove done process from arrived list
                // removeArrayElement(arrivedProcessesIndex, rrIndex, &arrivedProcessesCounter);

                remainingProcesses--;
                doneProcessBoolean = 1;

            } else if (reducingBurstTime[workingProcessIndex] > timeQuantum)
            {
                // adjust time
                currentTimeFrame += timeQuantum;

                // reduce burst time
                reducingBurstTime[workingProcessIndex] -= timeQuantum;

            }

            // add turnaround time to all arrived job
            for (int i = 0; i < arrivedProcessesCounter; i++)
            {
                processData[turnaroundTime][arrivedProcessesIndex[i]] += currentTimeFrame - previousTimeFrame;
            }

            // add waiting time to all already arrived but not working jobs
            for (int i = 0; i < arrivedProcessesCounter; i++)
            {
                if (arrivedProcessesIndex[i] != workingProcessIndex)
                {
                    processData[waitingTime][arrivedProcessesIndex[i]] += currentTimeFrame - previousTimeFrame;
                }
            }
            
            if (doneProcessBoolean)
            {
                removeArrayElement(arrivedProcessesIndex, rrIndex, &arrivedProcessesCounter);
                // output value
                printf("\nProcess[%d]\t\t%d\t\t %d\t\t\t %d",
                        workingProcessIndex + 1,
                        processData[burstTime][workingProcessIndex],
                        processData[turnaroundTime][workingProcessIndex],
                        processData[waitingTime][workingProcessIndex]);
                doneProcessBoolean = 0;
            }            
        } else
        {
            currentTimeFrame += timeQuantum;
        }
        

    }

    for (int i = 0; i < numberOfProcess; i++)
    {
        totalTurnaroundTime += processData[turnaroundTime][i];
        totalWaitingTime += processData[waitingTime][i];
    }
    
    *averageWaitingTime = (double) totalWaitingTime / numberOfProcess;
    *averageTurnaroundTime = (double) totalTurnaroundTime / numberOfProcess;

    printf("\n\nAverage Waiting Time:\t%.2f", *averageWaitingTime);
    printf("\nAverage Turnaround Time:\t%.2f\n", *averageTurnaroundTime);

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