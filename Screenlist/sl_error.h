#ifndef SL_ERROR_H
#define SL_ERROR_H

#include <QString>

namespace sl
{
class SLExc
{
public:
    SLExc() {}
    virtual QString what() const = 0;
};
class SLExcString : public SLExc
{
public:
    SLExcString(QString msg);
    QString what() const override;

protected:
    QString text;
};

#define SL_VERIFY(expression) {if(false == static_cast<bool>(expression)) throw SLExcString("SL_VERIFY(" #expression ")");}
#define SL_VERIFY_MSG(expression, error_message) {if(false == static_cast<bool>(expression)) throw SLExcString(error_message);}

}
#endif // SL_ERROR_H
