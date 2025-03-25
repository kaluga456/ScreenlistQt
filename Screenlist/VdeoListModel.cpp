#include "VdeoListModel.h"

CVideoItemModel::CVideoItemModel(QObject* parent) : QAbstractItemModel(parent)
{
}
CVideoItemModel::~CVideoItemModel()
{
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
        const PVideoItem pi = Get(index);
        switch(index.column())
        {
        case COLUMN_VIDEO: return pi->VideoFilePath;
        case COLUMN_STATE: return GetStateString(pi->State);
        case COLUMN_RESULT: return pi->ResultString;
        }
    }
    return QVariant();
}
QModelIndex CVideoItemModel::index(int row, int column, const QModelIndex &parent) const
{
    //TODO:
    return createIndex(row, column, Get(row).get());
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
void CVideoItemModel::sort(int column, Qt::SortOrder order)
{
    //TODO:
    if(COLUMN_VIDEO == column)
    {
        beginResetModel();
        if(Qt::AscendingOrder == order)
            std::sort(Items.begin(), Items.end(), [](const PVideoItem& item1, const PVideoItem& item2) {return item1->VideoFilePath < item2->VideoFilePath;});
        else
            std::sort(Items.begin(), Items.end(), [](const PVideoItem& item1, const PVideoItem& item2) {return item1->VideoFilePath > item2->VideoFilePath;});
        endResetModel();
    }
    else if(COLUMN_STATE == column)
    {
        beginResetModel();
        if(Qt::AscendingOrder == order)
            std::sort(Items.begin(), Items.end(), [](const PVideoItem& item1, const PVideoItem& item2) {return item1->State < item2->State;});
        else
            std::sort(Items.begin(), Items.end(), [](const PVideoItem& item1, const PVideoItem& item2) {return item1->State > item2->State;});
        endResetModel();
    }
}
void CVideoItemModel::Add(QString vide_file_path)
{
    if(nullptr == vide_file_path)
        return;

    //ignore duplicates
    for(const auto& video_item : Items)
    {
        if(0 == video_item->VideoFilePath.compare(vide_file_path))
            return;
    }
    Items.push_back(PVideoItem(new CVideoItem(vide_file_path)));
}
void CVideoItemModel::Add(const QStringList &file_names)
{
    beginResetModel();
    for(const auto& file_name : file_names)
        Add(file_name);
    endResetModel();
}
PVideoItem CVideoItemModel::Get(int row)
{
    Q_ASSERT(row < Items.size());
    return (row < Items.size()) ? Items[row] : PVideoItem();
}
const PVideoItem CVideoItemModel::Get(int row) const
{
    Q_ASSERT(row < Items.size());
    return (row < Items.size()) ? Items[row] : nullptr;
}

int CVideoItemModel::GetRow(CVideoItem *video_item) const
{
    if(nullptr == video_item)
        return -1;

    int row = -1;
    int index = 0;
    for(const auto& item : Items)
    {
        if(video_item == item.get())
        {
            row = index;
            break;
        }
        ++index;
    }
    return row;
}
void CVideoItemModel::Update(CVideoItem *video_item, bool full /*= true*/)
{
    const int row = GetRow(video_item);
    if(row < 0)
        return;

    QModelIndex mi = createIndex(row, COLUMN_STATE);
    QList<int>roles{Qt::DisplayRole};
    emit dataChanged(mi, mi, roles);

    if(full)
    {
        QModelIndex mi = createIndex(row, COLUMN_RESULT);
        QList<int>roles{Qt::DisplayRole};
        emit dataChanged(mi, mi, roles);
    }
}
PVideoItem CVideoItemModel::Get(const QModelIndex &index)
{
    const int i = index.row();
    if(i < 0 || Items.size() <= i)
        return nullptr;
    return Items[i];
}
const PVideoItem CVideoItemModel::Get(const QModelIndex &index) const
{
    const int i = index.row();
    if(i < 0 || Items.size() <= i)
        return nullptr;
    return Items[i];
}
