#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

int main()
{
    double a = 6;
    double b = 3;

    void *handle;
    double (*Add)(double, double), (*Subtract)(double, double), 
    (*Multiply)(double, double), (*Divide)(double, double);
    char *error;

    handle = dlopen("./exLib/libCal.so",RTLD_LAZY);
    if(!handle)
    {
        fputs(dlerror(), stderr);
        exit(1);
    }

    Add = dlsym(handle, "Add");
    if((error=dlerror())!= NULL)
    {
        fprintf(stderr, "%s", error);
        exit(1);
    }

    Subtract = dlsym(handle, "Subtract");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s", error);
        exit(1);
    }

    Multiply = dlsym(handle, "Multiply");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s", error);
        exit(1);
    }

    Divide = dlsym(handle, "Divide");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s", error);
        exit(1);
    }

    printf("6+3: %f \n",Add(a,b));
    printf("6-3: %f \n",Subtract(a,b));
    printf("6*3: %f \n",Multiply(a,b));
    printf("6/3: %f \n",Divide(a,b));

    dlclose(handle);
    return 0;
}