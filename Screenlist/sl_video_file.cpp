#include <QDebug>
#include <QFileInfo>
#include <QSignalSpy>
#include <QWaitCondition>
#include "sl_error.h"
#include "sl_video_file.h"

using namespace sl;

//header painter
constexpr int HEADER_VERTICAL_PADDING = 5; //vertical padding, px

constexpr int SIGNAL_WAIT_TIMEOUT = 3000;
CVideoFile::CVideoFile(QString video_file_path) :
    MediaPlayer{nullptr},
    VideoSink{nullptr}
{
    //profile preview
    if(video_file_path.isEmpty())
    {
        Image = QImage(SAMPLE_FRAME_RES_NAME);
        SL_VERIFY(false == Image.isNull());
        return;
    }

    //init media player
    MediaPlayer = new QMediaPlayer;
    connect(MediaPlayer, &QMediaPlayer::errorOccurred, this, &CVideoFile::errorOccurred);
    connect(MediaPlayer, &QMediaPlayer::errorChanged, this, &CVideoFile::errorChanged);
    connect(MediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &CVideoFile::mediaStatusChanged);
    connect(MediaPlayer, &QMediaPlayer::positionChanged, this, &CVideoFile::positionChanged);

    //load video file
    QUrl url(QUrl::fromLocalFile(video_file_path));
    MediaPlayer->setSource(url);
    QSignalSpy ssmp(MediaPlayer, &QMediaPlayer::mediaStatusChanged);
    MediaPlayer->setPosition(0);
    MediaPlayer->pause();
    WaitSignal(ssmp);
    SL_VERIFY_MSG(true == MediaPlayer->hasVideo(), "Not a video file");
    SL_VERIFY_MSG(MediaPlayer->playbackState() == QMediaPlayer::PausedState, "Could not play video");

    //init video sink
    VideoSink = new QVideoSink;
    QSignalSpy ssvs(VideoSink, &QVideoSink::videoFrameChanged);
    //SL_VERIFY(ssvs.isValid())
    MediaPlayer->setVideoSink(VideoSink);
    WaitSignal(ssvs);
    //SL_VERIFY(ssvs.wait(SIGNAL_WAIT_TIMEOUT));

    //get video resolution
    QVideoFrame video_frame = VideoSink->videoFrame();
    FrameWidth = video_frame.width();
    FrameHeight = video_frame.height();

    FilePath = video_file_path;
}
CVideoFile::~CVideoFile()
{
    delete VideoSink;
    delete MediaPlayer;
}
void CVideoFile::errorOccurred(QMediaPlayer::Error error, const QString &errorString)
{
    qDebug() << "errorOccurred: " << errorString;
}
void CVideoFile::errorChanged()
{
    qDebug() << "errorChanged";
}
void CVideoFile::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qDebug() << "mediaStatusChanged: " << status;
}
void CVideoFile::positionChanged(qint64 position)
{
    //qDebug() << "positionChanged" << position;
}
void CVideoFile::WaitSignal(QSignalSpy& signal_spy)
{
    //wait
    SL_VERIFY(signal_spy.isValid());
    SL_VERIFY(signal_spy.wait(SIGNAL_WAIT_TIMEOUT));

    //check error
    if(QMediaPlayer::NoError != MediaPlayer->error())
        throw SLExcString(MediaPlayer->errorString());
}
QString CVideoFile::GetFileName() const
{
    if(FilePath.isEmpty())
        return SAMPLE_FRAME_NAME;

    QDir relative_name(FilePath);
    return relative_name.dirName();
};
QString CVideoFile::GetFilePath() const
{
    return MediaPlayer ? FilePath : SAMPLE_FRAME_NAME;
}
qint64 CVideoFile::GetFileSize() const
{
    if(nullptr == MediaPlayer)
        return SAMPLE_FRAME_SIZE; //sample value

    QFileInfo file_info{FilePath};
    return file_info.size();
}
qint64 CVideoFile::GetDuration() const
{
    return MediaPlayer ? MediaPlayer->duration() : SAMPLE_FRAME_DURATION;
}
int CVideoFile::GetFrameWidth() const
{
    return MediaPlayer ? FrameWidth : Image.width();
}
int CVideoFile::GetFrameHeight() const
{
    return MediaPlayer ? FrameHeight : Image.height();
}
const QImage& CVideoFile::GetFrameImage(qint64 pos)
{
    if(nullptr == MediaPlayer)
        return Image;

    //set position
    QSignalSpy ssvs(VideoSink, &QVideoSink::videoFrameChanged);
    MediaPlayer->setPosition(pos);
    WaitSignal(ssvs);
    //SL_VERIFY(ssvs.isValid());
    //SL_VERIFY(ssvs.wait(SIGNAL_WAIT_TIMEOUT));

    //get frame
    QVideoFrame vf = VideoSink->videoFrame();
    SL_VERIFY_MSG(vf.isValid(), "Could not get video frame");

    Image = vf.toImage();
    return Image;
}

