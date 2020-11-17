
/* count3s_runner.c
  * main routine to be linked with count3s. * Will initialise array and length, call count3s  * and look in count for the result.*/
/* standard headers */

// link compile & run
/* 
gcc count3s.c count3s_runner.c -o countrunner;./countrunner
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

/* these should be defined in count3s.c */
extern int *array;
extern int length;
extern int count;
extern int count3s();

/* constant - 30% of RAND_MAX; Note: integer arithmetic */
#define RAND_SMALL (RAND_MAX / 10 * 3)

/* the main program... */
int main(int argc, char **argv)
{
    /* all local variables */
    int i;
    struct timeval start, end;

    length = 50000000;

    /* length as an argument? (optional) */
    if (argc >= 2)
        length = atoi(argv[1]);

    printf("Using length %d\n", length);

    /* allocate array of data */
    array = (int *)malloc(length * sizeof(int));

    /* initialise array at random with some 3s */
    for (i = 0; i < length; i++)
    {
        int r = rand();
        if (r < RAND_SMALL)
            array[i] = 3;
        else
            array[i] = 0;
    }
    printf("Counting...\n");
    gettimeofday(&start, NULL);
    /* do the work */
    count3s();
    /* and the answer is... */
    if (gettimeofday(&end, NULL) == 0)
    {
        double elapsed = (end.tv_sec - start.tv_sec) + 0.000001 * (end.tv_usec - start.tv_usec);
        printf("Elapsed time %lf seconds\n", elapsed);
    }
    else
    {
        printf("Could not get time using clock_gettime\n");
    }
    printf("Found %d / %d were '3'\n", count, length);

    /* no error */
    return 0;
}
/* end of file */
