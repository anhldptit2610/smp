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

    if (y == highlight + 1 && highlight != -1)
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

void Metadata::Update(std::string filePath, int key, std::string val)
{
    TagLib::FileRef file(filePath.c_str());

    if (!file.isNull() && file.tag()) {
        // Access the tag
        TagLib::Tag *tag = file.tag();

        switch (key) {
        case MK_TITLE:
            tag->setTitle(val);
            SetTitle(val);
            break;
        case MK_ARTIST:
            tag->setArtist(val);
            SetArtist(val);
            break;
        case MK_ALBUM:
            tag->setAlbum(val);
            SetAlbum(val);
            break;
        case MK_YEAR:
            tag->setYear(atoi(val.c_str()));
            SetYear(val);
            break;
        case MK_COMMENT:
            tag->setComment(val);
            SetComment(val);
            break;
        case MK_TRACK:
            tag->setTrack(atoi(val.c_str()));
            SetTrack(val);
            break;
        case MK_GENRE:
            tag->setGenre(val);
            SetGenre(val);
            break;
        default:
            break;
        }

        // Save the updated tags back to the file
        file.save();
        
        std::cout << "Tag updated successfully!" << std::endl;
    }
}

/* getter/setter */
std::string Metadata::GetTitle(void) { return title; }
std::string Metadata::GetArtist(void) { return artist; }
std::string Metadata::GetAlbum(void) { return album; }
std::string Metadata::GetYearInString(void) { return std::to_string(year); }
std::string Metadata::GetComment(void) { return comment; }
std::string Metadata::GetTrackInString(void) { return std::to_string(track); }
std::string Metadata::GetGenre(void) { return genre; }
void Metadata::SetTitle(std::string _title) { title = _title; }
void Metadata::SetArtist(std::string _artist) { artist = _artist; }
void Metadata::SetAlbum(std::string _album) { album = _album; }
void Metadata::SetYear(std::string _year) { year = atoi(_year.c_str()); }
void Metadata::SetTrack(std::string _track) { track = atoi(_track.c_str()); }
void Metadata::SetGenre(std::string _genre) { genre = _genre; }
void Metadata::SetComment(std::string _comment) { comment = _comment; }

Metadata::Metadata(std::string filePath)
{
    TagLib::FileRef f(filePath.c_str());

    if (!f.isNull() && f.tag()) {
        TagLib::Tag *tag = f.tag();
        title = tag->title().to8Bit();
        artist = tag->artist().to8Bit();
        album = tag->album().to8Bit();
        year = tag->year();
        comment = tag->comment().to8Bit();
        track = tag->track();
        genre = tag->genre().to8Bit();
    }
}

Metadata::~Metadata()
{

}