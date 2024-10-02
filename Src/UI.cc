#include "UI.h"
#include "Metadata.h"

static int highlight = 0;
void PrintMetadata(WINDOW *win, Metadata& metadata, int highlight = -1);

WINDOW * UI::GetMainWindow(void)
{
    return mainWin;
}

WINDOW * UI::GetSideWindow(void)
{
    return sideWin;
}


int GetHighlight(void)
{
    return highlight;
}

void UpdateHighlight(int n)
{
    highlight = n;
}

int UI::GetMainSide(void)
{
    return mainWindow;
}

int UI::GetLastLeftIndex(void)
{
    return lastLeftIndex;
}

void UI::UpdateMaxReach(int _mr) 
{
    maxReach = _mr;
    listMaxReach = maxReach;
}

void UI::ClearAllWindow(WINDOW* win)
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
    mvwgetnstr(mainWin, 1, 1, str, sizeof(str) - 1);
    noecho();
    return str;
}

KEY UI::GetInput(void)
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
    // TODO: make these magic number disappear
    case 'p':
    case 'q':
    case 'r':
    case 'b':
    case 'n':
    case 'k':
    case '-':
    case '=':
        return KEY(FN_KEY, opt);
    case KEYC_RETURN:
        return KEY(RET_KEY, highlight);
    default:
        break;
    }
    return KEY(UNKNOWN_KEY, -1);      // nothing special was pressed
}

void UI::Test(void)
{
    mvwprintw(mainWin, 1, 1, "Hello world");
}

void UI::ResizeWindow(WINDOW *win, int newHeight, int newWidth)
{
    clear();
    wclear(win);
    wresize(win, newHeight, newWidth);
    box(win, 0, 0);
}

void UI::DirectoryLayout(FILELIST& list)
{
    int y = 1;
    MediaFileManage& fileManage = GetFileManage();

    ClearWindowButBox(mainWin);
    mvwprintw(mainWin, 0, 40, " Choose media file ");
    for (int i = 25 * (pagination - 1); i < (25 * (pagination - 1) + 25) && i < list.size(); i++) {
        if (i == highlight)
            wattron(mainWin, A_REVERSE);
        mvwprintw(mainWin, y, 1, list[i]->GetPath().c_str());
        wattroff(mainWin, A_REVERSE);
        y++;
    }
    wrefresh(mainWin);
    PrintMetadata(sideWin, list[highlight]->GetMetadata(), -1);
    mvwprintw(sideWin, 0, 10, " Metadata ");
    wrefresh(sideWin);
}

void UI::EditMetadata(FILELIST& list)
{
    int y = 1;

    ClearWindowButBox(mainWin);
    mvwprintw(mainWin, 0, 40, " Choose media file ");
    for (int i = 25 * (pagination - 1); i < (25 * (pagination - 1) + 25) && i < list.size(); i++) {
        if (i == highlight && mainWindow == LEFT)
            wattron(mainWin, A_REVERSE);
        mvwprintw(mainWin, y, 1, list[i]->GetPath().c_str());
        wattroff(mainWin, A_REVERSE);
        y++;
    }
    wrefresh(mainWin);
    if (mainWindow == RIGHT) {
        PrintMetadata(sideWin, list[lastLeftIndex]->GetMetadata(), highlight);
    // if (mainWindow == LEFT)
    } else if (mainWindow == LEFT) {
        PrintMetadata(sideWin, list[highlight]->GetMetadata(), -1);
    }
    // PrintMetadata(mainWin, list[0]->GetMetadata(), highlight + 1);
    mvwprintw(sideWin, 0, 8, "Edit Metadata");
    wrefresh(sideWin);
}

void UI::InputString(void)
{
    ClearAllWindow(mainWin);
    ClearAllWindow(sideWin);
    refresh();
    ResizeWindow(mainWin, INPUT_WINDOW_HEIGHT, INPUT_WINDOW_WIDTH);
    mvwprintw(mainWin, 0, 1, "Enter a string: ");
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
    wrefresh(win);
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
    // refresh();
    // wrefresh(mainWin);

    sideWin = newwin(MEDIA_LIST_WINDOW_HEIGHT, 30, 0, MEDIA_LIST_WINDOW_WIDTH);  
    keypad(sideWin, TRUE);
    box(sideWin, 0, 0);
    // refresh();
    // wrefresh(sideWin);

    highlight = 0;
    pagination = 1;
    mainWindow = LEFT;
    lastLeftIndex = 0;

    // ClearAllWindow(mainWin);
    // ClearAllWindow(sideWin);
}

UI::~UI()
{
    delwin(mainWin);
    delwin(sideWin);
    endwin();
}