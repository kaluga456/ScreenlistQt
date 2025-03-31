#include "VideoItem.h"

QString GetStateString(int item_state)
{
    //TODO:
    switch(item_state)
    {
    case VIS_WAIT:
    {
        // if(PTS_NONE == ProcessingState || (IsProcessSelected && false == pi->Selected))
        //     return _T("");
        return "Wait";
    }
    case VIS_DONE: return "Done";
    case VIS_FAILED: return "Failed";
    }

    if(VIS_MIN_PROCESSING <= item_state && item_state <= VIS_MAX_PROCESSING)
        return QString().setNum(item_state) + "%";

    return "Unknown";
}

CVideoItem::CVideoItem(QString vide_file_path) :
    VideoFilePath(vide_file_path),
    State(VIS_WAIT)
{
}


