#ifndef VDEOLISTMODEL_H
#define VDEOLISTMODEL_H

#include <QHeaderView>
#include "VideoItem.h"

typedef std::vector<PVideoItem> CVideoItems;

class CVideoItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    //columns
    enum : int
    {
        COLUMN_VIDEO,
        COLUMN_STATE,  //image or progress bar
        COLUMN_RESULT, //error description if any

        COLUMNS_COUNT
    };

    //ctor/dtor
    CVideoItemModel(QObject* parent);
    ~CVideoItemModel();

    //overrides
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    //drag and drop
    QStringList mimeTypes() const override;
    Qt::DropActions supportedDropActions() const override;
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

    //add
    void Add(QString vide_file_path);
    void Add(const QStringList& file_names);

    //remove
    void RemoveFailed();
    void RemoveSelected(QItemSelectionModel* sm);
    void RemoveCompleted();
    void RemoveAll();
    void ResetSelected(QItemSelectionModel* sm);
    void ResetAll();

    //access
    PVideoItem Get(const QModelIndex &index);
    PVideoItem Get(int row);
    int GetRow(CVideoItem* video_item) const;

    //if sm = null get any available video
    PVideoItem GetVideoToProcess(QItemSelectionModel* sm = nullptr);

    //
    void UpdateCurrent(bool full = true);

    //serialization
    void Load();
    void Save();

    //currently processing item
    PVideoItem CurrentVideo;

private:
    CVideoItems Items;

    void Add(PVideoItem item); //for loading from file
};

#endif // VDEOLISTMODEL_H
