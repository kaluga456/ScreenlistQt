#ifndef SETTINGS_H
#define SETTINGS_H

#include <QHeaderView>
#include <QMainWindow>

//video file types
constexpr const char* FILE_DIALOG_FILTER = "Video Files (*.avi *.mkv *.mp4 *.m4v *.mp4v *.mpv4 *.hdmov *.mov *.3gp *.3gpp *.3g2 *.3gp2 *.flv *.f4v"
                                           " ogm *.ogv *.rm *.ram *.rpm *.rmm *.rt *.rp *.smi *.smil *.wmvwmp *.wm *.asf"
                                           " smk *.bik *.fli *.flc *.flic *.dsm *.dsv *.dsa *.dss *.ivf *.divx *.rmvb *.amv)"
                                           ";;All Files (*.*)";
//////////////////////////////////////////////////////////////////////////////
class COutputDirList
{
public:
    static constexpr int MAX_OUTPUT_DIRS = 4;

    COutputDirList();

    //access
    QStringList::const_iterator begin() const {return Dirs.cbegin();}
    QStringList::const_iterator end() const {return Dirs.cend();}

    //if dir not exist in list inserts it in 2nd position
    //returns added or existing dir index
    int Add(QString Dir);

private:
    QStringList Dirs;
};
//////////////////////////////////////////////////////////////////////////////
class CSettings
{
public:
    CSettings();

    bool OverwriteFiles{true};

    //profiles
    QString ProfileName;

    //output dirs
    COutputDirList OutputDirs;
    int LastOutputDir{0};

    QStringList VideoFilters;
};

extern CSettings Settings;

#endif // SETTINGS_H
