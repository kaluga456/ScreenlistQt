#ifndef VDEOLISTMODEL_H
#define VDEOLISTMODEL_H

#include <QHeaderView>
#include "VideoItem.h"

typedef std::shared_ptr<CVideoItem> PVideoItem;
typedef std::vector<CVideoItem*> CVideoItems;

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

    //
    void Add(const char* vide_file_path);
    CVideoItem* Get(const QModelIndex &index);
    const CVideoItem* Get(const QModelIndex &index) const;
    CVideoItem* Get(int row);
    const CVideoItem* Get(int row) const;

private:
    CVideoItems Items;
};

#endif // VDEOLISTMODEL_H
