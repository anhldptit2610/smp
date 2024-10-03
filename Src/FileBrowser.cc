#include "FileBrowser.h"
#include <fstream>

bool IsMP3File(const std::filesystem::directory_entry& fileEntry)
{
    return (fileEntry.path().extension() == ".mp3");
}

bool IsMP4File(const std::filesystem::directory_entry& fileEntry)
{
    return (fileEntry.path().extension() == ".mp4");
}

void BrowsePath(FILELIST& list, std::string bPath)
{
    namespace fs = std::filesystem;

    // MediaFileManage& fileManage = GetFileManage();
    // FILELIST& list = fileManage.GetMediaList();

    list.clear();

    if (!fs::is_directory(bPath)) {
        if (fs::path(bPath).filename().extension() == ".playlist") {
            std::ifstream f(bPath.c_str());
            if (f.is_open()) {
                std::string line;
                while (std::getline(f, line)) {
                    std::shared_ptr<MediaFile> mf = std::make_shared<MediaFile>(line);
                    list.push_back(mf);
                }
                f.close();
            }
        } else {
            std::shared_ptr<MediaFile> mf = std::make_shared<MediaFile>(bPath);
            list.push_back(mf);
        }
    } else {
        for (const fs::directory_entry& dir_entry : fs::recursive_directory_iterator(bPath)) {
            if (IsMP3File(dir_entry) || IsMP4File(dir_entry)) {
                std::shared_ptr<MediaFile> mf = std::make_shared<MediaFile>(dir_entry.path());
                list.push_back(mf);
            }
        }
    }
}