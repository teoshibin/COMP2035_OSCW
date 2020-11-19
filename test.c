#include <stdio.h>

void add(int array[5], int *front, int *rear, int value);
int getFirst(int array[5], int *front, int *rear);

int main(int argc, char const *argv[])
{
    int value = 0;
    int counter = 0;
    int array[5] = {0};
    int question = 0;
    int front = 0;
    int rear = 0;

    while (1)
    {
        printf("question : ");
        scanf("%d", &question);

        switch (question)
        {
        case 0:
            printf("value : ");
            scanf("%d", &value);
            add(array, &front, &rear, value);
            break;
        case 1:
            value = getFirst(array, &front, &rear);
            printf("%d\n", value);
            break;
        default:
            break;
        }

        for (int i = 0; i < 5 ; i++)
        {
            printf("%d ", array[i]);
        }
        printf("\n");
    }

    return 0;
}

void add(int array[5], int *front, int *rear, int value)
{
    *rear %= 5;
    array[*rear] = value;
    *rear += 1;
}

int getFirst(int array[5], int *front, int *rear)
{
    int value = array[*front];
    *front += 1;
    *front %= 5;
    return value;
}