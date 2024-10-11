#include<math.h>
#include<time.h>
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

#define fixed_precision 12

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Surface* window_surface;
int windowWidth = 1200;
int windowHeight = 800;
char windowName[32] = "Amogus";
int keep_window_open = runivan;

SDL_Texture* heisenburger;
SDL_Rect heisenburgerRect;

#define skaits 4096
point2i punkti[skaits];
vektor2i punktSakumPos[skaits];

vektor2i mousePos = nullVector2i;
int mouseButtonLeftDown = 0;
int mouseButtonRightDown = 0;
int mouseButtonMiddleDown = 0;

vektor2i spherePos = nullVector2i;
int sphereRadius = 100;

int start()
{
    // load heisenburger
    loadTexture(&renderer, "build/res/heisenburger.Jpg", &heisenburger);
    heisenburgerRect.x = 0; //the x coordinate
    heisenburgerRect.y = 0; //the y coordinate
    heisenburgerRect.w = 50; //the width of the texture
    heisenburgerRect.h = 50; //the height of the texture
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
        position = randomDirectionVektor2I(pow(2, fixed_precision - 0.125), (int)(360.0 * pow(2, fixed_precision - 7)));
        punktSakumPos[i] = position;
        position = transformUnitToPixelCoordinates(windowWidth, windowHeight, position, fixed_precision);
        colour color = (colour){ 255, 255, 255, 255 }; // melns
        punkti[i].pos = position;
        //printf("\nVektor = { %d, %d } ; length = %d\n", punkti[i].pos.x, punkti[i].pos.y, magnitudeOfVektor2I(punkti[i].pos));
        punkti[i].color = color;
    }
    clearScreen(&renderer, (colour){ 0, 0, 0, 255 });
    SDL_RenderPresent(renderer);

    spherePos = (vektor2i){ windowWidth >> 1, windowHeight >> 1};
    sphereRadius = 100;
}

int update(int deltaTime)
{
    printf(" %3.3f FPS | ", (float)(1000.0 / (float)(deltaTime * pow(2, -fixed_precision))));
    clearScreen(&renderer, (colour){ 100, 100, 100, 255 });

    // //drawPoints(&renderer, &punkti[0], skaits, 0);
    // for (int i = 1; i < skaits; i++)
    // {
    //     point2i current = punkti[i];
    //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //     SDL_RenderDrawPoint(renderer, current.pos.x, current.pos.y);
    // }

    if (mouseButtonLeftDown || !mouseButtonLeftDown) { spherePos = mousePos; printf("\nNew Sphere Coordinates Set\n"); }
    int sphereRadiusSquared = pow(sphereRadius, 2);
    vektor2i pixPos = { 0, 0 };
    int precision = -4;

    for (pixPos.y = windowHeight - 1; pixPos.y > -1; pixPos.y--) // draw sphere
    {
        for (pixPos.x = 0; pixPos.x < windowWidth; pixPos.x++)
        {
            // calcDst
            int distanceSquared = magnitudeSquaredOfVektor2I(subtractVektors2I(spherePos, pixPos));
            if (distanceSquared >= sphereRadiusSquared) // pixel not in sphere
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawPoint(renderer, pixPos.x, pixPos.y);
            }
            else // pixel is in sphere
            {
                int colorIntensity = (int)(510 * asin(sqrt(distanceSquared) / sphereRadius) / PI);
                SDL_SetRenderDrawColor(renderer, 255 - colorIntensity, 255 - colorIntensity, 255 - colorIntensity, 255);
                SDL_RenderDrawPoint(renderer, pixPos.x, pixPos.y);
            }
        }
    }

    // calcDst to mouse location
    int distanceSquared = magnitudeSquaredOfVektor2I(subtractVektors2I(spherePos, mousePos));
    if (distanceSquared >= sphereRadiusSquared) // pixel not in sphere
    {
        printf("Pointer not in sphere!\n");
    }
    else // pixel is in sphere
    {
        int colorIntensity = (int)(510 * asin(sqrt(distanceSquared) / sphereRadius) / PI);
        printf("Intensity for given pixel : %3d of distance %d\n", colorIntensity, (int)sqrt(distanceSquared));
    }

    // heisenburger
    SDL_RenderCopy(renderer, heisenburger, NULL, &heisenburgerRect);

    SDL_RenderPresent(renderer);
}

int resizeHandler(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    printf("Window resized to %d * %d\n", windowWidth, windowHeight);
    SDL_SetWindowSize(window, windowWidth, windowHeight);
    window_surface = SDL_GetWindowSurface(window);
    uint32_t black = SDL_MapRGBA(window_surface->format, 0, 0, 0, 255);
    SDL_FillRect(window_surface, NULL, black);
    SDL_UpdateWindowSurface(window);

    for (int i = 0; i < skaits; i++) // punktu pikseļkoordinātu pārrēķināšana
    {
        vektor2i position = punktSakumPos[i];
        position = transformUnitToPixelCoordinates(windowWidth, windowHeight, position, fixed_precision);
        punkti[i].pos = position;
    }
}
int mouseMovementHandler() {  } // empty

int processEvent(SDL_Event event)
{
    int resized = 0;
    int mouseMoved = 0;
    switch(event.type)
    {
        case SDL_QUIT:
            keep_window_open = ļaunādains;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                resizeHandler(event.window.data1, event.window.data2);
            }
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&(mousePos.x), &(mousePos.y));
            mouseMovementHandler();
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (SDL_BUTTON_LEFT == event.button.button)
            {
                mouseButtonLeftDown = 1;
                //printf("LMB Down\n");
            }
            if (SDL_BUTTON_RIGHT == event.button.button)
            {
                mouseButtonRightDown = 1;
                //printf("RMB Down\n");
            }
            if (SDL_BUTTON_MIDDLE == event.button.button)
            {
                mouseButtonMiddleDown = 1;
                //printf("MMB Down\n");
            }
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
    clock_t start, end;
    int cpu_time_used = 0;
    int precisionPositive = (fixed_precision >= 0);
    if (precisionPositive) { cpu_time_used = 1000 << fixed_precision; }
    else { cpu_time_used = 1000 >> abs(fixed_precision); }

    while (keep_window_open)
    {
        start = clock();
        SDL_Event e;
        while(SDL_PollEvent(&e) > 0)
        {
            processEvent(e);
        }
        //SDL_FillRect(window_surface, NULL, SDL_MapRGB(window_surface->format, 0, 0, 0));
        update(cpu_time_used); // deltaTime in miliocesonds
        mouseButtonLeftDown = 0;
        mouseButtonRightDown = 0;
        mouseButtonMiddleDown = 0;
        //SDL_UpdateWindowSurface(window);
        end = clock();
        if (precisionPositive) { cpu_time_used = ((int)(end - start)) << fixed_precision; }
        else { cpu_time_used = ((int)(end - start)) >> abs(fixed_precision); }
    }

    //fgets(inputBuffer, 64, stdin);

    SDL_DestroyTexture(heisenburger);

    SDL_FreeSurface(window_surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 1;
}