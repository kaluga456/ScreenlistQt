#ifndef VIDEOITEM_H
#define VIDEOITEM_H

#include <QString>

//video item processing state
enum
{
    //processing now 0...100%
    VIS_MIN_PROCESSING = 0,
    VIS_MAX_PROCESSING = 100,

    VIS_WAIT, //ready to process
    VIS_DONE, //processed successfully
    VIS_FAILED, //processed with error

    VIS_SELECTED //for removal from list
};

QString GetStateString(int item_state);

class CVideoItem
{
public:
    explicit CVideoItem(QString vide_file_path);

    int State;
    QString VideoFilePath;
    QString ResultString;
};
typedef std::shared_ptr<CVideoItem> PVideoItem;

#endif // VIDEOITEM_H
