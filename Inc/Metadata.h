#ifndef METADATA_H_
#define METADATA_H_

#include <string>
#include <ncurses.h>

void PrintMetadata(WINDOW *win, std::string path, int key = -1);

#endif