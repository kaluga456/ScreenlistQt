#ifndef PROCESSINGTHREAD_H
#define PROCESSINGTHREAD_H

#include <QObject>
#include <QThread>

class CProcessingThread : public QThread
{
    Q_OBJECT

public:
    explicit CProcessingThread(QObject *parent = nullptr);

protected:
    void run() override;

signals:
    void threadNotify(const QString& result_string);
    void threadFinished(const QString& result_string);

};

#endif // PROCESSINGTHREAD_H
