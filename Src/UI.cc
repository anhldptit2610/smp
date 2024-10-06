#include "UI.h"
#include "Metadata.h"
#include "Playlist.h"
#include "FileBrowser.h"

static int highlight = 0;
WINDOW *leftWin = nullptr;
WINDOW *timeWin = nullptr;

void PrintMetadata(WINDOW *win, Metadata& metadata, int highlight = -1);

/* getter/setter */

WINDOW * GetMainWindow(void) { return leftWin; }
WINDOW * GetTimeWindow(void) { return timeWin; }
WINDOW * UI::GetSideWindow(void) { return rightWin; }
int GetHighlight(void) { return highlight; }
void UpdateHighlight(int n) { highlight = n; }
int UI::GetMainSide(void) { return mainWindow; }
int UI::GetLastLeftIndex(void) { return lastLeftIndex; }

void UI::UpdateMaxReach(int _mr) 
{
    maxReach = _mr;
    listMaxReach = maxReach;
}

void UI::ClearWindow(WINDOW* win)
{
    clear();
    wclear(win);
}

void UI::ClearWindowButBox(WINDOW *win)
{
    clear();
    wclear(win);
    box(win, 0, 0);
}

std::string UI::GetStringInput(void)
{
    char str[50];

    echo();
    mvwgetnstr(leftWin, 1, 1, str, sizeof(str) - 1);
    noecho();

    return str;
}

KEY UI::GetInput(void)
{
    int opt = wgetch(leftWin);

    switch (opt) {
    case KEY_UP:
        highlight--;
        if ((highlight + 1 <= 25 * (pagination - 1)) && (pagination > 1))
            pagination--;
        if (highlight == minReach)
            highlight = 0;
        break;
    case KEY_DOWN:
        highlight++;
        if (highlight + 1 > 25 * pagination)
            pagination++;
        if (highlight == maxReach)
            highlight = maxReach - 1;
        break; 
    case KEY_RIGHT:
        if (mainWindow == LEFT) {
            mainWindow = RIGHT;
            lastLeftIndex = highlight;
            maxReach = 7;
            highlight = 0;
        }
        break;
    case KEY_LEFT:
        if (mainWindow == RIGHT) {
            mainWindow = LEFT;
            highlight = lastLeftIndex;
            maxReach = listMaxReach;
        }
        break;
    case KEYC_PAUSE:                // 'p'
    case KEYC_QUIT:                 // 'q'
    case KEYC_RESUME:               // 'r'
    case KEYC_BACK:                 // 'k'
    case KEYC_PLAYLIST_RET:         // 'z'
    case KEYC_NEXT:                 // 'n'
    case KEYC_PREV:                 // 'b'
    case KEYC_ADD:                  // 'a'
    case KEYC_DELETE:               // 'd'
    case KEYC_EDIT:                 // 'e'
    case KEYC_VOLUME_DOWN:          // '-'
    case KEYC_VOLUME_UP:            // '+'
    case KEYC_LOOPING_TOGGLE:       // 'l'
        return KEY(FN_KEY, opt);
    case KEYC_RETURN:
        return KEY(RET_KEY, highlight);
    default:
        break;
    }
    return KEY(UNKNOWN_KEY, -1);      // nothing special was pressed
}

void UI::ResizeWindow(WINDOW *win, int newHeight, int newWidth)
{
    clear();
    wclear(win);
    wresize(win, newHeight, newWidth);
    box(win, 0, 0);
}

void UI::DrawDirectoryLeftWin(std::string exePath, FILELIST& list, int hl, int mode)
{
    ClearWindowButBox(leftWin);
    mvwprintw(leftWin, 0, 1, exePath.c_str());
    for (int y = 1, i = 25 * (pagination - 1); i < (25 * (pagination - 1) + 25) && i < list.size(); i++) {
        if (i == hl) {
            wattron(leftWin, A_REVERSE);
        }
        mvwprintw(leftWin, y, 1, (mode == OPTION_PLAYLIST_DIR || mode == OPTION_PLAYLIST_FILE)
                        ? list[i]->GetPath().c_str()
                        : TrimThePath(exePath, list[i]->GetPath()).c_str());
        wattroff(leftWin, A_REVERSE);
        y++;
    }
    wrefresh(leftWin);
}

void UI::DirectoryLayout(std::string exePath, FILELIST& mediaList, int mode)
{
    mvwprintw(GetTimeWindow(), 0, 12, " Time ");
    wrefresh(GetTimeWindow());
    DrawDirectoryLeftWin(exePath, mediaList, highlight, mode);
    PrintMetadata(rightWin, mediaList[highlight]->GetMetadata(), -1);
    mvwprintw(rightWin, 0, 10, " Metadata ");
    wrefresh(rightWin);
}

void UI::EditMetadata(std::string exePath, FILELIST& list, int mode)
{
    DrawDirectoryLeftWin(exePath, list, (mainWindow == LEFT) ? highlight : lastLeftIndex, mode);
    if (mainWindow == RIGHT)
        PrintMetadata(rightWin, list[lastLeftIndex]->GetMetadata(), highlight);
    else if (mainWindow == LEFT)
        PrintMetadata(rightWin, list[highlight]->GetMetadata(), -1);
    mvwprintw(rightWin, 0, 8, "Edit Metadata");
    wrefresh(rightWin);
}

void UI::InputString(void)
{
    ClearWindow(leftWin);
    ClearWindow(rightWin);
    refresh();
    ResizeWindow(leftWin, INPUT_WINDOW_HEIGHT, INPUT_WINDOW_WIDTH);
    mvwprintw(leftWin, 0, 1, "Enter a string: ");
}

void PrintMetadata(WINDOW *win, Metadata& metadata, int highlight)
{
    wclear(win);
    box(win, 0, 0);
    PrintMetadataKey(win, 1, 1, "Title: ", metadata.GetTitle(), highlight);
    PrintMetadataKey(win, 2, 1, "Artist: ", metadata.GetArtist(), highlight);
    PrintMetadataKey(win, 3, 1, "Album: ", metadata.GetAlbum(), highlight);
    PrintMetadataKey(win, 4, 1, "Year: ", metadata.GetYearInString(), highlight);
    PrintMetadataKey(win, 5, 1, "Comment: ", metadata.GetComment(), highlight);
    PrintMetadataKey(win, 6, 1, "Track: ", metadata.GetTrackInString(), highlight);
    PrintMetadataKey(win, 7, 1, "Genre: ", metadata.GetGenre(), highlight);
    PrintMetadataKey(win, 8, 1, "Length: ", metadata.GetLengthInString(), highlight);
    wrefresh(win);
}

/* constructor/destructor */

void CreateWindow(WINDOW **win, int height, int width, int startY, int startX, bool hasBox)
{
    *win = newwin(height, width, startY, startX);  
    keypad(*win, TRUE);
    if (hasBox)
        box(*win, 0, 0);
    refresh();
    wrefresh(*win);
}

UI::UI()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    CreateWindow(&leftWin, MEDIA_LIST_WINDOW_HEIGHT, MEDIA_LIST_WINDOW_WIDTH, 0, 0, true);
    CreateWindow(&rightWin, MEDIA_LIST_WINDOW_HEIGHT - 4, 30, 4, MEDIA_LIST_WINDOW_WIDTH, true);
    CreateWindow(&timeWin, 4, 30, 0, MEDIA_LIST_WINDOW_WIDTH, true);

    highlight = 0;
    pagination = 1;
    mainWindow = LEFT;
    lastLeftIndex = 0;
}

UI::~UI()
{
    delwin(leftWin);
    delwin(rightWin);
    endwin();
}