#include <QDebug>
#include <QDir>
#include <format>
#include "sl_video_file.h"
#include "sl_generator.h"

using namespace sl;

constexpr int HEADER_TEXT_PAD = 4;
constexpr int HEADER_LINES_COUNT = 4;
constexpr const char* HEADER_FORMAT_STRING = "File Name: {:s}\nFile Size: {:s}\nResolution: {:d}x{:d}\nDuration: {:s}";

//TODO: sanity check
constexpr int MIN_FRAME_SIZE = 100;
constexpr int MAX_FRAME_SIZE = 4096;
constexpr int MIN_PROFILE_GRID = 1;
constexpr int MAX_PROFILE_GRID = 10;

std::string GetDurationString(qint64 duration)
{
    constexpr const char* format_string = "{:02d}:{:02d}:{:02d}";

    const int secs = duration / 1000;
    const int seconds = secs % 60;
    const int minutes = (secs / 60) % 60;
    const int hours = secs / 3600;

    std::string result = std::format(format_string, hours, minutes, seconds);
    return result;
}
std::string GetFileSizeString(const qint64& file_size)
{
    std::string size_string;
    uint32_t size_value = 0;
    const qint64 TERABYTE = qint64(1024 * 1024) * qint64(1024 * 1024);
    if (file_size >= TERABYTE)
    {
        size_value = static_cast<uint32_t>(file_size / TERABYTE);
        size_string = std::format("{:d} TB", size_value);
    }
    if (file_size >= 1024 * 1024 * 1024)
    {
        size_value = static_cast<uint32_t>(file_size / (1024 * 1024 * 1024));
        size_string = std::format("{:d} GB", size_value);
    }
    else if (file_size >= 1024 * 1024)
    {
        size_value = static_cast<uint32_t>(file_size / (1024 * 1024));
        size_string = std::format("{:d} MB", size_value);
    }
    else if (file_size >= 1024)
    {
        size_value = static_cast<uint32_t>(file_size / 1024);
        size_string = std::format("{:d} KB", size_value);
    }
    else
    {
        size_value = static_cast<uint32_t>(file_size);
        size_string = std::format("{:d} B", size_value);
    }

    //TODO: check big int
    std::string bytes_string;
    if (file_size)
        bytes_string = std::format(" ({:d} bytes)", file_size);
    else
        bytes_string = std::format(" ({:d} bytes)", file_size);

    return size_string + bytes_string;
}

//convert TIMESTAMP_TYPE_ to QTextOption
static Qt::AlignmentFlag GetTimastampAlignment(int Timestamp)
{
    int result = 0;

    //horz alignment
    if(Timestamp & TIMESTAMP_TYPE_CENTER)
        result |= Qt::AlignHCenter;
    else if(Timestamp & TIMESTAMP_TYPE_LEFT)
        result |= Qt::AlignLeft;
    else
        result |= Qt::AlignRight;

    //vert alignment
    if(Timestamp & TIMESTAMP_TYPE_BOTTOM)
        result |= Qt::AlignBottom;
    else
        result |= Qt::AlignTop;

    return static_cast<Qt::AlignmentFlag>(result);
}

QString GetOutputFilePath(const char *video_file_path, const COptions& options)
{
    QString output_path = options.OutputPath;
    if(output_path.back() != '\\' && output_path.back() != '/')
        output_path.push_back('/');

    if(nullptr == video_file_path)
        return output_path + PREVIEW_FILE_NAME;

    if(options.OutputPath.isEmpty())
        return QString(video_file_path) + ".jpg";

    QDir in_path(video_file_path);
    QString file_name{in_path.dirName() + ".jpg"};
    return output_path + file_name;
}

CScreenlist::CScreenlist()// : Pixmap{nullptr}, Painter{nullptr}
{
}
CScreenlist::~CScreenlist()
{
    // delete Painter;
    // delete Pixmap;
}
int CScreenlist::Generate(const char *video_file_path,
                          const CProfile &profile,
                          const COptions& options,
                          QString& result_string,
                          IEventCallback* event_callback /*= nullptr*/)
{
    Q_ASSERT(MIN_PROFILE_GRID <= profile.GridColumns && profile.GridColumns <= MAX_PROFILE_GRID);
    Q_ASSERT(MIN_PROFILE_GRID <= profile.GridRows && profile.GridRows <= MAX_PROFILE_GRID);

    if(event_callback)
        event_callback->SetProgress(0);

    //output file path
    QString output_file_path = GetOutputFilePath(video_file_path, options);
    Q_ASSERT(false == output_file_path.isEmpty());

    //check output file
    if(false == options.OverwriteFiles)
    {
        QDir dir;
        if(dir.exists(output_file_path))
        {
            result_string = "Outut file exists: " + output_file_path;
            return RESULT_FAILED;
        }
    }

    //init video file
    CVideoFile video_file;
    if(0 != video_file.Open(video_file_path))
        return RESULT_FAILED;

    //video file attributes
    const qint64 video_file_size = video_file.GetFileSize(); //bytes
    const qint64 duration = video_file.GetDuration(); //ms
    const int frame_width = video_file.GetFrameWidth(); //px
    const int frame_height = video_file.GetFrameHeight(); //px

    Q_ASSERT(MIN_FRAME_SIZE <= frame_width && frame_width <= MAX_FRAME_SIZE);
    Q_ASSERT(MIN_FRAME_SIZE <= frame_height && frame_height <= MAX_FRAME_SIZE);

    //calculate header height
    int header_height = 0;
    if(profile.HeaderType)
    {
        const int line_height = static_cast<int>(profile.HeaderFont.pointSize() * 1.2);
        header_height = HEADER_TEXT_PAD + HEADER_LINES_COUNT * line_height + 3*HEADER_TEXT_PAD;
    }

    //grid
    const float aspect_ration = static_cast<float>(frame_width) / static_cast<float>(frame_height);
    const int frame_count = profile.GridRows * profile.GridColumns;
    const int out_frame_width = static_cast<float>(profile.ImageWidth) / profile.GridColumns;
    const int out_frame_height = out_frame_width / aspect_ration;
    const int output_height = header_height + out_frame_height * profile.GridRows;

    //TODO:
    std::unique_ptr<QPixmap> Pixmap(new QPixmap{profile.ImageWidth, output_height});
    std::unique_ptr<QPainter> Painter(new QPainter{Pixmap.get()});

    //draw background
    QBrush bg_brush{profile.BgColor};
    Painter->setBackground(bg_brush);
    Painter->setBackgroundMode(Qt::OpaqueMode);
    Painter->fillRect(Pixmap->rect(), bg_brush);

    //draw header
    if(profile.HeaderType)
    {
        std::string header_text = std::format(HEADER_FORMAT_STRING,
                                              video_file.GetFileName().toLatin1().data(),
                                              GetFileSizeString(video_file_size),
                                              frame_width, frame_height,
                                              GetDurationString(duration));
        QTextOption text_options;
        text_options.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        text_options.setWrapMode(QTextOption::NoWrap);
        Painter->setFont(profile.HeaderFont);
        Painter->setPen(QPen{QColor(profile.HeaderFontColor)});
        Painter->drawText(QRectF{HEADER_TEXT_PAD, HEADER_TEXT_PAD,
                                 static_cast<qreal>(profile.ImageWidth),
                                 static_cast<qreal>(header_height)},
                                 header_text.c_str(), text_options);
    }


    Painter->setBackgroundMode(Qt::TransparentMode);
    const qint64 interval = duration / (frame_count + 1);
    qint64 current_position = interval;
    int frame_index = 0;
    for (int y = 0; y < profile.GridRows; ++y)
    {
        for (int x = 0; x < profile.GridColumns; ++x)
        {
            //
            if(event_callback && event_callback->IsTerminate())
                return RESULT_TERMINATED;

            //draw frame image
            const int frame_left = x * out_frame_width;
            const int frame_top = header_height + y * out_frame_height;

            const QImage& frame = video_file.GetFrameImage(current_position);
            Q_ASSERT(false == frame.isNull());
            const QRect frame_rect{frame_left, frame_top, out_frame_width, out_frame_height};
            Painter->drawImage(frame_rect, frame);

            //draw imestamp
            if(TIMESTAMP_TYPE_DISABLED != profile.Timestamp)
            {
                std::string timestamp_text = GetDurationString(current_position);
                QTextOption timestamp_text_options;
                timestamp_text_options.setAlignment(GetTimastampAlignment(profile.Timestamp));
                timestamp_text_options.setWrapMode(QTextOption::NoWrap);
                Painter->setFont(profile.TimestampFont);

                //draw shadow
                constexpr int TIMESTAMP_SHADOW_SHIFT = 2;
                const QColor shadow_color{0,0,0};
                Painter->setPen(QPen{QColor(shadow_color)});
                Painter->drawText(QRect{frame_rect.left() + TIMESTAMP_SHADOW_SHIFT,
                                        frame_rect.top() + TIMESTAMP_SHADOW_SHIFT,
                                        frame_rect.width(),
                                        frame_rect.height()},
                                        timestamp_text.c_str(), timestamp_text_options);

                //draw Timestamp
                Painter->setPen(QPen{QColor(profile.TimestampFontColor)});
                Painter->drawText(QRect{frame_rect.left(),
                                        frame_rect.top(),
                                        frame_rect.width(),
                                        frame_rect.height()},
                                        timestamp_text.c_str(), timestamp_text_options);
            }

            if(event_callback)
                event_callback->SetProgress(100.f * (frame_count - frame_index) / frame_count);

            //go to next frame
            current_position += interval;
            ++frame_index;
        }
    }

    //TODO: set image quality and format
    //write output file
    if(false == Pixmap->save(output_file_path))
    {
        result_string = "Unable to save file: " + output_file_path;
        return RESULT_FAILED;
    }


    if(event_callback)
        event_callback->SetProgress(100);

    result_string = output_file_path;
    return RESULT_SUCCESS;
}
