#include <stdio.h>


// typedef enum
// {
//     processID = 0,
//     startTime = 1,
//     burstTime = 2,
//     waitingTime = 3,
//     turnaroundTime = 4,
//     arrivalTime = 5,
//     NUM_PROCESSDATATYPE
// } processDataType;


void removeArrayElement(int array[], int index, int* end);

int main()
{
    //input
    int timeQuantum;
    int numberOfProcess;

    printf("\nEnter Total Number of Processes:\t");
    scanf("%d", &numberOfProcess);

    int arrivalTime[numberOfProcess];
    int burstTime[numberOfProcess];
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    int waitingTime[numberOfProcess];
    int turnaroundTime[numberOfProcess];

    
    //inner loop variable
    int currentTimeFrame = 0; // current time
    int previousTimeFrame = -timeQuantum; // previous current time
    int overHead = 0;
    int remainingProcesses = numberOfProcess;
    int rrIndex = 0;// round robin value
    int workingProcessIndex = -1;// current working process index
    int arrivedProcessesCounter = 0;// index of (int arrivedProcessesIndex[numberOfProcess];) variable
    int arrivedProcessesIndex[numberOfProcess];// add or remove the index of process to list if arrive or done
    int reducingBurstTime[numberOfProcess];// gradually reduce burst time to 0 and set it as done
    int doneProcessBoolean = 0;

    //output
    double averageWaitTime = 0;
    double averageTurnaroundTime = 0;

    //take input
    for (int i = 0; i < numberOfProcess; i++)
    {
        printf("\nEnter Details of Process[%d]\n", i + 1);

        printf("Arrival Time:\t");

        scanf("%d", &arrivalTime[i]);

        printf("Burst Time:\t");

        scanf("%d", &burstTime[i]);

        reducingBurstTime[i] = burstTime[i];
    }

    printf("\nEnter Time Quantum:\t");
    scanf("%d", &timeQuantum);
 
    printf("\nEnter overhead:\t");
    scanf("%d", &overHead);

    printf("\nProcess ID\t\tBurst Time\t Turnaround Time\t Waiting Time\n");

    //initialize variable 
    //initialize all processes
    for (int i = 0; i < numberOfProcess; i++)
    {
        // initialize all arrived process index
        // arrivedProcessesIndex[i] = not_arrived;   
        turnaroundTime[i] = 0;
        waitingTime[i] = 0;     
    }
    
    /// while there is still remaining job then continue looping
    while (remainingProcesses)
    {
        /// check for arrival and update arrival list
        for (int i = 0; i < numberOfProcess; i++)
        {
            // if arrival comes after previous time and before or equql to current time then register new arrival
            if (arrivalTime[i] > previousTimeFrame && arrivalTime[i] <= currentTimeFrame)
            {
                //store process index of the arrived job
                arrivedProcessesIndex[arrivedProcessesCounter] = i;
                arrivedProcessesCounter++;
            }
        }

        previousTimeFrame = currentTimeFrame;

        /// round robin
        // if (only one process) or (this is the first time frame) then disable round robin
        if (((arrivedProcessesCounter == 1) && (arrivedProcessesIndex[rrIndex] == workingProcessIndex))|| 
           (arrivedProcessesCounter > 1 && currentTimeFrame == 0))           
        {
            rrIndex = 0;
            workingProcessIndex = arrivedProcessesIndex[rrIndex];
        } // starting from the second time frame if arrived more than one job then begin round robin
        else if (arrivedProcessesIndex[rrIndex] != workingProcessIndex)
        {
            rrIndex = 0;
            workingProcessIndex = arrivedProcessesIndex[rrIndex];
            currentTimeFrame += overHead;
        }
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
                turnaroundTime[arrivedProcessesIndex[i]] += currentTimeFrame - previousTimeFrame;
            }

            // add waiting time to all already arrived but not working jobs
            for (int i = 0; i < arrivedProcessesCounter; i++)
            {
                if (arrivedProcessesIndex[i] != workingProcessIndex)
                {
                    waitingTime[arrivedProcessesIndex[i]] += currentTimeFrame - previousTimeFrame;
                }
            }
            
            if (doneProcessBoolean)
            {
                removeArrayElement(arrivedProcessesIndex, rrIndex, &arrivedProcessesCounter);
                // output value
                printf("\nProcess[%d]\t\t%d\t\t %d\t\t\t %d", workingProcessIndex + 1, burstTime[workingProcessIndex], turnaroundTime[workingProcessIndex], waitingTime[workingProcessIndex]);
                doneProcessBoolean = 0;
            }
            
            
        } else
        {
            currentTimeFrame += timeQuantum;
        }
        

    }

    for (int i = 0; i < numberOfProcess; i++)
    {
        totalTurnaroundTime += turnaroundTime[i];
        totalWaitingTime += waitingTime[i];
    }
    
    averageWaitTime = (double) totalWaitingTime / numberOfProcess;
    averageTurnaroundTime = (double) totalTurnaroundTime / numberOfProcess;

    printf("\n\nAverage Waiting Time:\t%.2f", averageWaitTime);
    printf("\nAverage Turnaround Time:\t%.2f\n", averageTurnaroundTime);

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
    
}