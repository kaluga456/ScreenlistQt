#include "VideoItem.h"

CProcessingItem::CProcessingItem(const char* vide_file_path) :
    VideoFilePath(vide_file_path),
    State(PIS_WAIT)
{
}
