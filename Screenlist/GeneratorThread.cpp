#include <QtDebug>

#include "sl_interface.h"
#include "GeneratorThread.h"

CGeneratorThread::CGeneratorThread(QObject *parent) : QThread{parent}
{
}
void CGeneratorThread::Start(const PVideoItem& video, const PProfile& profile, const sl::COptions &options)
{
    Stop();

    //thread context
    VideoFilePath = video->VideoFilePath;
    Profile.HeaderType = profile->HeaderType;
    Profile.HeaderFont = profile->HeaderFont;
    Profile.HeaderFontColor = profile->HeaderFontColor;
    Profile.GridRows = profile->GridRows;
    Profile.GridColumns = profile->GridColumns;
    Profile.ImageWidth = profile->ImageWidth;
    Profile.Timestamp = profile->Timestamp;
    Profile.TimestampFont = profile->TimestampFont;
    Options = options;

    //TODO:
    start();
}
void CGeneratorThread::Stop()
{
    requestInterruption();
    wait();
}
void CGeneratorThread::run()
{
    qDebug() << "CGeneratorThread start: " << QThread::currentThreadId();

    int result = PIS_FAILED;
    QString result_string;

#ifndef SHALLOW_PROCESSING
    result = generate(result_string);
#else
    result = shallow_proc(result_string);
#endif //SHALLOW_PROCESSING

    emit threadFinished(result, "<result text>");

    qDebug() << "CGeneratorThread end: " << QThread::currentThreadId();
}

int CGeneratorThread::generate(QString& result_string)
{
    //TODO:
    const char* video_file_path = VideoFilePath.toLatin1().data();
    return sl::generate(video_file_path, Profile, Options, result_string, this);
}

int CGeneratorThread::shallow_proc(QString& result_string)
{
    for(int i = 1; i <= 10; ++i)
    {
        if(IsTerminate())
        {
            qDebug() << "CGeneratorThread isInterruptionRequested()";
            result_string = "<shallow_proc() terminated>";
            return PIS_FAILED;
        }

        QThread::msleep(500);
        SetProgress(i * 10);
    }

    result_string = "<shallow_proc() success>";
    return PIS_DONE;
}

bool CGeneratorThread::IsTerminate() const
{
    return isInterruptionRequested();
}

void CGeneratorThread::SetProgress(int progress)
{
    emit threadNotify(progress);
}
