#ifndef SL_GENERATOR_H
#define SL_GENERATOR_H

#include <QPixmap>
#include <QPainter>
#include "sl_options.h"
#include "sl_profile.h"

namespace sl
{

class CScreenlist
{
public:
    CScreenlist();
    ~CScreenlist();

    int Generate(const char *video_file_path,
                 const CProfile &profile,
                 const COptions& options,
                 QString& result_string,
                 IEventCallback* event_callback = nullptr);

private:
    // QPixmap* Pixmap{nullptr};
    // QPainter* Painter{nullptr};
};

}
#endif // SL_GENERATOR_H
