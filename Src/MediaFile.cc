#include "MediaFile.h"

/* getter/setter */

std::string MediaFile::GetTrackPath(int n)
{
    return list[n].path();
}

int MediaFile::GetTotalTrack(void) { return totalTrack; }

int MediaFile::GetNextTrack(void) { return nextTrack; }

int MediaFile::GetCurrentTrack(void) { return currentTrack; }

void MediaFile::SetTotalTrack(int n) { totalTrack = n; }

void MediaFile::SetNextTrack(int n) { nextTrack = n; }

void MediaFile::SetCurrentTrack(int n) { currentTrack = n; }

FILELIST& MediaFile::GetMediaList(void) { return list; }

/* constructor/destructor */

MediaFile::MediaFile() : currentTrack(0), nextTrack(0), totalTrack(0)
{

}

MediaFile::~MediaFile()
{

}

MediaFile mediaFile;

MediaFile& GetMediaFile(void)
{
    return mediaFile;
}