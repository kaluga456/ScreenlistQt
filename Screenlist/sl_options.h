#ifndef SL_OPTIONS_H
#define SL_OPTIONS_H

#include <QString>

namespace sl
{

//generator results
enum : int
{
    RESULT_SUCCESS = 0, //result string contains output image file path
    RESULT_FAILED,      //result string contains error description
    RESULT_TERMINATED   //result string not used
};

//callback target interface
class IEventCallback
{
public:
    virtual bool IsTerminate() const = 0;
    virtual void SetProgress(int progress) = 0;
};

class COptions
{
public:
    COptions();

    QString OutputPath; //if empty write screenlist to video file directory
    bool OverwriteFiles;
};

}
#endif // SL_OPTIONS_H
