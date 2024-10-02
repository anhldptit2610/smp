#include "Controller.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <unistd.h>

void UpdateCurrentAndNextTrack(int n)
{
    MediaFileManage& fileManage = GetFileManage();

    fileManage.SetCurrentTrack(n);
    fileManage.SetNextTrack(n + 1);
}

void MusicFinished(void)
{
    MediaFileManage& fileManage = GetFileManage();

    fileManage.SetNextTrack(fileManage.GetNextTrack() + 1);
    PlaySong(fileManage.GetTrackPath(fileManage.GetNextTrack()));
    UpdateHighlight(fileManage.GetNextTrack());
}

void Controller::ParseArgument(int argc, char *argv[])
{
    int opt;
    std::string str;

    auto SetWorkPath = [this](std::string _path, int _mode) -> void {
            workPath = _path;
            mode = _mode;
            BrowsePath(workPath);
    };

    while ((opt = getopt(argc, argv, "d:e:")) != -1) {
        switch (opt) {
        case 'd':
            str = optarg;
            if (!std::filesystem::is_directory(str)) {
                std::cout << "It's a file..." << std::endl;
                ok = false;
                return;
            }
            SetWorkPath(str, OPTION_PLAY_MUSIC_DIRECTORY);
            break;
        case 'e':
            if (optarg == NULL) {
                ok = false;
                return;
            }
            str = optarg;
            SetWorkPath(str, OPTION_EDIT_METADATA);
            break;
        default:
            break;
        }
    }
}

void Controller::UpdateScreen(MediaFileManage& fileManage)
{
    switch (mode) {
    case OPTION_PLAY_MUSIC_DIRECTORY:
    case OPTION_PLAY_MUSIC_NORMAL:
        ui.DirectoryLayout(fileManage.GetMediaList());
        break;
    case OPTION_EDIT_METADATA:
        ui.EditMetadata(fileManage.GetMediaList());
        break;
    case OPTION_INPUT_STRING:
        ui.InputString();
        break;
    default:
        break;
    }
}

void Controller::InputHandler(MediaFileManage& fileManage, bool* quit, KEY key, std::string str)
{
    switch (key.first) {
    case UNKNOWN_KEY:
        return;
    case FN_KEY:
        switch (key.second) {
            case KEYC_QUIT:
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
                    UpdateCurrentAndNextTrack(fileManage.GetCurrentTrack() + 1);
                    player.PlayTheSong(fileManage.GetTrackPath(fileManage.GetCurrentTrack()));
                }
                break;
            case KEYC_PREV:
                if (mode == OPTION_PLAY_MUSIC_DIRECTORY || mode == OPTION_PLAY_MUSIC_NORMAL) {
                    UpdateCurrentAndNextTrack(fileManage.GetCurrentTrack() - 1);
                    player.PlayTheSong(fileManage.GetTrackPath(fileManage.GetCurrentTrack()));
                }
                break;
            case KEYC_VOLUME_DOWN:
                player.TurnVolumeDown();
                break;
            case KEYC_VOLUME_UP:
                player.TurnVolumeUp();
                break;
            default:
                break;
        }
        break;
    case RET_KEY:
        if (mode == OPTION_EDIT_METADATA) {
            mode = OPTION_INPUT_STRING;
        } else if (mode == OPTION_INPUT_STRING) {
            fileManage.UpdateMetadata((ui.GetMainSide()) ? ui.GetLastLeftIndex() : 0, GetHighlight(), str);
            mode = OPTION_EDIT_METADATA;
            ui.ResizeWindow(ui.GetMainWindow(), MEDIA_LIST_WINDOW_HEIGHT, MEDIA_LIST_WINDOW_WIDTH);
            refresh();
        } else {
            UpdateCurrentAndNextTrack(key.second);
            if (mode == OPTION_PLAY_MUSIC_DIRECTORY || mode == OPTION_PLAY_MUSIC_NORMAL) {
                Mix_HookMusicFinished(MusicFinished);
            }
            player.PlayTheSong(fileManage.GetTrackPath(key.second));
        }
        break;
    default:
        break;
    }
}

void Controller::Run(void)
{
    bool quit = false;
    KEY pressKey;
    std::string inputString = "";
    MediaFileManage& fileManage = GetFileManage();

    while (!quit) {
        UpdateScreen(fileManage);
        if (mode == OPTION_INPUT_STRING) {
            inputString = ui.GetStringInput();
        } else {
            pressKey = ui.GetInput();
        }
        InputHandler(fileManage, &quit, pressKey, inputString);
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
    ui.UpdateMaxReach(GetFileManage().GetTotalTrack());
}

Controller::~Controller()
{

}
