#include "Controller.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <unistd.h>

const std::string PLAYLIST_PATH = "/home/anhld/Projects/test/playlist/";

void UpdateCurrentAndNextTrack(int n)
{
    MediaFileManage& fileManage = GetFileManage();

    fileManage.SetCurrentTrack(n);
    fileManage.SetNextTrack(n + 1);
}

void MusicFinished(void)
{
    MediaFileManage& fileManage = GetFileManage();

    // TODO: add option to turn off looping
    SetPlayState(false);
    KillTimeThread();
    if (fileManage.IsLooping()) {
        PlaySong(fileManage.GetTrackPath(fileManage.GetNextTrack()));
        UpdateHighlight(fileManage.GetCurrentTrack());
    }
}

void Controller::SwitchMode(int newMode)
{
    prevMode = mode;
    mode = newMode;
}

void Controller::SetWorkPath(FILELIST& list, std::string _path, int _mode)
{
    workPath = _path;
    mode = _mode;
    BrowsePath(list, workPath, workPath, mode);
    GetFileManage().SetTotalTrack(GetFileManage().GetMediaList().size());
}

void Controller::ParseArgument(int argc, char *argv[])
{
    int opt;
    std::string str;

    while ((opt = getopt(argc, argv, "d:e:p:n")) != -1) {
        switch (opt) {
        case 'd':
            str = optarg;
            if (!std::filesystem::is_directory(str)) {
                std::cout << "It's a file..." << std::endl;
                ok = false;
                return;
            }
            SetWorkPath(GetFileManage().GetMediaList(), str, OPTION_PLAY_MUSIC_DIRECTORY);
            break;
        case 'e':
            str = optarg;
            SetWorkPath(GetFileManage().GetMediaList(), str, OPTION_EDIT_METADATA);
            break;
        case 'p':
            str = optarg;
            SetWorkPath(GetFileManage().GetMediaList(), str, 
                    (std::filesystem::is_directory(str)) ? OPTION_PLAYLIST_DIR : OPTION_PLAYLIST_FILE);
            GetFileManage().SetPlaylistPath(str);
            break;
        case 'n':
            prevMode = OPTION_CREATE_PLAYLIST;
            mode = OPTION_INPUT_STRING;
            break; 
        case '?':
            if (optopt == 'p') {
                str = PLAYLIST_PATH;
                SetWorkPath(GetFileManage().GetMediaList(), str, OPTION_LIST_PLAYLIST);
            } else {
                ok = false;
            }
            break;
        default:
            ok = false;
            break;
        }
    }
}

void Controller::UpdateScreen(MediaFileManage& fileManage)
{
    switch (mode) {
    case OPTION_LIST_PLAYLIST:
    case OPTION_PLAY_MUSIC_DIRECTORY:
    case OPTION_PLAY_MUSIC_NORMAL:
    case OPTION_PLAYLIST_FILE:
    case OPTION_PLAYLIST_DIR:
        ui.DirectoryLayout(workPath, fileManage.GetMediaList(), mode);
        break;
    case OPTION_EDIT_METADATA:
        ui.EditMetadata(exePath, fileManage.GetMediaList(), mode);
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
    auto BackToMainWindow = [this](int _mode) -> void {
        mode = _mode;
        ui.ResizeWindow(GetMainWindow(), MEDIA_LIST_WINDOW_HEIGHT, MEDIA_LIST_WINDOW_WIDTH);
         refresh();
    };

    auto CheckMode = [this](void) -> bool {
        return (mode == OPTION_PLAY_MUSIC_DIRECTORY || mode == OPTION_PLAY_MUSIC_NORMAL 
                || mode == OPTION_PLAYLIST_FILE);
    };

    auto UpdateTrackInfoAndPlaySong = [this, &fileManage](int n) -> void {
        UpdateCurrentAndNextTrack(fileManage.GetCurrentTrack() + 1);
        UpdateHighlight(fileManage.GetCurrentTrack());
        player.PlayTheSong(GetHighlight(), fileManage.GetTrackPath(fileManage.GetCurrentTrack()));
    };

    switch (key.first) {
    case UNKNOWN_KEY:
        return;
    case FN_KEY:
        switch (key.second) {
            case KEYC_QUIT:
                *quit = true;
                KillTimeThread();
                break;
            case KEYC_PAUSE:
                if (CheckMode()) {
                    SetPlayState(false);
                    player.PauseTheSong();
                }
                break;
            case KEYC_RESUME:
                if (CheckMode()) {
                    SetPlayState(true);
                    player.ResumeTheSong();
                }
                break;
            case KEYC_NEXT:
                if (CheckMode())
                    UpdateTrackInfoAndPlaySong(fileManage.GetCurrentTrack() + 1);
                break;
            case KEYC_PREV:
                if (CheckMode())
                    UpdateTrackInfoAndPlaySong(fileManage.GetCurrentTrack() - 1);
                break;
            case KEYC_PLAYLIST_RET:
                if (mode == OPTION_PLAYLIST_FILE) {
                    SwitchMode(OPTION_LIST_PLAYLIST);
                    SetWorkPath(fileManage.GetMediaList(), PLAYLIST_PATH, OPTION_LIST_PLAYLIST);
                }
                break;
            case KEYC_VOLUME_DOWN:
                player.TurnVolumeDown();
                break;
            case KEYC_VOLUME_UP:
                player.TurnVolumeUp();
                break;
            case KEYC_LOOPING_TOGGLE:
                if (CheckMode())
                    fileManage.ToggleLooping();
                break;
            case KEYC_ADD:
                if (mode == OPTION_PLAYLIST_FILE) {
                    SwitchMode(OPTION_INPUT_STRING);
                }
                break;
            case KEYC_DELETE:
                if (mode == OPTION_PLAYLIST_FILE) {
                    fileManage.DeleteTrack(GetHighlight());
                    ui.UpdateMaxReach(fileManage.GetTotalTrack());
                }
                break;
            default:
                break;
        }
        break;
    case RET_KEY:
        if (mode == OPTION_EDIT_METADATA) {
            SwitchMode(OPTION_INPUT_STRING);
        } else if (mode == OPTION_LIST_PLAYLIST) {
            fileManage.SetPlaylistPath(fileManage.GetTrackPath(GetHighlight()));
            fileManage.ParsePlaylist(fileManage.GetTrackPath(GetHighlight()));
            ui.UpdateMaxReach(fileManage.GetTotalTrack());
            SwitchMode(OPTION_PLAYLIST_FILE);
        } else {
            UpdateCurrentAndNextTrack(key.second);
            Mix_HookMusicFinished(MusicFinished);
            player.PlayTheSong(key.second, fileManage.GetTrackPath(key.second));
        }
        break;
    case STRING:
        if (prevMode == OPTION_EDIT_METADATA) {
            fileManage.UpdateMetadata((ui.GetMainSide()) ? ui.GetLastLeftIndex() : 0, GetHighlight(), str);
            BackToMainWindow(OPTION_EDIT_METADATA);
        }  else if (prevMode == OPTION_PLAYLIST_FILE) {
            SwitchMode(OPTION_PLAYLIST_FILE);
            fileManage.AddNewTrack(str);
            BackToMainWindow(OPTION_PLAYLIST_FILE);
            ui.UpdateMaxReach(GetFileManage().GetTotalTrack());
        } else if (prevMode == OPTION_CREATE_PLAYLIST) {
            std::string finalPath = PLAYLIST_PATH + str + ".playlist";
            GetFileManage().CreateNewPlaylist(finalPath);
            *quit = true;
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
            pressKey.first = STRING;
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
        BrowsePath(GetFileManage().GetMediaList(), exePath, workPath, mode);
    }
    ui.UpdateMaxReach(GetFileManage().GetTotalTrack());
}

Controller::~Controller()
{

}
