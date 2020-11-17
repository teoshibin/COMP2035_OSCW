

int *array;
int length;
int count;

int count3s()
{
    int i;
    count = 0;
    for (i = 0; i < length; i++)
    {
        if (array[i] == 3)
        {
            count++;
        }
    }
}
