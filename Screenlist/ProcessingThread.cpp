#include <QtDebug>

#include "ProcessingThread.h"

CProcessingThread::CProcessingThread(QObject *parent) : QThread{parent}
{
}

void CProcessingThread::run()
{
    qDebug() << "CProcessingThread start: " << QThread::currentThreadId();
    for(int i = 01; i <= 10; ++i)
    {
        if(isInterruptionRequested())
        {
            qDebug() << "CProcessingThread isInterruptionRequested()";
            break;
        }

        QThread::msleep(500);

        emit threadNotify(QString("<notify event>"));
        //qDebug() << "CProcessingThread iteration: " << i;
    }
    qDebug() << "CProcessingThread end: " << QThread::currentThreadId();

    emit threadFinished("<result text>");
}
