#include "VideoItem.h"

CVideoItem::CVideoItem(const char* vide_file_path) :
    VideoFilePath(vide_file_path),
    State(PIS_WAIT)
{
}
