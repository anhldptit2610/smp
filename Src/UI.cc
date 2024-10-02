#include "UI.h"
#include "Metadata.h"

static int highlight = 0;

void UpdateHighlight(int n)
{
    highlight = n;
}

void UI::UpdateMaxReach(int _mr) 
{
    maxReach = _mr;
}

void UI::ClearWindow(WINDOW *win)
{
    clear();
    wclear(win);
    box(win, 0, 0);
}

int UI::GetInput(void)
{
    int opt = wgetch(mainWin);

    switch (opt) {
    case KEY_UP:
        highlight--;
        if ((highlight + 1 <= 25 * (pagination - 1)) && (pagination > 1)) {
            pagination--;
        }
        if (highlight == minReach)
            highlight = 0;
        break;
    case KEY_DOWN:
        highlight++;
        if (highlight + 1 > 25 * pagination) {
            pagination++;
        }
        if (highlight == 100)
            highlight = 100 - 1;
        break; 
    // TODO: make these magic number disappear
    case 'p':
    case 'q':
    case 'r':
    case 'b':
    case 'n':
    case 'k':
        return opt;
    case KEY_RETURN:
        return highlight;
    default:
        break;
    }
    return -1;      // nothing special was pressed
}

void UI::Test(void)
{
    mvwprintw(mainWin, 1, 1, "Hello world");
}

void UI::DirectoryLayout(FILELIST& list)
{
    int y = 1;
    MediaFile& mediaFile = GetMediaFile();

    ClearWindow(mainWin);
    for (int i = 25 * (pagination - 1); i < (25 * (pagination - 1) + 25) && i < list.size(); i++) {
        if (i == highlight)
            wattron(mainWin, A_REVERSE);
        mvwprintw(mainWin, y, 1, list[i].path().c_str());
        wattroff(mainWin, A_REVERSE);
        y++;
    }
    wrefresh(mainWin);
    PrintMetadata(sideWin, mediaFile.GetTrackPath(highlight), -1);
}

/* constructor/destructor */

UI::UI()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    mainWin = newwin(MEDIA_LIST_WINDOW_HEIGHT, MEDIA_LIST_WINDOW_WIDTH, 0, 0);  
    keypad(mainWin, TRUE);
    box(mainWin, 0, 0);
    refresh();
    wrefresh(mainWin);

    sideWin = newwin(MEDIA_LIST_WINDOW_HEIGHT, 30, 0, MEDIA_LIST_WINDOW_WIDTH);  
    keypad(sideWin, TRUE);
    box(sideWin, 0, 0);
    refresh();
    wrefresh(sideWin);

    highlight = 0;
    pagination = 1;
}

UI::~UI()
{
    delwin(mainWin);
    delwin(sideWin);
    endwin();
}