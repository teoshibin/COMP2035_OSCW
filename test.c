#include <stdio.h>

int main(int argc, char const *argv[])
{
    int arr[5] = {9,5,2,1,6};
    int temp;
    int smallestValueIndex;
    
    for (int i = 0; i < 5; i++)
    {
        smallestValueIndex = i;
        int j;

        for (j = i + 1; j < 5; j++)
        {
            if (arr[j] < arr[i])
                smallestValueIndex = j;
        }

        // swap if current burst time is not the smallest burst time
        if (smallestValueIndex != i)
        {
            temp = arr[i];
            arr[i] = arr[smallestValueIndex];
            arr[j] = temp;
        }
    }


    return 0;
}




