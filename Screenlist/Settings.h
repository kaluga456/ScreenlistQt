#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QHeaderView>
#include <QMainWindow>
//////////////////////////////////////////////////////////////////////////////
template<typename T>
class CUpdateModel
{
public:
    CUpdateModel(T* model) : Model(model) {emit Model->layoutAboutToBeChanged();}
    ~CUpdateModel() {emit Model->layoutChanged();}
private:
    T* Model;
};
//////////////////////////////////////////////////////////////////////////////
class COutputDirList
{
public:
    static constexpr int MAX_OUTPUT_DIRS = 10;

    COutputDirList();

    //access
    QStringList::const_iterator begin() const {return Dirs.cbegin();}
    QStringList::const_iterator end() const {return Dirs.cend();}

    //if dir not exist in list inserts it in 2nd position
    //returns added or existing dir index
    int Add(QString Dir);

    //serialization
    void Save();
    void Load();

private:
    QStringList Dirs;
};
//////////////////////////////////////////////////////////////////////////////
class CSettings
{
public:
    CSettings();

    bool OverwriteFiles{true};
    bool ShowFullPath{true};

    //profiles
    QString ProfileName;

    //output dirs
    COutputDirList OutputDirs;
    int LastOutputDir{0};

    QStringList VideoFilters;
    //TODO: QStringList::join()

    QString GetFileDialogFilter() const;
};
//////////////////////////////////////////////////////////////////////////////
extern CSettings Settings;

#endif // SETTINGS_H
