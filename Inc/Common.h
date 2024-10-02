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

#define KEYC_RETURN                      '\n'
#define KEYC_PAUSE                      'p'                   
#define KEYC_NEXT                       'n'
#define KEYC_PREV                       'b'
#define KEYC_RESUME                     'r'
#define KEYC_QUIT                       'q'
#define KEYC_BACK                       'k'
#define KEYC_VOLUME_UP                  '='
#define KEYC_VOLUME_DOWN                '-'

#define KEY                             std::pair<int, int>

enum OPTION {
    OPTION_PLAY_MUSIC_NORMAL,
    OPTION_PLAY_MUSIC_SINGLE,
    OPTION_PLAY_MUSIC_DIRECTORY,
    OPTION_EDIT_METADATA,
    OPTION_INPUT_STRING,
};

#endif