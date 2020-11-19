// circular array

#include <stdio.h>

void enque(int size, int array[size], int front, int *rear, int *fill, int value);
int deque(int size, int array[size], int *front, int *fill, int rear);

#define EMPTY -1;

int main(int argc, char const *argv[])
{

    int op;
    int value;
    int array[5];
    int front = 0;
    int rear = 0;
    int fill = 0;

    for (int i = 0; i < 5; i++)
    {
        array[i] = EMPTY;
    }
    
    while (1)
    {
        printf("op : ");
        scanf("%d", &op);

        switch (op)
        {
        case 0:
            printf("value : ");
            scanf("%d", &value);
            printf("\n");
            enque(5, array, front, &rear, &fill, value);
            break;
        case 1:
            value = deque(5, array, &front, &fill, rear);
            printf("get value : %d\n", value);
            break;
        }

        printf("fill : %d\n", fill);
        for (int i = 0; i < 5; i++)
        {
            printf("%d ", array[i]);
        }
        printf("\n\n");
    }

    return 0;
}

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

int deque(int size, int array[size], int *front, int *fill, int rear)
{
    int value = EMPTY;
    if (*fill)
    {
        value = array[*front];
        array[*front] = -1;
        *front += 1;
        *front %= size;
        *fill -= 1;
    }
    return value;
}