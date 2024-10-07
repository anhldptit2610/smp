#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <cstdio>
#include <memory>
#include <filesystem>
#include <vector>
#include <list>
#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>

#define MEDIA_LIST_WINDOW_HEIGHT        27
#define MEDIA_LIST_WINDOW_WIDTH         80
#define EDIT_METADATA_WINDOW_HEIGHT     10
#define EDIT_METADATA_WINDOW_WIDTH      30
#define INPUT_WINDOW_HEIGHT             3
#define INPUT_WINDOW_WIDTH              50

enum KEY_TYPE {
    FN_KEY,
    RET_KEY,    
    STRING,
    UNKNOWN_KEY,
};

#define KEYC_RETURN                     '\n'
#define KEYC_PAUSE                      112             // 'p'
#define KEYC_NEXT                       110             // 'n'
#define KEYC_PREV                       98              // 'b'
#define KEYC_RESUME                     114             // 'r'
#define KEYC_QUIT                       113             // 'q'
#define KEYC_BACK                       107             // 'k'
#define KEYC_PLAYLIST_RET               122             // 'z'
#define KEYC_VOLUME_UP                  61              // '='
#define KEYC_VOLUME_DOWN                45              // '-'
#define KEYC_LOOPING_TOGGLE             108             // 'l'
#define KEYC_EDIT                       101             // 'e'
#define KEYC_ADD                        97              // 'a'
#define KEYC_DELETE                     100             // 'd'

#define KEY                             std::pair<int, int>
#define PATH                            std::filesystem::path

enum OPTION {
    OPTION_PLAY_MUSIC_NORMAL,
    OPTION_PLAY_MUSIC_SINGLE,
    OPTION_PLAY_MUSIC_DIRECTORY,
    OPTION_EDIT_METADATA,
    OPTION_INPUT_STRING,
    OPTION_PLAYLIST_DIR,
    OPTION_PLAYLIST_FILE,
    OPTION_CREATE_PLAYLIST,
    OPTION_LIST_PLAYLIST,
};

#endif