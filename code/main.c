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
#include "C:\Users\karlis.svaza\Documents\Sigma mindset\Particles-are-SUS\Particles-are-sus\sdl-vscode-c-cpp\common.h"
#include "C:\Users\karlis.svaza\Documents\Sigma mindset\Particles-are-SUS\Particles-are-sus\sdl-vscode-c-cpp\sdl_utils.h"
#include <stdlib.h>

char inputBuffer[65];

SDL_Window* window;
SDL_Surface* window_surface;
SDL_Rect windowDimensions;

int keep_window_open = 1;

SDL_Surface* image;

SDL_Rect imageDimensions;

typedef struct {
    double x;
    double y;
    double vx;
    double vy;
    double w;
    double h;
}particle;

#define partikles 10
double gravitate = -1;
particle maksimilianaKungs[partikles]; // partikāļi
particle imageDefParticle = { 0, 0, 0, 0, 200, 200 };
int start()
{
    image = SDL_LoadBMP("res/image02.bmp");
    if(!image)
    {
        printf("Failed to load image\n");
        printf("SDL2 Error: %d\n",  SDL_GetError());
        return -1;
    }
    imageDefParticle.x = (windowDimensions.w - imageDefParticle.w) / 2;
    imageDefParticle.y = (windowDimensions.h - imageDefParticle.h) / 2; // neticēsi

    for (int i = 0; i < partikles; i++)
    {
        maksimilianaKungs[i].x = imageDefParticle.x;
        maksimilianaKungs[i].y = imageDefParticle.y;
        maksimilianaKungs[i].w = imageDefParticle.w;
        maksimilianaKungs[i].h = imageDefParticle.h;
    }
}

int physicsUpdate(double deltaTime)
{
    for (int i = 0; i < partikles; i++)
    {
        // apply acceleration;
        maksimilianaKungs[i].vy = maksimilianaKungs[i].vy - (gravitate * deltaTime); // gravitate

        maksimilianaKungs[i].x = maksimilianaKungs[i].x + (maksimilianaKungs[i].vx * deltaTime);
        maksimilianaKungs[i].y = maksimilianaKungs[i].y + (maksimilianaKungs[i].vy * deltaTime);

        // faktiski apspiest maksimiliana kunga apkārtmerus un parvietošanās iespējas ekrāna ietvaros
        if (maksimilianaKungs[i].y >= windowDimensions.h - maksimilianaKungs[i].h)
        {
            maksimilianaKungs[i].y = windowDimensions.h - maksimilianaKungs[i].h;
            maksimilianaKungs[i].vy = 0;
        }
        else if (maksimilianaKungs[i].y <= 0 + maksimilianaKungs[i].h)
        {
            maksimilianaKungs[i].y = 0 + maksimilianaKungs[i].h;
            maksimilianaKungs[i].vy = 0;
        }
        
        if (maksimilianaKungs[i].x >= windowDimensions.w - maksimilianaKungs[i].w)
        {
            maksimilianaKungs[i].x = windowDimensions.w - maksimilianaKungs[i].w;
            maksimilianaKungs[i].vx = 0;
        }
        else if (maksimilianaKungs[i].x <= 0 + maksimilianaKungs[i].w)
        {
            maksimilianaKungs[i].x = 0 + maksimilianaKungs[i].w;
            maksimilianaKungs[i].vx = 0;
        }
    }
}
SDL_Rect partikleTuRekt(particle target)
{
    SDL_Rect result;
    result.x = target.x;
    result.y = target.y;
    result.w = target.w;
    result.h = target.h;
    return result;
}

int update(double deltaTime)
{
    //if (deltaTime != (double)0) { printf("Amogus = ( %6.1f, %6.1f )\n", maksimilianaKungs[0].x, maksimilianaKungs[0].y); }
    physicsUpdate(deltaTime);
    SDL_Rect imgPrtcle = partikleTuRekt(maksimilianaKungs[0]);
    SDL_BlitSurface(image, NULL, window_surface, &imgPrtcle);
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
}

int initWindow()
{
    windowDimensions.x = 0;
    windowDimensions.y = 0;
    windowDimensions.w = 720;
    windowDimensions.h = 600;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
        return -1;
    }

    window = SDL_CreateWindow("Melnums", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 600, 0);

    if(!window)
    {
        printf("Failed to create window\n");
        return -1;
    }

    window_surface = SDL_GetWindowSurface(window);

    if(!window_surface)
    {
        printf("Failed to get the surface from the window\n");
        return -1;
    }
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
    LARGE_INTEGER frequency;
    LARGE_INTEGER t1, t2;
    double elapsedTime = 0;
    QueryPerformanceFrequency(&frequency);

    fgets(&inputBuffer[0], 64, stdin);
    printf("The Sus has arised.\n\n");
    initWindow();
    start();

    int i = 0;
    while (keep_window_open)
    {
        QueryPerformanceCounter(&t1);
        SDL_Event e;
        while(SDL_PollEvent(&e) > 0)
        {
            processEvent(e.type);
        }
        SDL_FillRect(window_surface, NULL, SDL_MapRGB(window_surface->format, 0, 0, 0));
        update(0.01667);
        //SDL_Wait(500);
        SDL_UpdateWindowSurface(window);
        QueryPerformanceCounter(&t2);
        elapsedTime = (t2.QuadPart - t1.QuadPart) / frequency.QuadPart;
        i++;
        //if (i == 100) { break; }
    }

    SDL_FreeSurface(window_surface);
    SDL_DestroyWindow(window);

    //SDL_Delay(5000);
    inputBuffer[64] = '\0';
    printf("\nProccess done");

    return 1;
    return 0;
}