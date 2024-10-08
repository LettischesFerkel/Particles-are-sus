#include<math.h>
#include<windows.h>
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
#include "..\sdl-vscode-c-cpp\common.h"
#include "..\sdl-vscode-c-cpp\sdl_utils.h"
#include <stdlib.h>
#include "libs/phisika.h"
#include "libs/amogus.h"
#define ļaunādains 1

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* window_surface;
int width = 600;
int height = 400;
char windowName[32] = "Amogus";
int keep_window_open = 1;

int start()
{

}

int update(int deltaTime)
{

}

int processEvent(int eventType)
{
    switch(eventType)
    {
        case SDL_QUIT:
            keep_window_open = 0;
            break;
        default:
            break;
    }
    return 0;
}

int main(int argc, char* args[])
{
    char inputBuffer[65];
    inputBuffer[64] = '\0';
    printf("The Sus has arised\n\n");

    // loga inicializācija
    initialiseAmogus(window, renderer, window_surface, width, height, windowName);

    start();

    while (keep_window_open)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e) > 0)
        {
            processEvent(e.type);
        }
        update(1);
    }

    //fgets(inputBuffer, 64, stdin);

    SDL_FreeSurface(window_surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 1;
}