#include "VideoItem.h"

QString GetStateString(int item_state)
{
    //TODO:
    switch(item_state)
    {
    case PIS_WAIT:
    {
        // if(PTS_NONE == ProcessingState || (IsProcessSelected && false == pi->Selected))
        //     return _T("");
        return "Wait";
    }
    case PIS_DONE: return "Done";
    case PIS_FAILED: return "Failed";
    }

    if(PIS_MIN_PROCESSING <= item_state && item_state <= PIS_MAX_PROCESSING)
        return QString().setNum(item_state) + "%";

    return "Unknown";
}

CVideoItem::CVideoItem(const char* vide_file_path) :
    VideoFilePath(vide_file_path),
    State(PIS_WAIT)
{
}


