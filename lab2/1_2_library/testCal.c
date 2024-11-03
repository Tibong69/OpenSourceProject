#include <stdio.h>
#include "calculate.h"
int main()
{
    double a = 6;
    double b = 3;
    printf("6+3: %f \n",Add(a,b));
    printf("6-3: %f \n",Subtract(a,b));
    printf("6*3: %f \n",Multiply(a,b));
    printf("6/3: %f \n",Divide(a,b));
}
