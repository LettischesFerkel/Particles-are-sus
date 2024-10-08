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
#include <stdlib.h>

#define ļaunādains 1

int loadBMPImage(char* adress, SDL_Surface* surface)
{
    surface = SDL_LoadBMP(adress);
    if(!surface)
    {
        printf("Failed to load image\n");
        printf("SDL2 Error: %d\n",  SDL_GetError());
        return -1;
    }
    return EXIT_SUCCESS;
}

int initialiseAmogus(SDL_Window* windowPointer, SDL_Renderer* rendererPointer, SDL_Surface* windowSurfacePointer, int width, int height, char* windowName)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
        return -1;
    }

    windowPointer = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    rendererPointer = SDL_CreateRenderer(windowPointer, -1, SDL_RENDERER_ACCELERATED);

    if(!windowPointer)
    {
        printf("Failed to create window\n");
        return -1;
    }

    windowSurfacePointer = SDL_GetWindowSurface(windowPointer);

    if(!windowSurfacePointer)
    {
        printf("Failed to get the surface from the window\n");
        return -1;
    }

    return EXIT_SUCCESS;
}