#ifndef PLAYLIST_H_
#define PLAYLIST_H_

#include "Common.h"
#include "MediaFile.h"

class Playlist {
private:
    FILELIST list;
    std::string path;
public:
    FILELIST& GetPlayList();
    std::string GetTrackPath(int i);
    void AddNewTrack(std::string path);
    void UpdatePath(std::string _path);
    /* constructor/destructor */
    Playlist();
    ~Playlist();

};

#endif