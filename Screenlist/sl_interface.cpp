#include <QtAssert>
#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QResource>
#include "sl_error.h"
#include "sl_generator.h"
#include "sl_interface.h"

using namespace sl;

int sl::generate(QString video_file_path,
                 const CProfile &profile,
                 const COptions &options,
                 QString& result_string,
                 IEventCallback* event_callback /*= nullptr*/)
{
    try
    {
        CGenerator output;

        qDebug() << "sl::generate: " << video_file_path;
        const int result = output.Generate(video_file_path, profile, options, result_string, event_callback);
        qDebug() << "sl::generate(): result = " << result << ": " << result_string;
        return result;
    }

    //TODO:
    catch (const SLExc& exc)
    {
        qDebug() << "EXCEPTION: " << exc.what();
        result_string = exc.what();
    }
    catch (...)
    {
        qDebug() << "UNKNOWN EXCEPTION";
        result_string = "UNKNOWN EXCEPTION";
    }

    return RESULT_FAILED;
}
