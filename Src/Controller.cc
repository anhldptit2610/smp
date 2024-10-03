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

    auto SetWorkPath = [this](FILELIST& list, std::string _path, int _mode) -> void {
            workPath = _path;
            mode = _mode;
            BrowsePath(list, workPath);
    };

    while ((opt = getopt(argc, argv, "d:e:p:")) != -1) {
        switch (opt) {
        case 'd':
            str = optarg;
            if (!std::filesystem::is_directory(str)) {
                std::cout << "It's a file..." << std::endl;
                ok = false;
                return;
            }
            SetWorkPath(GetFileManage().GetMediaList(), str, OPTION_PLAY_MUSIC_DIRECTORY);
            GetFileManage().SetTotalTrack(GetFileManage().GetMediaList().size());
            break;
        case 'e':
            if (optarg == NULL) {
                ok = false;
                return;
            }
            str = optarg;
            SetWorkPath(GetFileManage().GetMediaList(), str, OPTION_EDIT_METADATA);
            GetFileManage().SetTotalTrack(GetFileManage().GetMediaList().size());
            break;
        case 'p':
            if (optarg == NULL) {

            } else {
                str = optarg;
                SetWorkPath(playlist.GetPlayList(), str, OPTION_PLAYLIST_FILE);
                playlist.UpdatePath(str);
                mode = OPTION_PLAYLIST_FILE;
            }
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
    case OPTION_PLAYLIST_FILE:
        ui.DirectoryLayout(playlist.GetPlayList());
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
                if (mode == OPTION_PLAY_MUSIC_DIRECTORY 
                    || mode == OPTION_PLAY_MUSIC_NORMAL
                    || mode == OPTION_PLAYLIST_FILE)
                    player.PauseTheSong();
                break;
            case KEYC_RESUME:
                if (mode == OPTION_PLAY_MUSIC_DIRECTORY 
                    || mode == OPTION_PLAY_MUSIC_NORMAL
                    || mode == OPTION_PLAYLIST_FILE)
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
            case KEYC_ADD:
                if (mode == OPTION_PLAYLIST_FILE) {
                    prevMode = mode;
                    mode = OPTION_INPUT_STRING;
                }
                break;
            default:
                break;
        }
        break;
    case RET_KEY:
        if (mode == OPTION_EDIT_METADATA) {
            prevMode = mode;
            mode = OPTION_INPUT_STRING;
        } else if (mode == OPTION_INPUT_STRING) {
            if (prevMode == OPTION_EDIT_METADATA) {
                fileManage.UpdateMetadata((ui.GetMainSide()) ? ui.GetLastLeftIndex() : 0, GetHighlight(), str);
                mode = OPTION_EDIT_METADATA;
                ui.ResizeWindow(ui.GetMainWindow(), MEDIA_LIST_WINDOW_HEIGHT, MEDIA_LIST_WINDOW_WIDTH);
                refresh();
            }  else if (prevMode == OPTION_PLAYLIST_FILE) {
                playlist.AddNewTrack(str);
                mode = OPTION_PLAYLIST_FILE;
                ui.ResizeWindow(ui.GetMainWindow(), MEDIA_LIST_WINDOW_HEIGHT, MEDIA_LIST_WINDOW_WIDTH);
                refresh();
            }
        } else if (mode == OPTION_PLAYLIST_FILE) {
            player.PlayTheSong(playlist.GetTrackPath(key.second));
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
            pressKey.first = RET_KEY;
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
        BrowsePath(GetFileManage().GetMediaList(), workPath);
    }
    ui.UpdateMaxReach(GetFileManage().GetTotalTrack());
}

Controller::~Controller()
{

}
