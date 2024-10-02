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

    MediaFile& mediaFile = GetMediaFile();
    FILELIST& list = mediaFile.GetMediaList();

    // std::string finalPath;

    list.clear();

    // if (path == "..") {
        // finalPath = cwd.parent_path().string();
        // cwd = cwd.parent_path();
        // fileList.push_back(fs::directory_entry(cwd));
    // } else {
        // cwd = path;
        // finalPath = path;
        // fileList.push_back(fs::directory_entry(cwd));
    // }
    // list.push_back(fs::directory_entry(".."));
    for (const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(path)) {
        if (IsMP3File(dir_entry) || IsMP4File(dir_entry))
            list.push_back(dir_entry);
    }
    mediaFile.SetTotalTrack(list.size());
}