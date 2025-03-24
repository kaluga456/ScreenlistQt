#include <QtDebug>

#include "ProcessingThread.h"

CProcessingThread::CProcessingThread(QObject *parent) : QThread{parent}
{
}
void CProcessingThread::Start(PVideoItem video, PProfile profile, const sl::COptions &options)
{

}
void CProcessingThread::Stop()
{

}
void CProcessingThread::run()
{
    qDebug() << "CProcessingThread start: " << QThread::currentThreadId();
    for(int i = 1; i <= 10; ++i)
    {
        if(isInterruptionRequested())
        {
            qDebug() << "CProcessingThread isInterruptionRequested()";
            emit threadFinished(PIS_FAILED, "<terminated>");
            break;
        }

        QThread::msleep(500);

        emit threadNotify(i * 10);
        //qDebug() << "CProcessingThread iteration: " << i;
    }
    qDebug() << "CProcessingThread end: " << QThread::currentThreadId();

    emit threadFinished(PIS_DONE, "<result text>");
}
