#include <QFile>
#include <QDataStream>
#include <QCoreApplication>
#include "Settings.h"
#include "ProfileList.h"

//profiles file
constexpr const char* PROFILES_FILE_NAME = "/profiles.cfg";
constexpr quint32 PROFILES_FILE_HEADER = 0xF103A86C;
constexpr quint32 PROFILES_FILE_VERSION = 1;
constexpr quint32 MAX_PROFILES = 50;
//////////////////////////////////////////////////////////////////////////////
//CStrIntModel
CStrIntModel::CStrIntModel() : QStringListModel(), DefaultRow{0}
{
}
CStrIntModel::~CStrIntModel()
{
}
int CStrIntModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}
int CStrIntModel::rowCount(const QModelIndex &parent) const
{
    return Data.size();
}
QVariant CStrIntModel::data(const QModelIndex &index, int role) const
{
    if(index.row() >= Data.size())
        return QVariant();
    if(Qt::DisplayRole == role)
        return Data[index.row()].Text;
    return QVariant();
}
void CStrIntModel::setDefaultRow(int def_row)
{
    Q_ASSERT(def_row < Data.size());
    DefaultRow = def_row;
}
int CStrIntModel::getData(int row) const
{
    if(row >= Data.size())
        return Data[DefaultRow].Data;
    return Data[row].Data;
}
int CStrIntModel::getRow(int data) const
{
    int row = 0;
    for(const auto& item : Data)
    {
        if(item.Data == data)
            return row;
        ++row;
    }
    return DefaultRow;
}
void CStrIntModel::addItem(QString text, int data)
{
    Data.push_back(CComboBoxItem(text, data));
}
//////////////////////////////////////////////////////////////////////////////
//CProfileModel
CProfileModel::CProfileModel() : CurrentRow{-1}
{
}
CProfileModel::~CProfileModel()
{   
}
int CProfileModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}
int CProfileModel::rowCount(const QModelIndex &parent) const
{
    return Profiles.size();
}
QVariant CProfileModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < Profiles.size() && Qt::DisplayRole == role)
        return Profiles[index.row()]->Name;
    return QVariant();
}
void CProfileModel::setProfile(int row, PProfile profile)
{
    Q_ASSERT(row < Profiles.size() && profile);
    if(row >= Profiles.size() || nullptr == profile)
        return;

    Profiles[row] = profile;
}
int CProfileModel::addProfile(QString name, PProfile profile)
{
    //TODO: ignore duplicates
    profile->Name = name;
    Profiles.push_back(profile);
    return Profiles.size() - 1;
}
void CProfileModel::deleteProfile(int row)
{
    Q_ASSERT(row < Profiles.size());
    if(row >= Profiles.size())
        return;

    Profiles.erase(Profiles.begin() + row);
    CurrentRow = -1;
}
void CProfileModel::SetCurrentRow(int row)
{
    if(row < Profiles.size())
    {
        CurrentRow = row;
        return;
    }
    CurrentRow = -1;
}
PProfile CProfileModel::getProfile(int row) const
{
    Q_ASSERT(row < Profiles.size());
    if(row >= Profiles.size())
        return PProfile();
    return Profiles[row];
}
PProfile CProfileModel::getProfile(QString name) const
{
    for(auto& profile : Profiles)
    {
        if(0 == name.compare(profile->Name, Qt::CaseInsensitive))
            return profile;
    }
    return PProfile();
}
int CProfileModel::getProfileRow(QString name) const
{
    for(int row = 0; row < Profiles.size(); ++row)
    {
        PProfile profile = Profiles[row];
        if(0 == name.compare(profile->Name, Qt::CaseInsensitive))
            return row;
    }
    return -1;
}
PProfile CProfileModel::getCurrentProfile() const
{
    if(CurrentRow < 0)
        return PProfile();

    Q_ASSERT(CurrentRow < Profiles.size());
    return Profiles[CurrentRow];
}
void CProfileModel::AddDefaultProfiles()
{
    PProfile def_profile(new CProfile);
    def_profile->Name = "<default>";
    def_profile->HeaderType = sl::HEADER_EXPANDED;
    def_profile->GridColumns = 4;
    def_profile->GridRows = 4;
    def_profile->ImageWidth = 800;
    Profiles.push_back(def_profile);

    PProfile profile1(new CProfile);
    profile1->Name = "4x4 | 1200px | hdr";
    profile1->HeaderType = sl::HEADER_EXPANDED;
    profile1->GridColumns = 4;
    profile1->GridRows = 4;
    profile1->ImageWidth = 1200;
    Profiles.push_back(profile1);

    PProfile profile2(new CProfile);
    profile2->Name = "4x8 | 1200px | hdr";
    profile2->HeaderType = sl::HEADER_EXPANDED;
    profile2->GridColumns = 4;
    profile2->GridRows = 8;
    profile2->ImageWidth = 1200;
    Profiles.push_back(profile2);
}
void CProfileModel::Load()
{
    CUpdateModel<CProfileModel> update(this);
    Profiles.clear();

    QString profiles_file_name = QCoreApplication::applicationDirPath() + PROFILES_FILE_NAME;
    QFile profiles_file(profiles_file_name);
    if(false == profiles_file.open(QIODevice::ReadOnly))
    {
        AddDefaultProfiles();
        return;
    }

    QDataStream stream(&profiles_file);
    quint32 header;
    stream >> header;
    if(header != PROFILES_FILE_HEADER)
        return;
    quint32 version;
    stream >> version;
    if(version != PROFILES_FILE_VERSION)
        return;
    quint16 count{0};
    stream >> count;
    if(count > MAX_PROFILES)
        return;

    for(int row = 0; row < count; ++row)
    {
        PProfile profile(new CProfile);
        stream >> profile->Name;
        stream >> profile->HeaderType;
        stream >> profile->HeaderFont;
        stream >> profile->ImageWidth;
        stream >> profile->GridColumns;
        stream >> profile->GridRows;
        stream >> profile->Timestamp;
        stream >> profile->TimestampFont;
        addProfile(profile->Name, profile);
    }

    if(0 == Profiles.size())
    {
        //TODO:
        //add default profiles
        PProfile def_profile(new CProfile);
        def_profile->Name = "<default>";
        Profiles.push_back(def_profile);

        PProfile profile1(new CProfile);
        profile1->Name = "<def_profile1>";
        Profiles.push_back(profile1);

        PProfile profile2(new CProfile);
        profile2->Name = "<def_profile2>";
        Profiles.push_back(profile2);
        return;
    }
}
void CProfileModel::Save()
{
    QString profiles_file_name = QCoreApplication::applicationDirPath() + PROFILES_FILE_NAME;
    QFile profiles_file(profiles_file_name);
    if(false == profiles_file.open(QIODevice::WriteOnly))
    {
        Q_ASSERT(0);
        return;
    }

    QDataStream stream(&profiles_file);
    stream << PROFILES_FILE_HEADER;
    stream << PROFILES_FILE_VERSION;
    stream << static_cast<quint16>(Profiles.size());
    for(const PProfile& profile : Profiles)
    {
        stream << profile->Name;
        stream << profile->HeaderType;
        stream << profile->HeaderFont;
        stream << profile->ImageWidth;
        stream << profile->GridColumns;
        stream << profile->GridRows;
        stream << profile->Timestamp;
        stream << profile->TimestampFont;
    }
}
bool CProfile::Compare(const CProfile &rop) const
{
    const sl::CProfile* sl_profile = this;
    const sl::CProfile* sl_rop = &rop;
    return *sl_profile == *sl_rop;
}
