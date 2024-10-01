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

char inputBuffer[65];

SDL_Window* window;
SDL_Surface* window_surface;
SDL_Rect windowDimensions;

int keep_window_open = 1;

SDL_Surface* image;
SDL_Surface* imageZils;
SDL_Surface* imageDzeltens;
SDL_Surface* imageBalts;
SDL_Surface* imageSarkans;

SDL_Rect imageDimensions;

typedef struct {
    double x;
    double y;
    double vx;
    double vy;
    double w;
    double h;
    double d;
}particle;
typedef struct {
    float x;
    float y;
}vector2f;
const vector2f nullVector2f = {0, 0};
#define partikles 500000
const int collisionsEnabled = 0;

const double gravitate = -50;
const float temperatura = 25; // pikseļos/sekundē obv

particle maksimilianaKungs[partikles]; // partikāļi
particle imageDefParticle = { 0, 0, 0, 0, 1, 1, 0 };
vector2f imageSize = {200, 200};

float vektorMagnitude(vector2f vektor)
{
    return sqrt(pow(vektor.x, 2) + pow(vektor.y, 2));
}
vector2f normaliseVektor(vector2f unnormalVecotr, float desiredMagnitude)
{
    float magnitude = vektorMagnitude(unnormalVecotr);
    if (magnitude == 0) { return nullVector2f; }
    float relativeMagnitude = desiredMagnitude / magnitude;
    vector2f result;
    result.x = unnormalVecotr.x * relativeMagnitude;
    result.y = unnormalVecotr.y * relativeMagnitude;
    return result;
}
SDL_Surface* loadBMPImage(char* adress)
{
    SDL_Surface* img = SDL_LoadBMP(adress);
    if(!img)
    {
        printf("Failed to load image\n");
        printf("SDL2 Error: %d\n",  SDL_GetError());
        return NULL;
    }
    return img;
}

int start()
{
    image = loadBMPImage("res/zalsApalsStarpMums.bmp");
    imageZils = loadBMPImage("res/zilsApalsStarpMums.bmp");
    imageDzeltens = loadBMPImage("res/dzeltensApalsStarpMums.bmp");
    imageBalts = loadBMPImage("res/baltsApalsStarpMums.bmp");
    imageSarkans = loadBMPImage("res/sarkansApalsStarpMums.bmp");
    imageDefParticle.x = (windowDimensions.w - imageDefParticle.w) / 2;
    imageDefParticle.y = (windowDimensions.h - imageDefParticle.h) / 2; // neticēsi

    for (int i = 0; i < partikles; i++)
    {
        maksimilianaKungs[i].x = imageDefParticle.x;
        maksimilianaKungs[i].y = imageDefParticle.y;
        maksimilianaKungs[i].w = imageDefParticle.w;
        maksimilianaKungs[i].h = imageDefParticle.h;
        maksimilianaKungs[i].d = imageDefParticle.d;
    }

    vector2f normalisationVector;
    vector2f randomisationVector;
    for (int i = 1; i < partikles; i++) { 
        maksimilianaKungs[i].x = (rand() % (int)(windowDimensions.w - maksimilianaKungs[i].w + 1)) + maksimilianaKungs[i].w / 2;
        maksimilianaKungs[i].y = (rand() % (int)(windowDimensions.h - maksimilianaKungs[i].h + 1)) + maksimilianaKungs[i].h / 2;
        randomisationVector.x = (rand() % (100 + 1)) - 50;
        randomisationVector.y = (rand() % (100 + 1)) - 50;
        normalisationVector = normaliseVektor(randomisationVector, temperatura);
        maksimilianaKungs[i].vx = normalisationVector.x;
        maksimilianaKungs[i].vy = normalisationVector.y;
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
        if (maksimilianaKungs[i].y > windowDimensions.h - maksimilianaKungs[i].h)
        {
            maksimilianaKungs[i].y = windowDimensions.h - maksimilianaKungs[i].h;
            maksimilianaKungs[i].vy = -maksimilianaKungs[i].vy;
        }
        else if (maksimilianaKungs[i].y < 0)
        {
            maksimilianaKungs[i].y = 0;
            maksimilianaKungs[i].vy = -maksimilianaKungs[i].vy;
        }
        
        if (maksimilianaKungs[i].x > windowDimensions.w - maksimilianaKungs[i].w)
        {
            maksimilianaKungs[i].x = windowDimensions.w - maksimilianaKungs[i].w;
            maksimilianaKungs[i].vx = -maksimilianaKungs[i].vx;
        }
        else if (maksimilianaKungs[i].x < 0)
        {
            maksimilianaKungs[i].x = 0;
            maksimilianaKungs[i].vx = -maksimilianaKungs[i].vx;
        }

        // neticēsi

        if (collisionsEnabled)
        {
            int calcd[partikles];
            for (int n = 0; n < partikles; n++) { calcd[n] = 0; }
            for (int n = 0; n < partikles; n++)
            {
                if ((n != i) && !(calcd[i] && calcd[n]))
                {
                    float dist = sqrt(pow(maksimilianaKungs[n].x, 2) + pow(maksimilianaKungs[n].x, 2)) - sqrt(pow(maksimilianaKungs[i].x, 2) + pow(maksimilianaKungs[i].x, 2));
                    if ((maksimilianaKungs[i].d/2 + maksimilianaKungs[n].d/2) <= dist)
                    {
                        float posDX = maksimilianaKungs[i].x - maksimilianaKungs[n].x;
                        float posDY = maksimilianaKungs[i].y - maksimilianaKungs[n].y;
                        float dX = maksimilianaKungs[i].vx - maksimilianaKungs[n].vx;
                        float dY = maksimilianaKungs[i].vy - maksimilianaKungs[n].vy;
                        float pX = -posDY; // X of Vector perpendicular of collision
                        float pY = posDX; // Y of Vector perpendicular of collision

                        float o1 = (dX * pX) + (dY * pY);
                        float gX = (o1 * pX) / (pow(pX, 2) * pow(pY, 2));
                        float gY = (o1 * pY) / (pow(pX, 2) * pow(pY, 2));
                        maksimilianaKungs[i].vx = maksimilianaKungs[i].vx + gX;
                        maksimilianaKungs[i].vy = maksimilianaKungs[i].vy + gY;
                        calcd[i] = 1;

                        float o2 = (dX * -pY) + (dY * pX);
                        float bX = (o2 * pX) / (pow(pX, 2) * pow(pY, 2));
                        float bY = (o2 * pY) / (pow(pX, 2) * pow(pY, 2));
                        maksimilianaKungs[n].vx = maksimilianaKungs[n].vx + gX;
                        maksimilianaKungs[n].vy = maksimilianaKungs[n].vy + gY;
                        calcd[n] = 1;
                    }
                }
            }
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
SDL_Rect vektorTuRekt(vector2f target)
{
    SDL_Rect result;
    result.x = 0;
    result.y = 0;
    result.w = target.x;
    result.h = target.y;
    return result;
}

int update(double deltaTime)
{
    //if (deltaTime != (double)0) { printf("Amogus = ( %6.1f, %6.1f )\n", maksimilianaKungs[0].x, maksimilianaKungs[0].y); }
    physicsUpdate(deltaTime);

    SDL_Rect imgPrtcle;
    SDL_Rect wndPrtclis;
    for (int i = 0; i < partikles; i++)
    {
        imgPrtcle = partikleTuRekt(maksimilianaKungs[i]);
        wndPrtclis = vektorTuRekt(imageSize);
        SDL_BlitScaled(imageDzeltens, &wndPrtclis, window_surface, &imgPrtcle);
    }
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
    windowDimensions.w = 1920;
    windowDimensions.h = 1080;
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize the SDL2 library\n");
        return -1;
    }

    window = SDL_CreateWindow("Melnums", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowDimensions.w, windowDimensions.h, 0);

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

    //fgets(&inputBuffer[0], 64, stdin);
    printf("The Sus has arised.\n\n");
    vector2f test = {40, 40};
    printf("vektor = (%f, %f) ; |vektor| = %f\n", test.x, test.y, vektorMagnitude(test));
    test = normaliseVektor(test, 100);
    printf("norm(vektor) = (%f, %f) ; |norm(vektor)| = %f\n\n", test.x, test.y, vektorMagnitude(test));
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