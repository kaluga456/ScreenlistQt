#include "ProfileList.h"


CProfileModel::CProfileModel()
{
    //TEST:
    PProfile def = PProfile(new CProfile);
    def->Name = "<default>";
    def->Timestamp = sl::TIMESTAMP_TYPE_BOTTOM_RIGHT;
    Profiles.push_back(def);
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
