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
}
void CSettings::Save(QMainWindow* main_window)
{
    QSettings qs;
    qs.setValue("OverwriteFiles", OverwriteFiles);
    qs.setValue("MainWindowGeometry", main_window->saveGeometry());
    qs.setValue("MainWindowState", main_window->saveState());
}
// void CSettings::SaveProfiles(const CProfileModel &profiles)
// {
//     QString profiles_file_name = QCoreApplication::applicationDirPath() + PROFILES_FILE_NAME;
//     QFile profiles_file(profiles_file_name);
//     if(false == profiles_file.open(QIODevice::WriteOnly))
//     {
//         Q_ASSERT(0);
//         return;
//     }

//     QDataStream stream(&profiles_file);
//     stream << PROFILES_FILE_HEADER;
//     stream << PROFILES_FILE_VERSION;
//     stream << static_cast<quint16>(profiles.rowCount());
//     for(int row = 0; row < profiles.rowCount(); ++row)
//     {
//         PProfile profile = profiles.getProfile(row);
//         stream << profile->Name;
//         stream << profile->HeaderType;
//         stream << profile->HeaderFont;
//         stream << profile->ImageWidth;
//         stream << profile->GridColumns;
//         stream << profile->GridRows;
//         stream << profile->Timestamp;
//         stream << profile->TimestampFont;
//     }
// }
// void CSettings::LoadProfiles(CProfileModel &profiles)
// {
//     QString profiles_file_name = QCoreApplication::applicationDirPath() + PROFILES_FILE_NAME;
//     QFile profiles_file(profiles_file_name);
//     if(false == profiles_file.open(QIODevice::ReadOnly))
//     {
//         //Q_ASSERT(0);
//         return;
//     }

//     QDataStream stream(&profiles_file);
//     quint32 header;
//     stream >> header;
//     if(header != PROFILES_FILE_HEADER)
//         return;
//     quint32 version;
//     stream >> version;
//     if(header != PROFILES_FILE_VERSION)
//         return;
//     quint16 count{0};
//     stream >> count;
//     if(count > 100)
//         return;

//     profiles.Clear();
//     for(int row = 0; row < count; ++row)
//     {
//         PProfile profile;
//         stream >> profile->Name;
//         stream >> profile->HeaderType;
//         stream >> profile->HeaderFont;
//         stream >> profile->ImageWidth;
//         stream >> profile->GridColumns;
//         stream >> profile->GridRows;
//         stream >> profile->Timestamp;
//         stream >> profile->TimestampFont;
//         profiles.addProfile(profile->Name, profile);
//     }
// }
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
