#ifndef SOL_H_
#define SOL_H_

#include "Common.h"
#include "FileBrowser.h"
#include "Player.h"
#include "UI.h"
#include "MediaFile.h"

class Controller {
private:
    std::filesystem::path exePath;
    std::filesystem::path workPath;
    int mode = OPTION_PLAY_MUSIC_NORMAL;
    int fileKey;
    std::string path;
    UI ui;
    Player player;
    void ParseArgument(int argc, char *argv[]);
public:
    Controller(int argc, char *argv[]);
    ~Controller();

    void Run(void);
    void UpdateScreen(MediaFile& mediaFile);
    void InputHandler(MediaFile& mediaFile, int key, bool* quit);
    void PlayMusicFromDirectory(void);
};

int InitSDL(void);
void DestroySDL(void);

#endif