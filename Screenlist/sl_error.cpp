#include "sl_error.h"

using namespace sl;

SLExcString::SLExcString(QString msg) : text(msg)
{
}
QString SLExcString::what() const
{
    return text;
}
