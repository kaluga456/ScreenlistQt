#include <QFile>
#include <QDataStream>
#include <QCoreApplication>
#include <QSettings>
#include "Settings.h"

//profiles file
// constexpr const char* PROFILES_FILE_NAME = "/profiles.dat";
// constexpr quint32 PROFILES_FILE_HEADER = 0xF103A86C;
// constexpr quint32 PROFILES_FILE_VERSION = 1;

CSettings Settings;

CSettings::CSettings()
{
}
void CSettings::Load(QMainWindow* main_window)
{
    QSettings qs;
    OverwriteFiles = qs.value("OverwriteFiles").toBool();
    main_window->restoreGeometry(qs.value("MainWindowGeometry").toByteArray());
    main_window->restoreState(qs.value("MainWindowState").toByteArray());

    ProfileName = qs.value("Profile").toString();
}
void CSettings::Save(QMainWindow* main_window)
{
    QSettings qs;
    qs.setValue("OverwriteFiles", OverwriteFiles);
    qs.setValue("MainWindowGeometry", main_window->saveGeometry());
    qs.setValue("MainWindowState", main_window->saveState());

    qs.setValue("Profile", ProfileName);

    //output dirs
    // qs.beginWriteArray("OutputDirs");
    // int index = 0;
    // for(CBestScore::const_iterator i = BestResults.beign(); i != BestResults.end(); ++i, ++index)
    // {
    //     qs.setArrayIndex(index);
    //     qs.setValue("result", i->first);
    //     qs.setValue("date", i->second);
    // }
    // sq.endArray();
}

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
