#define UNITY_BUILD 1
#include <stdio.h>              // IWYU pragma: keep
#ifdef _WIN64
 #include <SDL.h>
#else
 #include <SDL2/SDL.h>
#endif
#include "common.h"
#include "sdl_utils.h"

char inputBuffer[65];

int initWindow()
{
    
}

int main()
{
    inputBuffer[64] = '\0';
    printf("The Sus has arised.");
    fgets(&inputBuffer[0], 64, stdin);
    return 1;
    SDL_Window *window;
    SDL_Renderer *renderer;
    // Init SDL without texture filtering for better pixelart results
    if (sdl_utils_Init("SDL Tutorial", &window, &renderer, 0)) 
    {
        SDL_Texture* texture = IMG_LoadTexture(renderer, "res/characters.png");
        
        // Sprite source rectangle
        SDL_Rect srcRect = {9, 42, 15, 21};
        // Target rectangle (note that we will paint it at x4 its original size)
        SDL_Rect destRect = {0, 0, srcRect.w * 4, srcRect.h * 4};
        while (1)
        {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
            SDL_RenderPresent(renderer);

            SDL_Event event;
            if (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT) break;
            } 
        }

        SDL_DestroyTexture(texture);		
    }
    sdl_utils_Quit(window, renderer);
    return 0;
}