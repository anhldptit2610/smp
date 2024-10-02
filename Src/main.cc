#include "Controller.h"

int main(int argc, char *argv[])
{
    // if (argc < 2) {
    //     std::cerr << "Need at least one argument to work..." << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    if (InitSDL() != 0)
        exit(EXIT_FAILURE);

    Controller sol(argc, argv);
    if (sol.IsOK())
        sol.Run();

    DestroySDL();

    return 0;
}

// #include <SDL2/SDL.h>
// #include <SDL2/SDL_mixer.h>
// #include <stdio.h>

// int main(int argc, char* argv[]) {
//     // Initialize SDL2 and check for success
//     if (SDL_Init(SDL_INIT_AUDIO) < 0) {
//         printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
//         return -1;
//     }

//     // Initialize SDL_mixer and check for success
//     if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
//         printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
//         SDL_Quit();
//         return -1;
//     }

//     // Load the music file (replace "your_song.mp3" with your actual file)
//     Mix_Music *music = Mix_LoadMUS("/home/anhld/Projects/test/keshi_requiem/Amen.mp3");
//     if (music == NULL) {
//         printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
//         Mix_CloseAudio();
//         SDL_Quit();
//         return -1;
//     }

//     // Play the music (-1 means play indefinitely, 0 means play once)
//     if (Mix_PlayMusic(music, 0) == -1) {
//         printf("Failed to play music! SDL_mixer Error: %s\n", Mix_GetError());
//         Mix_FreeMusic(music);
//         Mix_CloseAudio();
//         SDL_Quit();
//         return -1;
//     }

//     printf("Playing music... Press Enter to stop.\n");

//     // Keep playing the music until the user presses Enter
//     getchar();

//     // Clean up resources
//     Mix_FreeMusic(music);
//     Mix_CloseAudio();
//     SDL_Quit();

//     return 0;
// }