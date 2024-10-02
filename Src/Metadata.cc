#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>
#include "Metadata.h"

void PrintMetadataKey(WINDOW *win, int y, int x, std::string firstPart, std::string key, int highlight)
{
    std::string msg = firstPart + key;

    if (y == highlight && highlight != -1)
        wattron(win, A_REVERSE);
    mvwprintw(win, y, x, msg.c_str());
    wattroff(win, A_REVERSE);
}

// Make sure win is always open
void PrintMetadata(WINDOW *win, std::string path, int key)
{
    TagLib::FileRef f(path.c_str());

    if (!f.isNull() && f.tag()) {
        TagLib::Tag *tag = f.tag();
        wclear(win);
        box(win, 0, 0);
        PrintMetadataKey(win, 1, 1, "Title: ",  tag->title().to8Bit(), key);
        PrintMetadataKey(win, 2, 1, "Artist: ", tag->artist().to8Bit(), key);
        PrintMetadataKey(win, 3, 1, "Album: ", tag->album().to8Bit(), key);
        PrintMetadataKey(win, 4, 1, "Year: ", std::to_string(tag->year()), key);
        PrintMetadataKey(win, 5, 1, "Comment: ", tag->comment().to8Bit(), key);
        PrintMetadataKey(win, 6, 1, "Track: ", std::to_string(tag->track()), key);
        PrintMetadataKey(win, 7, 1, "Genre: ", tag->genre().to8Bit(), key);
        wrefresh(win);
    }
}