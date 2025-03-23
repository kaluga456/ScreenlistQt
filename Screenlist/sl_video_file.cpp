#include <QDebug>
#include <QFileInfo>
#include <QSignalSpy>
#include <QWaitCondition>
#include "sl_error.h"
#include "sl_video_file.h"

using namespace sl;

//header painter
constexpr size_t HEADER_VERTICAL_PADDING = 5; //vertical padding, px
constexpr size_t HEADER_LINES_COUNT = 4;

constexpr int SIGNAL_WAIT_TIMEOUT = 3000;
CVideoFile::CVideoFile(const char *video_file_path /*= nullptr*/) :
    MediaPlayer{nullptr},
    VideoSink{nullptr}
{
    Open(video_file_path);
}

CVideoFile::~CVideoFile()
{
    Close();
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
    qDebug() << "positionChanged" << position;
}
QString CVideoFile::GetFileName() const
{
    if(FilePath.isEmpty())
        return SAMPLE_FRAME_NAME;

    QDir relative_name(FilePath);
    return relative_name.dirName();
};
int CVideoFile::Open(const char *video_file_path)
{
    Close();
    if(nullptr == video_file_path)
    {
        Image = QImage(SAMPLE_FRAME_RES_NAME);
        SL_VERIFY(false == Image.isNull());
        return 0;
    }

    //video file
    MediaPlayer = new QMediaPlayer;
    connect(MediaPlayer, &QMediaPlayer::errorOccurred, this, &CVideoFile::errorOccurred);
    connect(MediaPlayer, &QMediaPlayer::errorChanged, this, &CVideoFile::errorChanged);
    connect(MediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &CVideoFile::mediaStatusChanged);
    connect(MediaPlayer, &QMediaPlayer::positionChanged, this, &CVideoFile::positionChanged);

    //load video file
    QUrl url(QUrl::fromLocalFile(video_file_path));
    MediaPlayer->setSource(url);

    QSignalSpy ssmp(MediaPlayer, &QMediaPlayer::mediaStatusChanged);
    SL_VERIFY(ssmp.isValid())
    MediaPlayer->setPosition(0);
    MediaPlayer->pause();
    SL_VERIFY(ssmp.wait(SIGNAL_WAIT_TIMEOUT));
    SL_VERIFY(true == MediaPlayer->hasVideo());
    SL_VERIFY(MediaPlayer->playbackState() == QMediaPlayer::PausedState);

    VideoSink = new QVideoSink;
    QSignalSpy ssvs(VideoSink, &QVideoSink::videoFrameChanged);
    SL_VERIFY(ssvs.isValid())
    MediaPlayer->setVideoSink(VideoSink);
    SL_VERIFY(ssvs.wait(SIGNAL_WAIT_TIMEOUT));

    //get video resolution
    QVideoFrame video_frame = VideoSink->videoFrame();
    FrameWidth = video_frame.width();
    FrameHeight = video_frame.height();

    FilePath = video_file_path;
    return 0;
}

void CVideoFile::Close()
{
    delete VideoSink;
    VideoSink = nullptr;

    delete MediaPlayer;
    MediaPlayer = nullptr;

}
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

    // QSignalSpy ssmp(MediaPlayer, &QMediaPlayer::positionChanged);
    // MediaPlayer->setPosition(pos);
    // SL_VERIFY(ssmp.wait(SIGNAL_WAIT_TIMEOUT));

    //NOTE: this is fast!!!
    QSignalSpy ssvs(VideoSink, &QVideoSink::videoFrameChanged);
    MediaPlayer->setPosition(pos);
    SL_VERIFY(ssvs.isValid());
    SL_VERIFY(ssvs.wait(SIGNAL_WAIT_TIMEOUT));

    //check erros
    QVideoFrame vf = VideoSink->videoFrame();
    Image = vf.toImage();
    return Image;
}

