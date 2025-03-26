#include "sl_profile.h"

using namespace sl;

constexpr const int DEFAULT_PROFILE_FONT_SIZE = 10;
constexpr const char* DEFAULT_PROFILE_FONT_NAME = "Times";

//default profile
CProfile::CProfile() :
    BgColor{QColor(0,0,0)},
    HeaderType{true},
    HeaderFontColor{QColor(255,255,255)},
    HeaderFont{QFont{DEFAULT_PROFILE_FONT_NAME, DEFAULT_PROFILE_FONT_SIZE}},
    GridRows{4},
    GridColumns{4},
    ImageWidth{1200},
    Timestamp{TIMESTAMP_TYPE_BOTTOM_RIGHT},
    TimestampFontColor{QColor(255,255,255)},
    TimestampFont{QFont{DEFAULT_PROFILE_FONT_NAME, DEFAULT_PROFILE_FONT_SIZE}}
{
}

bool CProfile::operator==(const CProfile &rop) const
{
    if(BgColor != rop.BgColor) return false;
    if(HeaderType != rop.HeaderType) return false;
    if(HeaderFont != rop.HeaderFont) return false;
    if(HeaderFontColor != rop.HeaderFontColor) return false;
    if(GridRows != rop.GridRows) return false;
    if(GridColumns != rop.GridColumns) return false;
    if(ImageWidth != rop.ImageWidth) return false;
    if(Timestamp != rop.Timestamp) return false;
    if(TimestampFont != rop.TimestampFont) return false;
    if(TimestampFontColor != rop.TimestampFontColor) return false;
    return true;
}
