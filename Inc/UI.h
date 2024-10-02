#ifndef UI_H_
#define UI_H_

#include "Common.h"
#include "FileBrowser.h"
#include <ncurses.h>

class UI {
private:
    int minReach = -1;
    int maxReach = 0;
    int pagination;
    WINDOW *mainWin = nullptr;
    WINDOW *sideWin = nullptr;
public:
    void DirectoryLayout(FILELIST& mediaList);
    void Test(void);
    int GetInput(void);
    void ClearWindow(WINDOW* win);

    /* getter/setter */
    void UpdateMaxReach(int maxReach);

    /* constructor/destructor */
    UI();
    ~UI();
};

void UpdateHighlight(int n);

#endif