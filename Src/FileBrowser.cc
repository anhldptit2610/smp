#include "FileBrowser.h"
#include <fstream>


// trim y from x
PATH TrimThePath(PATH y, PATH x)
{
    return std::filesystem::relative(x, y);
}

bool IsMP3File(const std::filesystem::directory_entry& fileEntry)
{
    return (fileEntry.path().extension() == ".mp3");
}

bool IsMP4File(const std::filesystem::directory_entry& fileEntry)
{
    return (fileEntry.path().extension() == ".mp4");
}

bool IsPlaylistFile(const std::filesystem::directory_entry& fileEntry)
{
    return (fileEntry.path().extension() == ".playlist");
}

void BrowsePath(FILELIST& list, std::string exePath, std::string bPath, int mode)
{
    namespace fs = std::filesystem;

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
        for (const fs::directory_entry& dirEntry : fs::recursive_directory_iterator(bPath)) {
            if ((IsMP3File(dirEntry) || IsMP4File(dirEntry)) || 
                (IsPlaylistFile(dirEntry) && (mode == OPTION_PLAYLIST_DIR || mode == OPTION_LIST_PLAYLIST))) {
                std::shared_ptr<MediaFile> mf = std::make_shared<MediaFile>(dirEntry.path());
                list.push_back(mf);
            }
        }
    }
}