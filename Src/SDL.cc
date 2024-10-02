#include "SDL.h"

int InitSDL(void)
{
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        SDL_Log("SDL_Init error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        SDL_Log("Mix_OpenAudio Error: %s\n", Mix_GetError());
        SDL_Quit();
        return -1;
    }
    return 0;
}

void DestroySDL(void)
{
    Mix_CloseAudio();
    SDL_Quit();
}