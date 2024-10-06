#ifndef FILEBROWSER_H_
#define FILEBROWSER_H_

#include "Common.h"
#include "MediaFile.h"
#include <filesystem>
#include <vector>

PATH TrimThePath(PATH y, PATH x);
void BrowsePath(FILELIST& list, std::string exePath, std::string bPath, int mode);

#endif