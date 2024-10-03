#include "MediaFile.h"
#include <iostream>
#include <fstream>

/********************************************************************/
/**************************** MediaFile *****************************/
/********************************************************************/

std::string MediaFile::GetPath(void) { return path; }

Metadata& MediaFile::GetMetadata(void) { return metadata; }

MediaFile::MediaFile(std::string _filePath) : metadata(_filePath) { path = _filePath; }

MediaFile::~MediaFile()
{

}

/********************************************************************/
/********************** MediaFileManage *****************************/
/********************************************************************/


void MediaFileManage::DeleteTrack(int index)
{
    list.erase(list.begin() + index);
    totalTrack--;
}

void MediaFileManage::AddNewTrack(std::string val)
{
    std::shared_ptr<MediaFile> ptr = std::make_shared<MediaFile>(val);
    list.push_back(ptr);
    totalTrack++;
}

void MediaFileManage::ParsePlaylist(std::string playlist)
{
    list.clear();
    std::ifstream f(playlist.c_str());
    if (f.is_open()) {
        std::string line;
        while (std::getline(f, line)) {
            std::shared_ptr<MediaFile> mf = std::make_shared<MediaFile>(line);
            list.push_back(mf);
        }
        f.close();
    }
    totalTrack = list.size();
}

void MediaFileManage::UpdateMetadata(int index, int key, std::string val)
{
    list[index]->GetMetadata().Update(list[index]->GetPath(), key, val);
}

void MediaFileManage::CreateNewPlaylist(std::string playlist)
{
    std::ofstream outfile(playlist.c_str());

    outfile.close();
}

/* getter/setter */

std::string MediaFileManage::GetTrackPath(int n) { return list[n]->GetPath(); }
int MediaFileManage::GetTotalTrack(void) { return totalTrack; }
int MediaFileManage::GetNextTrack(void) { return nextTrack; }
int MediaFileManage::GetCurrentTrack(void) { return currentTrack; }
void MediaFileManage::SetTotalTrack(int n) { totalTrack = n; }
void MediaFileManage::SetNextTrack(int n) { nextTrack = n; }
void MediaFileManage::SetCurrentTrack(int n) { currentTrack = n; }
void MediaFileManage::SetPlaylistPath(std::string path) { playlistPath = path; }
FILELIST& MediaFileManage::GetMediaList(void) { return list; }

/* constructor/destructor */

MediaFileManage::MediaFileManage() : currentTrack(0), nextTrack(0), totalTrack(0), playlistPath("")
{

}

MediaFileManage::~MediaFileManage()
{
    if (playlistPath != "") {
        std::ofstream file(playlistPath, std::ios::out | std::ios::trunc);
        if (file.is_open()) {
            for (auto& i : list)
                file << i->GetPath() << std::endl;
            file.close();
        }
    }
}

MediaFileManage mediaFileManage;

MediaFileManage& GetFileManage(void)
{
    return mediaFileManage;
}