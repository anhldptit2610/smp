#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <memory>
#include <filesystem>
#include <vector>

#define MEDIA_LIST_WINDOW_HEIGHT        27
#define MEDIA_LIST_WINDOW_WIDTH         100

#define KEY_RETURN                      '\n'
#define KEYC_PAUSE                      'p'                   
#define KEYC_NEXT                       'n'
#define KEYC_PREV                       'b'
#define KEYC_RESUME                     'r'
#define KEYC_QUIT                       'q'
#define KEYC_BACK                       'k'
#define KEYC_VOLUME_UP                  '='
#define KEYC_VOLUME_DOWN                '-'

#define FILELIST            std::vector<std::filesystem::directory_entry>

enum OPTION {
    OPTION_PLAY_MUSIC_NORMAL,
    OPTION_PLAY_MUSIC_SINGLE,
    OPTION_PLAY_MUSIC_DIRECTORY,
    OPTION_EDIT_METADATA,
};

#endif