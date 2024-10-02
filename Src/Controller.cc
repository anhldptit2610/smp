#include "Controller.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <unistd.h>

void UpdateCurrentAndNextTrack(int n)
{
    MediaFile& mediaFile = GetMediaFile();

    mediaFile.SetCurrentTrack(n);
    mediaFile.SetNextTrack(n + 1);
}

void MusicFinished(void)
{
    MediaFile& mediaFile = GetMediaFile();

    mediaFile.SetNextTrack(mediaFile.GetNextTrack() + 1);
    PlaySong(mediaFile.GetTrackPath(mediaFile.GetNextTrack()));
    UpdateHighlight(mediaFile.GetNextTrack());
}

void Controller::ParseArgument(int argc, char *argv[])
{
    int opt;
    std::string str;

    while ((opt = getopt(argc, argv, "d:")) != -1) {
        switch (opt) {
        case 'd':
            str = optarg;
            workPath = str;
            mode = OPTION_PLAY_MUSIC_DIRECTORY;
            BrowsePath(workPath);
            break;
        default:
            break;
        }
    }
}

void Controller::UpdateScreen(MediaFile& mediaFile)
{
    switch (mode) {
    case OPTION_PLAY_MUSIC_DIRECTORY:
    case OPTION_PLAY_MUSIC_NORMAL:
        ui.DirectoryLayout(mediaFile.GetMediaList());
        break;
    default:
        break;
    }
}

void Controller::InputHandler(MediaFile& mediaFile, int key, bool* quit)
{
    if (key == -1)
        return;
    if (key == KEYC_QUIT) {
        // stop all running tasks
        if (Mix_PlayingMusic())
            Mix_CloseAudio();
        *quit = true;
    } else if (key == KEYC_PAUSE) {
        player.PauseTheSong();
    } else if (key == KEYC_RESUME) {
        player.ResumeTheSong();
    } else if (key == KEYC_NEXT) {
        UpdateCurrentAndNextTrack(mediaFile.GetCurrentTrack() + 1);
        player.PlayTheSong(mediaFile.GetTrackPath(mediaFile.GetCurrentTrack()));
    } else if (key == KEYC_PREV) {
        UpdateCurrentAndNextTrack(mediaFile.GetCurrentTrack() - 1);
        player.PlayTheSong(mediaFile.GetTrackPath(mediaFile.GetCurrentTrack()));
    } else {
        if (mode == OPTION_PLAY_MUSIC_DIRECTORY || mode == OPTION_PLAY_MUSIC_NORMAL) {
            UpdateCurrentAndNextTrack(key);
            Mix_HookMusicFinished(MusicFinished);
        }
        player.PlayTheSong(mediaFile.GetTrackPath(key));
    }
}

void Controller::Run(void)
{
    bool quit = false;
    int key;
    MediaFile& mediaFile = GetMediaFile();

    // player.PlayTheSong(fileBrowser.GetFilePath(0));
    while (!quit) {
        UpdateScreen(mediaFile);
        key = ui.GetInput();
        InputHandler(mediaFile, key, &quit);
    }
}


/* constructor/destructor */

Controller::Controller(int argc, char *argv[]) : ui()
{
    exePath = std::filesystem::current_path();
    if (argc >= 2) {
        ParseArgument(argc, argv);
    } else {        // we go normal mode when no argument is present
        mode = OPTION_PLAY_MUSIC_NORMAL;
        workPath = exePath;
        BrowsePath(workPath);
    }
}

Controller::~Controller()
{

}

/* SDL init/destroy functions */

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