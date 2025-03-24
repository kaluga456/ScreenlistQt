#ifndef PROCESSINGTHREAD_H
#define PROCESSINGTHREAD_H

#include <QObject>
#include <QThread>
#include "sl_options.h"
#include "ProfileList.h"
#include "VideoItem.h"

class CProcessingThread : public QThread
{
    Q_OBJECT

public:
    explicit CProcessingThread(QObject *parent = nullptr);

    void Start(PVideoItem video, PProfile profile, const sl::COptions& options);
    void Stop();

protected:
    void run() override;

signals:
    void threadNotify(int progress);
    void threadFinished(int result, const QString& result_string);

};

#endif // PROCESSINGTHREAD_H
