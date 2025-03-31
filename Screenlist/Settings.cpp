#include <QFile>
#include <QDataStream>
#include <QCoreApplication>
#include <QSettings>
#include "Settings.h"

CSettings Settings;
constexpr const char* OUTPUT_CFG_NAME = "/output.cfg";
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
void COutputDirList::Save()
{
    QString cfg_file_name = QCoreApplication::applicationDirPath() + OUTPUT_CFG_NAME;
    QFile cfg_file(cfg_file_name);
    if(false == cfg_file.open(QIODevice::WriteOnly))
    {
        Q_ASSERT(0);
        return;
    }

    const quint32 count = Dirs.size();
    Q_ASSERT(count > 0);
    if(count < 2)
        return;

    QDataStream stream(&cfg_file);
    stream << static_cast<quint32>(count - 1);

    QStringList::const_iterator dir_i = Dirs.cbegin();
    ++dir_i;
    for(; dir_i != Dirs.cend(); ++dir_i)
        stream << *dir_i;
}
void COutputDirList::Load()
{
    QString cfg_file_name = QCoreApplication::applicationDirPath() + OUTPUT_CFG_NAME;
    QFile cfg_file(cfg_file_name);
    if(false == cfg_file.open(QIODevice::ReadOnly))
        return;

    QDataStream stream(&cfg_file);
    quint32 count{0};
    stream >> count;
    count = std::min(count, static_cast<quint32>(MAX_OUTPUT_DIRS));
    for(int row = 0; row < count; ++row)
    {
        QString output_dir;
        stream >> output_dir;
        Dirs.push_back(output_dir);
    }
}
//////////////////////////////////////////////////////////////////////////////
CSettings::CSettings()
{
    VideoFilters << "*.avi" << "*.mkv" << "*.mp4" << "*.m4v" << "*.mp4v" << "*.mpv4"
                 << "*.hdmov" << "*.mov" << "*.3gp" << "*.3gpp" << "*.3g2" << "*.3gp2"
                 << "*.flv" << "*.f4v" << "*.ogm" << "*.ogv" << "*.rm" << "*.ram"
                 << "*.rpm" << "*.rmm" << "*.rt" << "*.rp" << "*.smi" << "*.smil"
                 << "*.wm" << "*.asf" << "*.smk" << "*.bik" << "*.fli" << "*.flc"
                 << "*.flic" << "*.dsm" << "*.dsv" << "*.dsa" << "*.dss" << "*.ivf"
                 << "*.divx" << "*.rmvb" << "*.amv";
}
QString CSettings::GetFileDialogFilter() const
{
    QString result("Video Files (");
    result += VideoFilters.join(" ");
    result += ");;All Files (*.*)";
    return result;
}
//////////////////////////////////////////////////////////////////////////////
