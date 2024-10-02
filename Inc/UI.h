#ifndef UI_H_
#define UI_H_

#include "Common.h"
#include "FileBrowser.h"
#include <ncurses.h>
#include "Metadata.h"

enum WIN {
    LEFT,
    RIGHT
};

class UI {
private:
    int minReach = -1;
    int maxReach = 0;
    int mainWindow;     // left is 0
    int pagination;
    int lastLeftIndex;
    int listMaxReach = 0;
    WINDOW *leftWin = nullptr;
    WINDOW *rightWin = nullptr;
public:
    void DirectoryLayout(FILELIST& mediaList);
    void EditMetadata(FILELIST& list);
    void InputString(void);
    KEY GetInput(void);
    std::string GetStringInput(void);
    void ClearWindowButBox(WINDOW* win);
    void ClearWindow(WINDOW* win);
    void ResizeWindow(WINDOW *win, int newHeight, int newWidth);
    void DrawDirectoryLeftWin(FILELIST& list);
    int GetMainSide(void);
    int GetLastLeftIndex(void);

    /* getter/setter */
    void UpdateMaxReach(int maxReach);
    WINDOW * GetMainWindow(void);
    WINDOW * GetSideWindow(void);

    /* constructor/destructor */
    UI();
    ~UI();
};

int GetHighlight(void);

void UpdateHighlight(int n);

#endif