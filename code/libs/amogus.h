#pragma once

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
//#include "../../sdl-vscode-c-cpp/common.h"
//#include "../../sdl-vscode-c-cpp/sdl_utils.h"
#include "komunals.h"
#include <stdlib.h>

int loadBMPImage(char* adress, SDL_Surface** surface)
{
    *surface = SDL_LoadBMP(adress);
    if(!(*surface))
    {
        printf("Failed to load image\n");
        printf("SDL2 Error: %d\n",  SDL_GetError());
        return -1;
    }
    return EXIT_SUCCESS;
}

int initialiseAmogus(SDL_Window** window, SDL_Renderer** renderer, SDL_Surface** window_surface, int width, int height, char* windowName)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
        return -1;
    }

    *window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    if(!(*window))
    {
        printf("Failed to create window\n");
        return -1;
    }

    *window_surface = SDL_GetWindowSurface(*window);

    if(!(*window_surface))
    {
        printf("Failed to get the surface from the window\n");
        return -1;
    }

    return EXIT_SUCCESS;
}

int drawPoints(SDL_Renderer** renderer, point2i* points, int count, int fixed_precision)
{
    point2i current;
    for (int i = 1; i < count; i++)
    {
        current = *(points + count);
        SDL_SetRenderDrawColor(*renderer, current.color.red, current.color.green, current.color.blue, current.color.alpha);
        if (fixed_precision >= 0) { SDL_RenderDrawPoint(*renderer, (int)(current.pos.x >> fixed_precision), (int)(current.pos.x >> fixed_precision)); }
        else { SDL_RenderDrawPoint(*renderer, (int)(current.pos.x << abs(fixed_precision)), (int)(current.pos.x << abs(fixed_precision))); }
    }
}
int clearScreen(SDL_Renderer** renderer, colour fillColour)
{
    SDL_SetRenderDrawColor(*renderer, fillColour.red, fillColour.green, fillColour.blue, fillColour.alpha);
    SDL_RenderClear(*renderer);
}

int drawLine(int height, int xPixel)
{
    
}