#ifndef PROFILELIST_H
#define PROFILELIST_H

#include <QStringListModel>
#include "sl_profile.h"


class CProfile : public sl::CProfile
{
public:
    QString Name;
};
using PProfile = std::shared_ptr<CProfile>;

//profiles
//TODO: default profile
class CProfileModel : public QStringListModel
{
public:
    CProfileModel();
    ~CProfileModel();

    //overrides
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setProfile(int row, PProfile profile);
    void addProfile(QString name, PProfile profile);
    void deleteProfile(int row);
    PProfile getProfile(int row) const;

private:
    std::vector<PProfile> Profiles;
};

#endif // PROFILELIST_H
