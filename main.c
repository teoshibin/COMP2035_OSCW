#include <stdio.h>

int main()
{

    int k = 0;
    while(1){

        int i[k];
        k++;
        for (int j = 0; j < k; j++)
        {
            i[j] = k;
            printf("%d", i[j]);
        }
        printf("\n");

        if (k==6)
        {
            break;
        }
        
    }
    return 0;
}
