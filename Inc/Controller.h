#ifndef SOL_H_
#define SOL_H_

#include "Common.h"
#include "FileBrowser.h"
#include "Player.h"
#include "UI.h"
#include "Playlist.h"
#include "MediaFile.h"
#include "Metadata.h"

class Controller {
private:
    UI ui;
    Player player;

    std::filesystem::path exePath;
    std::filesystem::path workPath;
    int mode = OPTION_PLAY_MUSIC_NORMAL;
    int prevMode;
    int fileKey;
    std::string path;
    bool ok;

    void ParseArgument(int argc, char *argv[]);
    void SwitchMode(int newMode);
    void SetWorkPath(FILELIST& list, std::string _path, int _mode);
public:

    void Run(void);
    void UpdateScreen(MediaFileManage& fileManage);
    void InputHandler(MediaFileManage& fileManage, bool* quit, KEY key, std::string str = "");
    void PlayMusicFromDirectory(void);
    bool IsOK(void);

    /* constructor/destructor */
    Controller(int argc, char *argv[]);
    ~Controller();
};

int InitSDL(void);
void DestroySDL(void);

#endif