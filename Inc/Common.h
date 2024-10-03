#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <memory>
#include <filesystem>
#include <vector>

#define MEDIA_LIST_WINDOW_HEIGHT        27
#define MEDIA_LIST_WINDOW_WIDTH         100
#define EDIT_METADATA_WINDOW_HEIGHT     10
#define EDIT_METADATA_WINDOW_WIDTH      30
#define INPUT_WINDOW_HEIGHT             3
#define INPUT_WINDOW_WIDTH              50

enum KEY_TYPE {
    FN_KEY,
    RET_KEY,    
    UNKNOWN_KEY,
};

#define KEYC_RETURN                     '\n'
#define KEYC_PAUSE                      112                   
#define KEYC_NEXT                       110
#define KEYC_PREV                       98
#define KEYC_RESUME                     114
#define KEYC_QUIT                       113
#define KEYC_BACK                       107
#define KEYC_VOLUME_UP                  61 
#define KEYC_VOLUME_DOWN                45
#define KEYC_EDIT                       101
#define KEYC_ADD                        97
#define KEYC_DELETE                     100

#define KEY                             std::pair<int, int>

enum OPTION {
    OPTION_PLAY_MUSIC_NORMAL,
    OPTION_PLAY_MUSIC_SINGLE,
    OPTION_PLAY_MUSIC_DIRECTORY,
    OPTION_EDIT_METADATA,
    OPTION_INPUT_STRING,
    OPTION_CREATE_PLAYLIST,
    OPTION_PLAYLIST_FILE,
};

#endif