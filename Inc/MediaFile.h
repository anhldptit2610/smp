#ifndef MEDIA_FILE_H_
#define MEDIA_FILE_H_

#include "Common.h"
#include "Metadata.h"

class MediaFile {
private:
    std::string path;
    Metadata metadata;
public:
    /* getter/setter */
    std::string GetPath(void);
    Metadata& GetMetadata(void);
    unsigned int GetMin(void);
    unsigned int GetSec(void);

    /* constructor/destructor */
    MediaFile(std::string filePath);
    ~MediaFile();
};

#define FILELIST            std::vector<std::shared_ptr<MediaFile>>

class MediaFileManage {
private:    
    FILELIST list;
    std::string playlistPath;
    int totalTrack;
    int nextTrack;
    int currentTrack;
    bool looping;
    unsigned int min;
    unsigned int sec;
public:
    void UpdateMetadata(int index, int key, std::string val);
    void AddNewTrack(std::string val);
    void DeleteTrack(int index);
    void CreateNewPlaylist(std::string playlist);
    void ParsePlaylist(std::string playlist);

    /* getter/setter */
    int GetTotalTrack(void);
    void SetTotalTrack(int n);
    int GetNextTrack(void);
    void SetNextTrack(int n);
    void NextTrack(void);
    void SetPlaylistPath(std::string path);
    int GetCurrentTrack(void);
    void SetCurrentTrack(int n);
    std::string GetTrackPath(int n);
    FILELIST& GetMediaList(void);
    bool IsLooping(void);
    void ToggleLooping(void);
    unsigned int GetMin(void);
    unsigned int GetSec(void);
    void SetMin(unsigned int _m);
    void SetSec(unsigned int _s);

    /* constructor/destructor */
    MediaFileManage();
    ~MediaFileManage();
};

MediaFileManage& GetFileManage(void);

void SetPlayState(bool state);      // 0 is off, 1 is on
void IncrementTime(void);
int GetCurrentTime(void);
void ResetTime(void);
void timeTracker(WINDOW *win);
void KillTimeThread(void);


#endif