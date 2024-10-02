#ifndef MEDIA_FILE_H_
#define MEDIA_FILE_H_

#include "Common.h"

class MediaFile {
private:
    FILELIST list;
    int totalTrack;
    int nextTrack;
    int currentTrack;
public:
    /* getter/setter */
    int GetTotalTrack(void);
    void SetTotalTrack(int n);
    int GetNextTrack(void);
    void SetNextTrack(int n);
    int GetCurrentTrack(void);
    void SetCurrentTrack(int n);
    std::string GetTrackPath(int n);
    FILELIST& GetMediaList(void);
    /* constructor/destructor */
    MediaFile();
    ~MediaFile();
};

MediaFile& GetMediaFile(void);

#endif