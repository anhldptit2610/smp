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
    int totalTrack;
    int nextTrack;
    int currentTrack;
public:
    void UpdateMetadata(int index, int key, std::string val);

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
    MediaFileManage();
    ~MediaFileManage();
};

MediaFileManage& GetFileManage(void);

#endif