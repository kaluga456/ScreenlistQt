#include <QDebug>
#include <QDir>
#include <format>
#include "sl_error.h"
#include "sl_video_file.h"
#include "sl_generator.h"

using namespace sl;

constexpr const char* HEADER_FORMAT_STRING = "File Name: {:s}\nFile Size: {:s}\nResolution: {:d}x{:d}\nDuration: {:s}";
constexpr const char* HEADER_FORMAT_COMPACT_STRING = "File Name: {:s}\nFile Info: {:s} | {:d}x{:d} | {:s}";

//TODO: sanity check
constexpr int MIN_FRAME_SIZE = 100;
constexpr int MAX_FRAME_SIZE = 4096;
constexpr int MIN_PROFILE_GRID = 1;
constexpr int MAX_PROFILE_GRID = 10;
//////////////////////////////////////////////////////////////////////////////
std::string GetDurationString(qint64 duration)
{
    constexpr const char* format_string = "{:02d}:{:02d}:{:02d}";
    const int secs = duration / 1000;
    const int seconds = secs % 60;
    const int minutes = (secs / 60) % 60;
    const int hours = secs / 3600;
    return std::format(format_string, hours, minutes, seconds);
}
std::string GetFileSizeString(const qint64& file_size)
{
    //common sizes
    enum : qint64
    {
        KILOBYTE = 1024ll,
        MEGABYTE = KILOBYTE * KILOBYTE,
        GIGABYTE = MEGABYTE * KILOBYTE,
        TERABYTE = GIGABYTE * KILOBYTE
    };

    //human readable size
    std::string size_string;
    uint32_t size_value = 0;
    if (file_size >= TERABYTE)
    {
        size_value = static_cast<uint32_t>(file_size / TERABYTE);
        size_string = std::format("{:d} TB", size_value);
    }
    else if (file_size >= GIGABYTE)
    {
        size_value = static_cast<uint32_t>(file_size / GIGABYTE);
        size_string = std::format("{:d} GB", size_value);
    }
    else if (file_size >= MEGABYTE)
    {
        size_value = static_cast<uint32_t>(file_size / MEGABYTE);
        size_string = std::format("{:d} MB", size_value);
    }
    else if (file_size >= KILOBYTE)
    {
        size_value = static_cast<uint32_t>(file_size / KILOBYTE);
        size_string = std::format("{:d} KB", size_value);
    }
    else
    {
        size_value = static_cast<uint32_t>(file_size);
        size_string = std::format("{:d} B", size_value);
    }

    //size in bytes
    std::string bytes_string;
    if (file_size)
        bytes_string = std::format(" ({:d} bytes)", file_size);
    else
        bytes_string = std::format(" ({:d} bytes)", file_size);

    return size_string + bytes_string;
}

//convert TIMESTAMP_TYPE_ to QTextOption
static Qt::Alignment GetTimastampAlignment(int timestamp)
{
    switch(timestamp)
    {
    case TIMESTAMP_TYPE_TOP_LEFT: return Qt::AlignTop | Qt::AlignLeft;
    case TIMESTAMP_TYPE_TOP_CENTER: return Qt::AlignTop | Qt::AlignHCenter;
    case TIMESTAMP_TYPE_TOP_RIGHT: return Qt::AlignTop | Qt::AlignRight;
    case TIMESTAMP_TYPE_BOTTOM_LEFT: return Qt::AlignBottom | Qt::AlignLeft;
    case TIMESTAMP_TYPE_BOTTOM_CENTER: return Qt::AlignBottom | Qt::AlignHCenter;
    case TIMESTAMP_TYPE_BOTTOM_RIGHT: return Qt::AlignBottom | Qt::AlignRight;
    }
    return Qt::Alignment();
}
QString GetOutputFilePath(QString video_file_path, const COptions& options)
{
    QString output_path = options.OutputPath;
    if(false == output_path.isEmpty() && output_path.back() != '\\' && output_path.back() != '/')
        output_path.push_back('/');

    if(video_file_path.isEmpty())
        return output_path + PREVIEW_FILE_NAME;

    if(options.OutputPath.isEmpty())
        return QString(video_file_path) + ".jpg";

    QDir in_path(video_file_path);
    QString file_name{in_path.dirName() + ".jpg"};
    return output_path + file_name;
}
//////////////////////////////////////////////////////////////////////////////
CGenerator::CGenerator()
{
}
CGenerator::~CGenerator()
{
}
int CGenerator::Generate(QString video_file_path,
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
    CVideoFile video_file(video_file_path);

    //video file attributes
    const qint64 video_file_size = video_file.GetFileSize(); //bytes
    const qint64 duration = video_file.GetDuration(); //ms
    const int frame_width = video_file.GetFrameWidth(); //px
    const int frame_height = video_file.GetFrameHeight(); //px

    Q_ASSERT(MIN_FRAME_SIZE <= frame_width && frame_width <= MAX_FRAME_SIZE);
    Q_ASSERT(MIN_FRAME_SIZE <= frame_height && frame_height <= MAX_FRAME_SIZE);

    //calculate header height
    int header_height = 0;
    const int text_padding = profile.ImageWidth * 0.005;
    if(profile.HeaderType)
    {
        std::unique_ptr<QPixmap> pixmap(new QPixmap{profile.ImageWidth, 100});
        std::unique_ptr<QPainter> painter(new QPainter{pixmap.get()});
        QRect rect;
        QRect bounds;
        QString sample_text((profile.HeaderType == HEADER_COMPACT) ? "line1\nline2" : "line1\nline2\nline3\nline4");
        painter->drawText(rect, 0, sample_text, &bounds);
        header_height = text_padding + bounds.height() + text_padding;
    }

    //grid
    const float aspect_ration = static_cast<float>(frame_width) / static_cast<float>(frame_height);
    const int out_frame_width = static_cast<float>(profile.ImageWidth) / profile.GridColumns;
    const int out_frame_height = out_frame_width / aspect_ration;
    const int output_height = header_height + out_frame_height * profile.GridRows;

    //graphics
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
        std::string header_text;
        if(profile.HeaderType == HEADER_COMPACT)
        {
            header_text = std::format(HEADER_FORMAT_COMPACT_STRING,
                                      video_file.GetFileName().toLatin1().data(),
                                      GetFileSizeString(video_file_size),
                                      frame_width, frame_height,
                                      GetDurationString(duration));
        }
        else
        {
            header_text = std::format(HEADER_FORMAT_STRING,
                                      video_file.GetFileName().toLatin1().data(),
                                      GetFileSizeString(video_file_size),
                                      frame_width, frame_height,
                                      GetDurationString(duration));
        }

        QTextOption text_options;
        text_options.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        text_options.setWrapMode(QTextOption::NoWrap);
        Painter->setFont(profile.HeaderFont);
        Painter->setPen(QPen{QColor(profile.HeaderFontColor)});
        Painter->drawText(QRect{text_padding, 0,
                                profile.ImageWidth,
                                header_height},
                                header_text.c_str(), text_options);
    }

    //write frames grid
    const int frame_count = profile.GridRows * profile.GridColumns;
    const qint64 interval = duration / (frame_count + 1);
    qint64 current_position = interval;
    int frame_index = 0;
    const Qt::Alignment timestamp_alignment = GetTimastampAlignment(profile.Timestamp);
    Painter->setBackgroundMode(Qt::TransparentMode);
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
                timestamp_text_options.setAlignment(timestamp_alignment);
                timestamp_text_options.setWrapMode(QTextOption::NoWrap);
                Painter->setFont(profile.TimestampFont);

                //draw shadow
                constexpr int TIMESTAMP_SHADOW_SHIFT = 2;
                const QColor shadow_color{0,0,0};
                QRect timestamp_rect = frame_rect.adjusted(text_padding, text_padding, -text_padding, -text_padding);
                Painter->setPen(QPen{QColor(shadow_color)});
                Painter->drawText(QRect{timestamp_rect.left() + TIMESTAMP_SHADOW_SHIFT,
                                        timestamp_rect.top() + TIMESTAMP_SHADOW_SHIFT,
                                        timestamp_rect.width(),
                                        timestamp_rect.height()},
                                        timestamp_text.c_str(), timestamp_text_options);

                //draw Timestamp
                Painter->setPen(QPen{QColor(profile.TimestampFontColor)});
                Painter->drawText(QRect{timestamp_rect.left(),
                                        timestamp_rect.top(),
                                        timestamp_rect.width(),
                                        timestamp_rect.height()},
                                        timestamp_text.c_str(), timestamp_text_options);
            }

            if(event_callback)
                event_callback->SetProgress(100.f * frame_index / frame_count);

            //go to next frame
            current_position += interval;
            ++frame_index;
        }
    }

    //TODO: set image quality and format
    //write output file
    if(false == Pixmap->save(output_file_path))
    {
        result_string = "Could not save file: " + output_file_path;
        return RESULT_FAILED;
    }

    if(event_callback)
        event_callback->SetProgress(100);

    result_string = output_file_path;
    return RESULT_SUCCESS;
}
