#ifndef SL_GENERATOR_H
#define SL_GENERATOR_H

#include <QPixmap>
#include <QPainter>
#include "sl_options.h"
#include "sl_profile.h"

namespace sl
{

class CGenerator
{
public:
    CGenerator();
    ~CGenerator();

    int Generate(QString video_file_path,
                 const CProfile &profile,
                 const COptions& options,
                 QString& result_string,
                 IEventCallback* event_callback = nullptr);
};

}
#endif // SL_GENERATOR_H
