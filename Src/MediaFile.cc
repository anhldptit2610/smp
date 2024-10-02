#include "MediaFile.h"

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

void MediaFileManage::UpdateMetadata(int index, int key, std::string val)
{
    list[index]->GetMetadata().Update(list[index]->GetPath(), key, val);
}

/* getter/setter */

std::string MediaFileManage::GetTrackPath(int n) { return list[n]->GetPath(); }
int MediaFileManage::GetTotalTrack(void) { return totalTrack; }
int MediaFileManage::GetNextTrack(void) { return nextTrack; }
int MediaFileManage::GetCurrentTrack(void) { return currentTrack; }
void MediaFileManage::SetTotalTrack(int n) { totalTrack = n; }
void MediaFileManage::SetNextTrack(int n) { nextTrack = n; }
void MediaFileManage::SetCurrentTrack(int n) { currentTrack = n; }
FILELIST& MediaFileManage::GetMediaList(void) { return list; }

/* constructor/destructor */

MediaFileManage::MediaFileManage() : currentTrack(0), nextTrack(0), totalTrack(0)
{

}

MediaFileManage::~MediaFileManage()
{

}

MediaFileManage mediaFileManage;

MediaFileManage& GetFileManage(void)
{
    return mediaFileManage;
}