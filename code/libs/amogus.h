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

int loadTexture(SDL_Renderer** renderer, char* adress, SDL_Texture** texture)
{
    *texture = IMG_LoadTexture(*renderer, adress);
    if(!(*texture))
    {
        printf("Failed to load texture '%s'\n", adress);
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

    *window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if(!(*window))
    {
        printf("Failed to create window\n");
        return -1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    if(!(*renderer))
    {
        printf("Failed to create renderer\n");
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
int resizeWindow(SDL_Surface** window_surfaceP, SDL_Window** windowP, int width, int height, colour fillColor)
{
    printf("Window resized to %d * %d\n", width, height);
    SDL_SetWindowSize(*windowP, width, height);
    *window_surfaceP = SDL_GetWindowSurface(*windowP);
    if (!(*window_surfaceP)) { printf("\nFailed to surface resized window!\n\n"); return EXIT_FAILURE; }
    uint32_t black = SDL_MapRGBA((*window_surfaceP)->format, fillColor.red, fillColor.green, fillColor.blue, fillColor.alpha);
    SDL_FillRect(*window_surfaceP, NULL, black);
    SDL_UpdateWindowSurface(*windowP);
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

void DrawCircle(SDL_Renderer** renderer, int32_t centreX, int32_t centreY, int32_t radius) // navmans
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(*renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(*renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(*renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(*renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(*renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(*renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(*renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(*renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

int drawHorizontalLine(int height, int xPixel)
{
    
}