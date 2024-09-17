#include <stdio.h>

int running = 1;

#define windowWidth 208
#define windowHeight 54
char window[windowHeight][windowWidth];

char output[65];

int drawScreen()
{
    printf("\n--------------------------------\n%s\n--------------------------------\n", window);
}
int clearScreen(char filler)
{
    for(int y = 0; y < windowHeight; y++)
    {
        for(int x = 0; x < windowWidth-1; x++)
        {
            window[y][x] = filler;
        }
        window[y][windowWidth-1] = '\n';
        if(y == windowHeight-1) { window[windowHeight-1][windowWidth-1] = '\0'; }
    }
}
int init()
{
    clearScreen(64);
}

calculate()
{
    int result = 0;
    for(int y = 0; y < windowHeight; y++)
    {
        for(int x = 0; x < windowWidth-1; x++)
        {
            result = 0;
            result = (((x - y) + 0.5) >= 0) && (((x - y) - 0.5) <= 0);
            window[y][x] = result * 64;
        }
        window[y][windowWidth-1] = '\n';
        if(y == windowHeight-1) { window[windowHeight-1][windowWidth-1] = '\0'; }
    }
}

int update()
{
    clearScreen(64);
    calculate();
    drawScreen();
    fgets(&output[0], 64, stdin);
    running = 0;
}

int main()
{
    printf("Amogus is sus\n");

    init();
    while(running)
    {
        update();
    }

    return 1;
}