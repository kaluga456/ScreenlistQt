#ifndef SL_INTERFACE_H
#define SL_INTERFACE_H

#include "sl_options.h"
#include "sl_profile.h"

namespace sl
{

int generate(const char* video_file_path,
             const CProfile& profile,
             const COptions& options,
             QString& result_string,
             IEventCallback* event_callback = nullptr);

}

#endif // SL_INTERFACE_H
