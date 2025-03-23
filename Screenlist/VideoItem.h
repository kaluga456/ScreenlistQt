#ifndef VIDEOITEM_H
#define VIDEOITEM_H

#include <QString>

//processing item state
enum
{
    //processing now 0...100%
    PIS_MIN_PROCESSING = 0,
    PIS_MAX_PROCESSING = 100,

    PIS_WAIT,                  //ready to process
    PIS_DONE,                  //processed successfully
    PIS_FAILED                 //processed with error
};

class CVideoItem
{
public:
    explicit CVideoItem(const char* vide_file_path);

    //TODO:
    //CVideoItem(int state, LPCTSTR source_file_name, LPCTSTR result_string = _T("")); //for loading from setting file

    int State;
    QString VideoFilePath;
    QString ResultString;
};

#endif // VIDEOITEM_H
