#ifndef GENERATORTHREAD_H
#define GENERATORTHREAD_H

#include <QObject>
#include <QThread>
#include "sl_options.h"
#include "ProfileList.h"
#include "VideoItem.h"

#ifdef _DEBUG
    #define SHALLOW_PROCESSING
#endif
class CGeneratorThread : public QThread, public sl::IEventCallback
{
    Q_OBJECT

public:
    explicit CGeneratorThread(QObject *parent = nullptr);

    void Start(const PVideoItem& video, const PProfile& profile, const sl::COptions& options);
    void Stop();

signals:
    void threadNotify(int progress);
    void threadFinished(int result, const QString& result_string);

private:
    //thread context
    QString VideoFilePath;
    sl::CProfile Profile;
    sl::COptions Options;

    //thread procedure
    void run() override;
    int generate(QString& result_string);
    int shallow_proc(QString& result_string);

    //callbacks
    bool IsTerminate() const override;
    void SetProgress(int progress) override;
};

#endif // GENERATORTHREAD_H
