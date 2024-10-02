#include "FileBrowser.h"

bool IsMP3File(const std::filesystem::directory_entry& fileEntry)
{
    return (fileEntry.path().extension() == ".mp3");
}

bool IsMP4File(const std::filesystem::directory_entry& fileEntry)
{
    return (fileEntry.path().extension() == ".mp4");
}

void BrowsePath(std::string path)
{
    namespace fs = std::filesystem;

    MediaFileManage& fileManage = GetFileManage();
    FILELIST& list = fileManage.GetMediaList();

    list.clear();

    if (!fs::is_directory(path)) {
        std::shared_ptr<MediaFile> mf = std::make_shared<MediaFile>(path);
        list.push_back(mf);
    } else {
        for (const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(path)) {
            if (IsMP3File(dir_entry) || IsMP4File(dir_entry)) {
                std::shared_ptr<MediaFile> mf = std::make_shared<MediaFile>(dir_entry.path());
                list.push_back(mf);
            }
        }
    }

    fileManage.SetTotalTrack(list.size());
}