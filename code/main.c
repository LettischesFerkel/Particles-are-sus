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

#define fixed_precision 8

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* window_surface;
int windowWidth = 1200;
int windowHeight = 800;
char windowName[32] = "Amogus";
int keep_window_open = runivan;

SDL_Surface* heisenburger;

#define skaits 1000
point2i punkti[skaits];

int start()
{
    // load heisenburger
    loadBMPImage("C:/Users/ksvaz/Documents/Skola/Programmesana/C/Particles-are-sus/code/build/res/heisenburger.bmp", &heisenburger);
    // initialise the rest

    // vektor2i vektori[8] = {
    //     { -128, 0 },
    //     { -96, 0 },
    //     { -64, 0 },
    //     { -32, 0 },
    //     { 32, 0 },
    //     { 64, 0 },
    //     { 96, 0 },
    //     { 128, 0 }
    // };
    // for (int i = 0; i < 8; i++)
    // {
    //     vektor2i position = vektori[i];
    //     printf("\nVektor = { %d, %d } ; length = %d\n", position.x, position.y, magnitudeOfVektor2I(position));
    //     position = transformUnitToPixelCoordinates(windowWidth, windowHeight, position, 7);
    //     printf("Transformed Vektor = { %d, %d } ; length = %d\n", position.x, position.y, magnitudeOfVektor2I(position));
    // }

    for (int i = 0; i < skaits; i++) // punktu inicializācija
    {
        vektor2i position = { 300, 200 };
        position = randomDirectionVektor2I(1024);
        position = transformUnitToPixelCoordinates(windowWidth, windowHeight, position, 10);
        colour color = (colour){ 0, 255, 0, 255 }; // melns
        punkti[i].pos = position;
        //printf("\nVektor = { %d, %d } ; length = %d\n", punkti[i].pos.x, punkti[i].pos.y, magnitudeOfVektor2I(punkti[i].pos));
        punkti[i].color = color;
    }
    clearScreen(&renderer, (colour){ 200, 200, 200, 255 });
    SDL_RenderPresent(renderer);
}

int update(int deltaTime)
{
    clearScreen(&renderer, (colour){ 200, 200, 200, 255 });

    //drawPoints(&renderer, &punkti[0], skaits, 0);
    for (int i = 1; i < skaits; i++)
    {
        point2i current = punkti[i];
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderDrawPoint(renderer, current.pos.x, current.pos.y);
    }
    

    //SDL_BlitSurface(heisenburger, NULL, window_surface, NULL);
    //SDL_UpdateWindowSurface(window);
    

    /*
        //Render red filled quad
        SDL_Rect fillRect = { windowWidth / 4, windowHeight / 4, windowWidth / 2, windowHeight / 2 };
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);        
        SDL_RenderFillRect(renderer, &fillRect);
    */
    SDL_RenderPresent(renderer);
}

int processEvent(int eventType)
{
    switch(eventType)
    {
        case SDL_QUIT:
            keep_window_open = ļaunādains;
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
    initialiseAmogus(&window, &renderer, &window_surface, windowWidth, windowHeight, windowName);

    start();

    while (keep_window_open)
    {
        SDL_Event e;
        while(SDL_PollEvent(&e) > 0)
        {
            processEvent(e.type);
        }
        //SDL_FillRect(window_surface, NULL, SDL_MapRGB(window_surface->format, 0, 0, 0));
        update(1);
        //SDL_Wait(500);
        //SDL_UpdateWindowSurface(window);
    }

    //fgets(inputBuffer, 64, stdin);

    SDL_FreeSurface(window_surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 1;
}