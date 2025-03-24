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
    friend class CResetModel;
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

    //serialization
    void Load();
    void Save();

private:
    void beginReset() {beginResetModel();}
    void endReset() {endResetModel();}
    std::vector<PProfile> Profiles;
};

class CResetModel
{
public:
    CResetModel(CProfileModel* model) : Model(model) {Model->beginResetModel();}
    ~CResetModel() {Model->endResetModel();}
private:
    CProfileModel* Model;
};

#endif // PROFILELIST_H
