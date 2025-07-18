#include <stdio.h>
#include "../headers/getConsoleWidth.h"

int main()
{
    int width = getConsoleWidth();
    printf("Terminal width: %d\n", width);
    return 0;
}
