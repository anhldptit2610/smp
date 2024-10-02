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

    while ((opt = getopt(argc, argv, "d:e:")) != -1) {
        switch (opt) {
        case 'd':
            str = optarg;       // TODO: make a lambda function for set workPath
            if (!std::filesystem::is_directory(str)) {
                std::cout << "It's a file..." << std::endl;
                ok = false;
                return;
            }
            workPath = str;
            mode = OPTION_PLAY_MUSIC_DIRECTORY;
            BrowsePath(workPath);
            break;
        case 'e':
            str = optarg;
            if (!std::filesystem::is_directory(str)) {
                std::cerr << "It's a file..." << std::endl;
                ok = false;
                return;
            }
            workPath = str;
            mode = OPTION_EDIT_METADATA;

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
    switch (key) {
    case -1:
        return;
    case KEYC_QUIT:
        if (Mix_PlayingMusic())
            Mix_CloseAudio();
        *quit = true;
        break;
    case KEYC_PAUSE:
        if (mode == OPTION_PLAY_MUSIC_DIRECTORY || mode == OPTION_PLAY_MUSIC_NORMAL)
            player.PauseTheSong();
        break;
    case KEYC_RESUME:
        if (mode == OPTION_PLAY_MUSIC_DIRECTORY || mode == OPTION_PLAY_MUSIC_NORMAL)
            player.ResumeTheSong();
        break;
    case KEYC_NEXT:
        if (mode == OPTION_PLAY_MUSIC_DIRECTORY || mode == OPTION_PLAY_MUSIC_NORMAL) {
            UpdateCurrentAndNextTrack(mediaFile.GetCurrentTrack() + 1);
            player.PlayTheSong(mediaFile.GetTrackPath(mediaFile.GetCurrentTrack()));
        }
        break;
    case KEYC_PREV:
        if (mode == OPTION_PLAY_MUSIC_DIRECTORY || mode == OPTION_PLAY_MUSIC_NORMAL) {
            UpdateCurrentAndNextTrack(mediaFile.GetCurrentTrack() - 1);
            player.PlayTheSong(mediaFile.GetTrackPath(mediaFile.GetCurrentTrack()));
        }
        break;
    case KEYC_VOLUME_DOWN:
        player.TurnVolumeDown();
        break;
    case '=':
        player.TurnVolumeUp();
        break;
    default:
        if (mode == OPTION_PLAY_MUSIC_DIRECTORY || mode == OPTION_PLAY_MUSIC_NORMAL) {
            UpdateCurrentAndNextTrack(key);
            Mix_HookMusicFinished(MusicFinished);
        }
        player.PlayTheSong(mediaFile.GetTrackPath(key));
        break;
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


bool Controller::IsOK(void)
{
    return ok;
}

/* constructor/destructor */

Controller::Controller(int argc, char *argv[]) : ui()
{
    exePath = std::filesystem::current_path();
    ok = true;
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
