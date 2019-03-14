#include <stdio.h>
#include <omp.h>

struct Compare { int val; int index; };
#pragma omp declare reduction(maximum : struct Compare : omp_out = omp_in.val > omp_out.val ? omp_in : omp_out)

void selectionsort(int* arr,int* indexes, int size)
{
    for (int i = size - 1; i > 0; --i)
    {
        struct Compare max;
        max.val = arr[i];
        max.index = i;
        #pragma omp parallel for reduction(maximum:max)
        for (int j = i - 1; j >= 0; --j)
        {
            if (arr[j] > max.val)
            {
                max.val = arr[j];
                max.index = j;
            }
        }
        int tmp = arr[i];
        int tmp2 = indexes[i];
        arr[i] = max.val;
        indexes[i] = max.val;

        arr[max.index] = tmp;
        indexes[max.index] = tmp2;
    }
}

int main()
{
        int x[10] = {8,7,9,1,2,5,4,3,0,6};
        int y[10] = {1,2,3,4,5,6,7,8,9,10};
        selectionsort(x,y, 10);

        for (int i = 0; i < 10; i++)
                printf("%d\n", y[i]);
        return 0;
}