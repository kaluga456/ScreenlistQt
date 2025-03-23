#include "sl_profile.h"

using namespace sl;

constexpr const int DEFAULT_PROFILE_FONT_SIZE = 16;
constexpr const char* DEFAULT_PROFILE_FONT_NAME = "Lucida Console";

//default profile
CProfile::CProfile() :
    BgColor{QColor(32,32,32)},
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
