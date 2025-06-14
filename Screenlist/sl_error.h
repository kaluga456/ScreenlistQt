#ifndef SL_ERROR_H
#define SL_ERROR_H

#include <source_location>
#include <QString>

namespace sl
{

class SLExc
{
public:
    explicit SLExc(QString msg, const std::source_location* sl = nullptr);
    QString what() const;
    QString message() const {return Message;}
    QString location() const {return Location;}

protected:
    QString Message;
    QString Location;
};

//throw error message without source file location
void Verify(bool condition, QString msg);

//throw error message with source file location
void VerifyLoc(bool condition, QString msg, const std::source_location& sloc = std::source_location::current());
#define SL_VERIFY(expression) VerifyLoc(static_cast<bool>(expression), #expression);

}

#endif // SL_ERROR_H
