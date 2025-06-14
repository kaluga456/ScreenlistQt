#include "sl_error.h"

using namespace sl;

SLExc::SLExc(QString msg, const std::source_location* sl /*= nullptr*/) : Message(msg)
{
    if (nullptr == sl)
        return;

    Location += sl->file_name();
    Location += '(';
    Location += std::to_string(sl->line());
    Location += ':';
    Location += std::to_string(sl->column());
    Location += "): ";
}
QString SLExc::what() const
{
    return Location.isEmpty() ? Message : Location + ": " + Message;
}

void sl::Verify(bool condition, QString msg)
{
    if(false == condition)
        throw SLExc(msg);
}
void sl::VerifyLoc(bool condition, QString msg, const std::source_location& sloc /*= std::source_location::current()*/)
{
    if(false == condition)
        throw SLExc(msg, &sloc);
}



