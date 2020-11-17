#include<stdio.h>
 
int main()
{
      int processIndex, limit, endingTime = 0, numberOfProcess, displayBoolean = 0, timeQuantum;
      int totalWaitingTime = 0, totalTurnaroundTime = 0, arrival_time[10], burst_time[10], temp[10];
      float average_wait_time, average_turnaround_time;
      printf("\nEnter Total Number of Processes:\t");
      scanf("%d", &limit);
      numberOfProcess = limit;
      //take input
      for(processIndex = 0; processIndex < limit; processIndex++)
      {
            printf("\nEnter Details of Process[%d]\n", processIndex + 1);
 
            printf("Arrival Time:\t");
 
            scanf("%d", &arrival_time[processIndex]);
 
            printf("Burst Time:\t");
 
            scanf("%d", &burst_time[processIndex]);
 
            temp[processIndex] = burst_time[processIndex];
      }
 
      printf("\nEnter Time Quantum:\t");
      scanf("%d", &timeQuantum);
      printf("\nProcess ID\t\tBurst Time\t Turnaround Time\t Waiting Time\n");

      //algorithms
      for(endingTime = 0, processIndex = 0; numberOfProcess != 0;)
      {
            // if current burst time <= timequantum and current burst time > 0
            if(temp[processIndex] <= timeQuantum && temp[processIndex] > 0)
            {
                  //progress time (can't add timeQuantum because in this condition burst time is smaller than timeQuantum)
                  endingTime = endingTime + temp[processIndex];
                  //set burst time that is smaller than timeQuantum to 0
                  temp[processIndex] = 0;
                  // set display boolean
                  displayBoolean = 1;
            }
            else if(temp[processIndex] > 0)
            {
                  //compute processIndex(reduce burst time)
                  temp[processIndex] = temp[processIndex] - timeQuantum;
                  //progress time
                  endingTime = endingTime + timeQuantum;
            }

            // if burst time is 0 and displayBoolean is true
            if(temp[processIndex] == 0 && displayBoolean == 1)
            {
                  numberOfProcess--;
                  printf("\nProcess[%d]\t\t%d\t\t %d\t\t\t %d", processIndex + 1, burst_time[processIndex], endingTime - arrival_time[processIndex], endingTime - arrival_time[processIndex] - burst_time[processIndex]);
                  totalWaitingTime = totalWaitingTime + endingTime - arrival_time[processIndex] - burst_time[processIndex];
                  totalTurnaroundTime = totalTurnaroundTime + endingTime - arrival_time[processIndex];
                  displayBoolean = 0;
            }

            //round robin algorithms
            //if processIndex == last index of all processes
            if(processIndex == limit - 1)
            {
                //go back to start (circular array[0,1,2,0,1,2.....])
                  processIndex = 0;
            }
            // if next process already arrived then moves on to next process;
            else if(arrival_time[processIndex + 1] <= endingTime)
            {
                  processIndex++;
            }
            // //go back to start because no process is waiting
            else
            {
                  processIndex = 0;
            }
      }
 
      average_wait_time = totalWaitingTime * 1.0 / limit;
      average_turnaround_time = totalTurnaroundTime * 1.0 / limit;
      printf("\n\nAverage Waiting Time:\t%f", average_wait_time);
      printf("\nAvg Turnaround Time:\t%f\n", average_turnaround_time);
      return 0;
}


//       P1   P2   P3   P4
//     |WORK|WAIT| NA |WAIT|
//     |WAIT|WORK|WAIT|WAIT|
//     |WAIT|WAIT|WORK|WAIT|
//     |WORK|WAIT|WAIT|WAIT|
//     |WORK|WAIT|WAIT|WAIT|
//     |WORK|WAIT|WAIT|WAIT|

// waitingCounter[num processes]
// waitingCounter[P1] += timeQuantum(5)
// [5,0,0,0]
// [10,0,0,0]
// turnaroundTimeCounter

// work_arr = p1, p2, p3, p2, p3...

// temparr[4] = 1, 0, 2, 0;
// for processIndex in workarr
      
      

// temparr[p1] set work

// for j in notworkarr
//       currenttime < arrivalTime print NA else print wait


// print temparr


// for status in temparr
//       switch status
//             case 1
//                   waitingarr[]+...
//             case 2
//                   working+....
