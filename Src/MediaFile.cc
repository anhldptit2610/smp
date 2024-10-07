#include "MediaFile.h"
#include "UI.h"
#include <iostream>
#include <fstream>

void timeTracker(WINDOW *win);

MediaFileManage mediaFileManage;

std::atomic<bool> killThread{false};
std::atomic<bool> isPlaying{false};
std::atomic<int> currentTime{0};

void SetPlayState(bool state) { isPlaying = state; }
void IncrementTime(void) { currentTime++; }
void ResetTime(void) { currentTime = 0; }
int GetCurrentTime(void) { return currentTime; }
void KillTimeThread(void) { killThread = true; }

void timeTracker(WINDOW *win) {
    currentTime = 0;
    while (!killThread) {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep for 1 second
        if (isPlaying && !killThread) {  // Check again to avoid counting when paused
            currentTime++;
            mvwprintw(win, 0, 12, " Time ");
            mvwprintw(win, 2, 2, "%02d:%02d/%02d:%02d", currentTime / 60, currentTime % 60, mediaFileManage.GetMin(), mediaFileManage.GetSec());
            wrefresh(win);
        }
    }
    killThread = false;
}

/********************************************************************/
/**************************** MediaFile *****************************/
/********************************************************************/

std::string MediaFile::GetPath(void) { return path; }
Metadata& MediaFile::GetMetadata(void) { return metadata; }
unsigned int MediaFile::GetMin(void) { return metadata.GetMin(); }
unsigned int MediaFile::GetSec(void) { return metadata.GetSec(); }

MediaFile::MediaFile(std::string _filePath) : metadata(_filePath) { path = _filePath; }
MediaFile::~MediaFile() { }

/********************************************************************/
/********************** MediaFileManage *****************************/
/********************************************************************/

void MediaFileManage::NextTrack(void)
{
    currentTrack = nextTrack % totalTrack;
    nextTrack = (nextTrack + 1) % totalTrack;
    SetMin(currentTrack);
    SetSec(currentTrack);
}

void MediaFileManage::DeleteTrack(int index)
{
    list.erase(list.begin() + index);
    if (totalTrack > 0)
        totalTrack--;
}

void MediaFileManage::AddNewTrack(std::string val)
{
    namespace fs = std::filesystem;
    if (fs::exists(val) && fs::is_regular_file(val)) {
        std::shared_ptr<MediaFile> ptr = std::make_shared<MediaFile>(val);
        list.push_back(ptr);
        totalTrack++;
    }
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

bool MediaFileManage::IsLooping(void) { return looping; }

void MediaFileManage::ToggleLooping(void) { looping ^= 1; }

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
unsigned int MediaFileManage::GetMin(void) { return min; }
unsigned int MediaFileManage::GetSec(void) { return sec; }
void MediaFileManage::SetMin(unsigned int index) { min = list[index]->GetMin(); }
void MediaFileManage::SetSec(unsigned int index) { sec = list[index]->GetSec(); }

/* constructor/destructor */

MediaFileManage::MediaFileManage() : 
    currentTrack(0), 
    nextTrack(0), 
    totalTrack(0), 
    min(0),
    sec(0),
    playlistPath(""),
    looping(true) { }

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


MediaFileManage& GetFileManage(void)
{
    return mediaFileManage;
}