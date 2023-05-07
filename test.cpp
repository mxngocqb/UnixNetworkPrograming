#include <stdio.h>
#include <malloc.h>
#include <string.h>
int main()

{
    float* data = NULL;
    float tmp;
    int count = 0;
   
    for (int i = 0;i < count;i++)
    {
        printf("%f\n", data[i]);
    }
    free(data);
    data = NULL;
}