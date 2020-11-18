#include <stdio.h>

void shiftArrayLeft(int array[], int index, int* end);

int main(int argc, char const *argv[])
{
    int num = 4;

    switch (num)
    {
    case 4:
    case 5:
        printf("hello");
        break;
    
    default:
        printf("hi");
        break;
    }
    return 0;
}

void shiftArrayLeft(int array[], int index, int* end){
    for (int i = index; i < *end - 1; i++)
    {
        printf("%d %d\n", i, *end);
        if ((i + 1) != *end)
        {
            array[i] = array[i + 1];
        }
    }
    array[*end - 1] = 0;
    *end -= 1;
}