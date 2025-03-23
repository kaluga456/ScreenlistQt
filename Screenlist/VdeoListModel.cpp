#include "VdeoListModel.h"

CVideoItemModel::CVideoItemModel(QObject* parent) : QAbstractItemModel(parent)
{
}
CVideoItemModel::~CVideoItemModel()
{
    for(auto i : Items)
        delete i;
}
int CVideoItemModel::columnCount(const QModelIndex &parent) const
{
    return COLUMNS_COUNT;
}
int CVideoItemModel::rowCount(const QModelIndex &parent) const
{
    return Items.size();
}
QVariant CVideoItemModel::data(const QModelIndex &index, int role) const
{
    if(Qt::DisplayRole == role)
    {
        const CVideoItem* pi = Get(index);
        switch(index.column())
        {
        case COLUMN_VIDEO: return pi->VideoFilePath;
        case COLUMN_STATE: return pi->State;
        case COLUMN_RESULT: return pi->ResultString;
        }
    }
    return QVariant();
}
QModelIndex CVideoItemModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column, Get(row));
}
QModelIndex CVideoItemModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}
QVariant CVideoItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(Qt::Horizontal == orientation && Qt::DisplayRole == role)
    {
        switch(section)
        {
        case COLUMN_VIDEO: return "Video File";
        case COLUMN_STATE: return "State";
        case COLUMN_RESULT: return "Result";
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}
void CVideoItemModel::Add(const char *vide_file_path)
{
    if(nullptr == vide_file_path)
        return;

    //TODO:
    //ignore duplicates
    // for(auto& i : Items)
    // {
    //     PVideoItem pi = i.second;
    //     if(0 == pi->SourceFileName.CompareNoCase(vide_file_path))
    //         return;
    // }

    Items.push_back(new CVideoItem(vide_file_path));
}
CVideoItem* CVideoItemModel::Get(int row)
{
    Q_ASSERT(row < Items.size());
    return (row < Items.size()) ? Items[row] : nullptr;
}
const CVideoItem* CVideoItemModel::Get(int row) const
{
    Q_ASSERT(row < Items.size());
    return (row < Items.size()) ? Items[row] : nullptr;
}
CVideoItem* CVideoItemModel::Get(const QModelIndex &index)
{
    const int i = index.row();
    Q_ASSERT(i < Items.size());
    return (i < Items.size()) ? Items[i] : nullptr;
}
const CVideoItem* CVideoItemModel::Get(const QModelIndex &index) const
{
    const int i = index.row();
    Q_ASSERT(i < Items.size());
    return (i < Items.size()) ? Items[i] : nullptr;
}
