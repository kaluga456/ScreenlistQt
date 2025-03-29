#include <QFile>
#include <QDataStream>
#include <QCoreApplication>
#include <QItemSelectionModel>
#include "VdeoListModel.h"

constexpr const char* VIDEO_LIST_FILE_NAME = "/list.dat";

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
    PVideoItem new_item(new CVideoItem(vide_file_path));
    Items.push_back(new_item);
}
void CVideoItemModel::Add(const QStringList &file_names)
{
    beginResetModel();
    for(const auto& file_name : file_names)
        Add(file_name);
    endResetModel();
}
void CVideoItemModel::RemoveFailed()
{
    beginResetModel();
    for(auto item_i = Items.begin(); item_i != Items.end();)
    {
        PVideoItem item = *item_i;
        if(item != CurrentVideo && PIS_FAILED == item->State)
            item_i = Items.erase(item_i);
        else
            ++item_i;
    }
    endResetModel();
}
void CVideoItemModel::RemoveSelected(QItemSelectionModel* sm)
{
    beginResetModel();
    QItemSelection is = sm->selection();
    QModelIndexList mil = is.indexes();
    for(auto mi = mil.begin(); mi != mil.end(); ++mi)
    {
        Q_ASSERT(mi->isValid());
        PVideoItem item = Items[mi->row()];
        if(item == CurrentVideo)
            continue;
        item->State = PIS_SELECTED;
    }
    for(auto item_i = Items.begin(); item_i != Items.end();)
    {
        PVideoItem item = *item_i;
        if(item != CurrentVideo && PIS_SELECTED == item->State)
            item_i = Items.erase(item_i);
        else
            ++item_i;
    }
    endResetModel();
}
void CVideoItemModel::RemoveCompleted()
{
    beginResetModel();
    for(auto item_i = Items.begin(); item_i != Items.end();)
    {
        PVideoItem item = *item_i;
        if(item != CurrentVideo && PIS_DONE == item->State)
            item_i = Items.erase(item_i);
        else
            ++item_i;
    }
    endResetModel();
}
void CVideoItemModel::RemoveAll()
{
    //TODO: CurrentVideo
    beginResetModel();
    Items.clear();
    endResetModel();
}
void CVideoItemModel::ResetSelected(QItemSelectionModel *sm)
{
    beginResetModel();
    QItemSelection is = sm->selection();
    QModelIndexList mil = is.indexes();

    for(const auto& mi : mil)
    {
        PVideoItem item = Items[mi.row()];
        if(item == CurrentVideo)
            continue;
        item->State = PIS_WAIT;
        item->ResultString = "";
    }
    endResetModel();
}
void CVideoItemModel::ResetAll()
{
    beginResetModel();
    for(PVideoItem& item : Items)
    {
        if(item == CurrentVideo)
            continue;
        item->State = PIS_WAIT;
        item->ResultString = "";
    }
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
void CVideoItemModel::Add(PVideoItem item)
{
    Q_ASSERT(item);
    Items.push_back(item);
}
void CVideoItemModel::Load()
{
    beginResetModel();
    Items.clear();

    QString list_file_name = QCoreApplication::applicationDirPath() + VIDEO_LIST_FILE_NAME;
    QFile list_file(list_file_name);
    if(false == list_file.open(QIODevice::ReadOnly))
    {
        endResetModel();
        return;
    }

    QDataStream stream(&list_file);
    quint32 size{0};
    stream >> size;

    QString input_name;
    int State{PIS_WAIT};
    QString result_string;
    for(int row = 0; row < size; ++row)
    {
        stream >> input_name;
        stream >> State;
        stream >> result_string;

        PVideoItem video_item(new CVideoItem(input_name));
        video_item->State = State;
        video_item->ResultString = result_string;
        Add(video_item);
    }

    endResetModel();
}

void CVideoItemModel::Save()
{
    QString list_file_name = QCoreApplication::applicationDirPath() + VIDEO_LIST_FILE_NAME;
    QFile list_file(list_file_name);
    if(false == list_file.open(QIODevice::WriteOnly))
    {
        Q_ASSERT(0);
        return;
    }

    QDataStream stream(&list_file);
    stream << static_cast<quint32>(Items.size());
    for(const PVideoItem& item : Items)
    {
        stream << item->VideoFilePath;
        stream << item->State;
        stream << item->ResultString;
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
