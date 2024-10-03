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
    void SetPlaylistPath(std::string path);
    int GetCurrentTrack(void);
    void SetCurrentTrack(int n);
    std::string GetTrackPath(int n);
    FILELIST& GetMediaList(void);

    /* constructor/destructor */
    MediaFileManage();
    ~MediaFileManage();
};

MediaFileManage& GetFileManage(void);

#endif