#include <stdbool.h>
#define UNITY_BUILD 1
#include <stdio.h>              // IWYU pragma: keep
#ifdef _WIN64
 #include <SDL.h>
 #include <SDL_image.h>
#else
 #include <SDL2/SDL.h>
 #include <SDL2/SDL_image.h>
#endif
#include "C:\Users\karlis.svaza\Documents\Sigma mindset\Particles-are-SUS\Particles-are-sus\sdl-vscode-c-cpp\common.h"
#include "C:\Users\karlis.svaza\Documents\Sigma mindset\Particles-are-SUS\Particles-are-sus\sdl-vscode-c-cpp\sdl_utils.h"
#include <stdlib.h>
char inputBuffer[65];

int initWindow()
{
    
}

char* hoard(int megabytes)
{
    int bytes = megabytes * 1000000;
    char* susBuffer = malloc(bytes);
    for (int n = 0; n < bytes; n++)
    {
        if(!(n%1000000)) { printf("Nu neticēsi: %3.8f% ; %4.1fMB\n", ((float)n)/bytes*100, ((float)n)/megabytes); }
        *(susBuffer+n) = (n%256);
    }
    printf("\nHoarded %f megabytes", megabytes);
    return susBuffer;
}

int main(int argc, char *argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1, 200, 0);

    if(!window)
    {
        printf("Failed to create window\n");
        return -1;
    }

    SDL_Surface *window_surface = SDL_GetWindowSurface(window);

    if(!window_surface)
    {
        printf("Failed to get the surface from the window\n");
        return -1;
    }

    SDL_UpdateWindowSurface(window);

    SDL_Delay(5000);
    inputBuffer[64] = '\0';
    printf("The Sus has arised.");
    char* susBuffer[10];
    for (int n = 0; n < 160; n++)
    {
        susBuffer[n] = hoard(111);
    }
    printf("\nProccess done");
    fgets(&inputBuffer[0], 64, stdin);

    return 1;
    return 0;
}