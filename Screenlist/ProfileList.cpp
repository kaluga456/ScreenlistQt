#include <QFile>
#include <QDataStream>
#include <QCoreApplication>
#include "ProfileList.h"

//profiles file
constexpr const char* PROFILES_FILE_NAME = "/profiles.dat";
constexpr quint32 PROFILES_FILE_HEADER = 0xF103A86C;
constexpr quint32 PROFILES_FILE_VERSION = 1;
constexpr quint32 MAX_PROFILES = 50;

CProfileModel::CProfileModel()
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
    //TODO:
}
void CProfileModel::addProfile(QString name, PProfile profile)
{
    //TODO: ignore duplicates
    Profiles.push_back(profile);
}
void CProfileModel::deleteProfile(int row)
{
    Q_ASSERT(row < Profiles.size());
    if(row >= Profiles.size())
        return;
    Profiles.erase(Profiles.begin() + row);
}
PProfile CProfileModel::getProfile(int row) const
{
    Q_ASSERT(row < Profiles.size());
    if(row >= Profiles.size())
        return PProfile();
    return Profiles[row];
}
void CProfileModel::Load()
{
    CResetModel reset_model(this);
    Profiles.clear();

    QString profiles_file_name = QCoreApplication::applicationDirPath() + PROFILES_FILE_NAME;
    QFile profiles_file(profiles_file_name);
    if(false == profiles_file.open(QIODevice::ReadOnly))
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
