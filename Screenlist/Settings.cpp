#include <QFile>
#include <QDataStream>
#include <QCoreApplication>
#include <QSettings>
#include "Settings.h"

CSettings Settings;
//////////////////////////////////////////////////////////////////////////////
COutputDirList::COutputDirList()
{
    Dirs.push_back("<Use video file directory for output>");
}
int COutputDirList::Add(QString Dir)
{
    if(Dir.isEmpty())
        return -1;

    //return existing dir index
    const int existing = Dirs.indexOf(Dir, Qt::CaseInsensitive);
    if(existing > 0)
        return existing;

    //insert in 2nd position
    QStringList::const_iterator pos = Dirs.cbegin();
    ++pos;
    if(pos == Dirs.end())
    {
        Dirs.push_back(Dir);
        return 1;
    }
    Dirs.insert(pos, Dir);
    if(Dirs.size() > MAX_OUTPUT_DIRS)
        Dirs.pop_back();
    return 1;
}
//////////////////////////////////////////////////////////////////////////////
CSettings::CSettings()
{
    //TODO:
    VideoFilters << "*.avi" << "*.mp4";
}
