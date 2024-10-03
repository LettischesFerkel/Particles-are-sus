#include <stdio.h>
#include "libs/fixedPointMathematics.h"

int main(int argc, char** args[])
{
    char inputBuffer[65];
    inputBuffer[64] = '\0';
    printf("The Sus has arised\n\n");
    fgets(inputBuffer, 64, stdin);
    return 1;
}