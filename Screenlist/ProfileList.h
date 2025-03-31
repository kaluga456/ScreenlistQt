#ifndef PROFILELIST_H
#define PROFILELIST_H

#include <QStringListModel>
#include "sl_profile.h"
//////////////////////////////////////////////////////////////////////////////
//data model for combo boxes
class CStrIntModel : public QStringListModel
{
public:
    CStrIntModel();
    ~CStrIntModel();

    //overrides
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setDefaultRow(int def_row);
    void addItem(QString text, int data);

    int getRow(int data) const;
    int getData(int row) const;

private:
    int DefaultRow;
    class CComboBoxItem
    {
    public:
        CComboBoxItem(QString text, int data) : Text{text}, Data{data} {}
        QString Text;
        int Data;
    };
    std::vector<CComboBoxItem> Data;
};
//////////////////////////////////////////////////////////////////////////////
class CProfile : public sl::CProfile
{
public:
    QString Name;

    //compares only sl::CProfile data
    bool Compare(const CProfile& rop) const;
};
using PProfile = std::shared_ptr<CProfile>;
//////////////////////////////////////////////////////////////////////////////
//profiles
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
    int addProfile(QString name, PProfile profile);
    void deleteProfile(int row);

    //access
    PProfile getProfile(int row) const;
    PProfile getProfile(QString name) const;
    int getProfileRow(QString name) const;
    PProfile getCurrentProfile() const;
    void SetCurrentRow(int row);
    int GetCurrentRow() const {return CurrentRow;}

    //serialization
    void Load();
    void Save();

private:
    int CurrentRow{-1};
    void AddDefaultProfiles();
    std::vector<PProfile> Profiles;
};

#endif // PROFILELIST_H
