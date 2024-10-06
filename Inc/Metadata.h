#ifndef METADATA_H_
#define METADATA_H_

#include <string>
#include <ncurses.h>

enum METADATA_KEY {
    MK_TITLE,
    MK_ARTIST,
    MK_ALBUM,
    MK_YEAR,
    MK_COMMENT,
    MK_TRACK,
    MK_GENRE,
};

class Metadata {
private:
    std::string title;
    std::string artist;
    std::string album;
    unsigned int year;
    std::string comment;
    unsigned int track;
    unsigned int length;
    unsigned int min;
    unsigned int sec;
    std::string genre;
public:
    void Update(std::string filePath, int key, std::string val);

    /* getter/setter */
    std::string GetTitle(void);
    std::string GetArtist(void);
    std::string GetAlbum(void);
    std::string GetYearInString(void);
    std::string GetTrackInString(void);
    std::string GetComment(void);
    std::string GetLengthInString(void);
    unsigned int GetLength(void);
    unsigned int GetMin(void);
    unsigned int GetSec(void);
    unsigned int GetTrack(void);
    std::string GetGenre(void);
    void SetTitle(std::string);
    void SetArtist(std::string);
    void SetAlbum(std::string);
    void SetYear(std::string);
    void SetComment(std::string);
    void SetTrack(std::string);
    void SetGenre(std::string);


    /* constructor/destructor */
    Metadata(std::string filePath);
    ~Metadata();
};

void PrintMetadataKey(WINDOW *win, int y, int x, std::string firstPart, std::string key, int highlight = -1);

#endif