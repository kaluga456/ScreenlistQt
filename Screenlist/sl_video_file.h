#ifndef SL_VIDEO_FILE_H
#define SL_VIDEO_FILE_H

#include <QSignalSpy>
#include <QVideoFrame>
#include <QVideoSink>
#include <QMediaPlayer>

namespace sl
{

//sample values for profile preview
constexpr const char* SAMPLE_FRAME_RES_NAME = ":/res/sample_frame.jpg";
constexpr const char* SAMPLE_FRAME_NAME = "sample_frame.bmp";
constexpr int SAMPLE_FRAME_SIZE = 230456;
constexpr int SAMPLE_FRAME_DURATION = 0;
constexpr int SAMPLE_FRAME_WIDTH = 320;
constexpr int SAMPLE_FRAME_HEIGHT = 200;
//////////////////////////////////////////////////////////////////////////////
class CVideoFile : public QObject
{
    Q_OBJECT

public:
    //ctor/dtor
    CVideoFile(QString video_file_path);
    ~CVideoFile();

    //access
    QString GetFilePath() const;
    QString GetFileName() const;
    qint64 GetFileSize() const; //bytes
    qint64 GetDuration() const; //ms
    int GetFrameWidth() const; //px
    int GetFrameHeight() const; //px

    //operation
    const QImage& GetFrameImage(qint64 pos);

private slots:
    void errorChanged();
    void errorOccurred(QMediaPlayer::Error error, const QString &errorString);
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void positionChanged(qint64 position);

private:
    //video attributes
    QString FilePath;
    int FrameWidth{0};
    int FrameHeight{0};

    //current frame
    QImage Image;

    //error
    bool HasError{false};
    void WaitSignal(QSignalSpy& signal_spy);

    //engine
    QMediaPlayer* MediaPlayer{nullptr}; //null for sample screenlist
    QVideoSink* VideoSink{nullptr};
};

}
#endif // SL_VIDEO_FILE_H
