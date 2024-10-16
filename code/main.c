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
#include "libs/ievads.h"

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
int mouseDown[3] = { 0, 0, 0 }; // LMB, RMB, MMB
Sint32 keys[256];

vektor2i spherePos = nullVector2i;
int sphereRadius = 100;

vektor2i lineCoordinates[4] =
/*{
    { 2,  1},
    { -3, -2 },
    { 7, 12 },
    { -9, - 18 }
};*/
{
    { -1,  -1 },
    {  6,   4 },
    { -8, -10 },
    { 13,  13 }
};
const int worldScale = 48;
worldMapVPPLM worldMap =
{
    4,
    0
};

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
    //     position = translateUnitToPixelCoordinates(windowWidth, windowHeight, position, 7);
    //     printf("Transformed Vektor = { %d, %d } ; length = %d\n", position.x, position.y, magnitudeOfVektor2I(position));
    // }

    for (int i = 0; i < skaits; i++) // punktu inicializācija
    {
        vektor2i position = { 300, 200 };
        position = randomDirectionVektor2I(pow(2, fixed_precision - 0.125), (int)(360.0 * pow(2, fixed_precision - 7)));
        punktSakumPos[i] = position;
        position = translateUnitToPixelCoordinates(windowWidth, windowHeight, position, fixed_precision);
        colour color = (colour){ 255, 255, 255, 255 }; // melns
        punkti[i].pos = position;
        //printf("\nVektor = { %d, %d } ; length = %d\n", punkti[i].pos.x, punkti[i].pos.y, magnitudeOfVektor2I(punkti[i].pos));
        punkti[i].color = color;
    }
    clearScreen(&renderer, (colour){ 0, 0, 0, 255 });
    SDL_RenderPresent(renderer);

    spherePos = (vektor2i){ windowWidth >> 1, windowHeight >> 1};
    sphereRadius = 100;
    
    for (int i = 0; i < worldMap.count; i++)
    {
        lineCoordinates[i].x = lineCoordinates[i].x * worldScale;
        lineCoordinates[i].y = lineCoordinates[i].y * worldScale;
    }

    worldMap.lineCoords = &lineCoordinates[0];
}

vektor2i playerPos = {0, 0};
int update(int deltaTime)
{
    //printf(" %4.0f FPS | ", (float)(1000.0 / (float)(deltaTime * pow(2, -fixed_precision))));

    clearScreen(&renderer, (colour){ 200, 200, 200, 255 });

    vektor2i max = { abs((worldMap.lineCoords + worldMap.count - 1)->x) + worldScale, abs((worldMap.lineCoords + worldMap.count - 1)->y) + worldScale };
    for (int i = 0; i < worldMap.count; i++) // draw world map lines
    {
        int xAxis = (worldMap.lineCoords + i)->x;
        for (int y = 1; y < max.y; y++)
        {
            int lesserVal;
            int greaterVal;
            if (abs((worldMap.lineCoords + 0)->y) > y)
            {
                lesserVal = 0;
                greaterVal = (worldMap.lineCoords + 0)->y;
            }
            else if (abs((worldMap.lineCoords + worldMap.count - 1)->y) < y)
            {
                lesserVal = (worldMap.lineCoords + worldMap.count - 1)->y;
                greaterVal = max.y;
            }
            else
            {
                for (int n = 1; n < worldMap.count - 1; n++)
                {
                    int yAxis = (worldMap.lineCoords + n)->y;
                    if (abs(yAxis) > y)
                    {
                        greaterVal = yAxis;
                        lesserVal = (worldMap.lineCoords + n - 1)->y;
                    }
                }
            }
            if (xAxis > 0) { SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); }
            else { SDL_SetRenderDrawColor(renderer, 0 , 200, 0, 255); }
            SDL_RenderDrawPoint(renderer, abs(xAxis) + (windowWidth >> 1) - (max.x >> 1), y + (windowHeight >> 1) - (max.y >> 1));
        }

        int yAxis = (worldMap.lineCoords + i)->y;
        for (int x = 1; x < max.x; x++)
        {
            int lesserVal;
            int greaterVal;
            if (abs((worldMap.lineCoords + 0)->x) > x)
            {
                lesserVal = 0;
                greaterVal = (worldMap.lineCoords + 0)->x;
            }
            else if (abs((worldMap.lineCoords + worldMap.count - 1)->x) < x)
            {
                lesserVal = (worldMap.lineCoords + worldMap.count - 1)->x;
                greaterVal = max.x;
            }
            else
            {
                for (int n = 1; n < worldMap.count - 1; n++)
                {
                    int xAxis = (worldMap.lineCoords + n)->x;
                    if (abs(xAxis) > x)
                    {
                        greaterVal = xAxis;
                        lesserVal = (worldMap.lineCoords + n - 1)->x;
                    }
                }
            }
            if (yAxis > 0) { SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255); }
            else { SDL_SetRenderDrawColor(renderer, 0 , 200, 0, 255); }
            SDL_RenderDrawPoint(renderer, x + (windowWidth >> 1) - (max.x >> 1), abs(yAxis) + (windowHeight >> 1) - (max.y >> 1));
        }
    }
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
    vektor2i playerPixelPos = translateUnitToPixelCoordinates(windowWidth, windowHeight, playerPos, fixed_precision * worldScale);
    DrawCircle(&renderer, playerPixelPos.x, playerPixelPos.y, 6);

    // //drawPoints(&renderer, &punkti[0], skaits, 0);
    // for (int i = 1; i < skaits; i++)
    // {
    //     point2i current = punkti[i];
    //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    //     SDL_RenderDrawPoint(renderer, current.pos.x, current.pos.y);
    // }

    //if (mouseButtonLeftDown || !mouseButtonLeftDown) { spherePos = mousePos; printf("\nNew Sphere Coordinates Set\n"); }
    //int sphereRadiusSquared = pow(sphereRadius, 2);
    //vektor2i pixPos = { 0, 0 };
    //int precision = -4;

    /*for (pixPos.y = windowHeight - 1; pixPos.y > -1; pixPos.y--) // draw sphere
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
    } */// sphēra
    


    // heisenburger

    heisenburgerRect.w = 50;
    heisenburgerRect.h = 50;
    heisenburgerRect.x = windowWidth - heisenburgerRect.w;
    heisenburgerRect.y = windowHeight - heisenburgerRect.h;

    SDL_RenderCopy(renderer, heisenburger, NULL, &heisenburgerRect);
    SDL_RenderPresent(renderer);
}

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
                windowWidth = event.window.data1;
                windowHeight = event.window.data2;
                resizeWindow(&window_surface, &window, windowWidth, windowHeight, (colour){ 100, 100, 100, 255 });

                /*for (int i = 0; i < skaits; i++) // punktu pikseļkoordinātu pārrēķināšana
                {
                    vektor2i position = punktSakumPos[i];
                    position = translateUnitToPixelCoordinates(windowWidth, windowHeight, position, fixed_precision);
                    punkti[i].pos = position;
                }*/
            }
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&(mousePos.x), &(mousePos.y));
            break;
        case SDL_MOUSEBUTTONDOWN:
            mouseButtonHandler(&mouseDown[0], 1, event.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            mouseButtonHandler(&mouseDown[0], 0, event.button.button);
            break;
        case SDL_KEYDOWN:
            keyboardHandler(&keys[0], 1, event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            keyboardHandler(&keys[0], 0, event.key.keysym.sym);
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
