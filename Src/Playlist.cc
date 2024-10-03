#include "Playlist.h"
#include <iostream>
#include <fstream>

void Playlist::UpdatePath(std::string _path)
{
    path = _path;
}

FILELIST& Playlist::GetPlayList()
{
    return list;
}

std::string Playlist::GetTrackPath(int i)
{
    return list[i]->GetPath();
}

void Playlist::AddNewTrack(std::string path)
{
    std::shared_ptr<MediaFile> n = std::make_shared<MediaFile>(path); 
    list.push_back(n);
}

Playlist::Playlist()
{
}

Playlist::~Playlist()
{
    std::ofstream file(path.c_str(), std::ios::out | std::ios::trunc);

    if (file.is_open()) {
        for (auto& i : list)
            file << i->GetPath() << std::endl;
        file.close();
    }
}

