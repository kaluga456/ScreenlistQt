#ifndef SL_PROFILE_H
#define SL_PROFILE_H

#include <QColor>
#include <QFont>

namespace sl
{

//TODO:
enum : int
{
    HEADER_DISABLED = 0,
    HEADER_EXPANDED,
    HEADER_COMPACT
};

//Timestamp position flags
enum : int
{
    TIMESTAMP_TYPE_LEFT = 0x00000001,
    TIMESTAMP_TYPE_RIGHT = 0x00000002,
    TIMESTAMP_TYPE_CENTER = TIMESTAMP_TYPE_LEFT | TIMESTAMP_TYPE_RIGHT,
    TIMESTAMP_TYPE_BOTTOM = 0x00000004
};

//Timestamp position
enum : int
{
    TIMESTAMP_TYPE_DISABLED = 0x00000000, //don`t write Timestamp

    TIMESTAMP_TYPE_TOP_LEFT = TIMESTAMP_TYPE_LEFT,
    TIMESTAMP_TYPE_TOP_CENTER = TIMESTAMP_TYPE_CENTER,
    TIMESTAMP_TYPE_TOP_RIGHT = TIMESTAMP_TYPE_RIGHT,
    TIMESTAMP_TYPE_BOTTOM_LEFT = TIMESTAMP_TYPE_LEFT | TIMESTAMP_TYPE_BOTTOM,
    TIMESTAMP_TYPE_BOTTOM_CENTER = TIMESTAMP_TYPE_CENTER | TIMESTAMP_TYPE_BOTTOM,
    TIMESTAMP_TYPE_BOTTOM_RIGHT = TIMESTAMP_TYPE_RIGHT | TIMESTAMP_TYPE_BOTTOM,

    TIMESTAMP_TYPE_COUNT
};

class CProfile
{
public:
    CProfile();

    bool operator==(const CProfile& rop) const;

    QColor BgColor;

    //header
    int HeaderType; //HEADER_ value
    QFont HeaderFont;
    QColor HeaderFontColor;

    //frames grid
    int GridRows;
    int GridColumns;

    //px, output image ImageWidth
    int ImageWidth;

    //Timestamp
    int Timestamp; //TIMESTAMP_TYPE_ value
    QFont TimestampFont;
    QColor TimestampFontColor;
};

}

#endif // SL_PROFILE_H
