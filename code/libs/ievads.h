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

int mouseButtonHandler(int* mouseDownP, int pressed, Uint8 button) // pressed : up/0 down/1
{
    if (SDL_BUTTON_LEFT == button) { *(mouseDownP + 0) = pressed; }
    else if (SDL_BUTTON_RIGHT == button) { *(mouseDownP + 1) = pressed; }
    else if (SDL_BUTTON_MIDDLE == button) { *(mouseDownP + 2) = pressed; }
    return EXIT_SUCCESS;
}

/*int keyboardHandler(Sint32* keys, int pressed, SDL_Keycode code)
{
    if ((code < 256) && (code >= 0)) { *(keys + (Sint32)code) = pressed; }
    return EXIT_SUCCESS;
}*/

int keyboardHandler(Sint32* keys, int pressed, SDL_Keycode code)
{
    if ((code < 128) && (code >= 0)) { printf("%d : '%c' '0x%04x'\n", pressed, (char)code, code); }
    else { printf("%d : neticēsi '0x%04x'\n", pressed, code); }
    return EXIT_SUCCESS;
}