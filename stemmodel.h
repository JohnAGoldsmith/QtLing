#ifndef STEMMODEL_H
#define STEMMODEL_H

#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include "StemCollection.h"




class stemmodel : public QAbstractItemModel
{   QList<CStem*>  *m_stems;
    int             rowCount(const QModelIndex &parent = QModelIndex())const override;
    int             columnCount(const QModelIndex &parent = QModelIndex())const override;
    QVariant        data(const QModelIndex & index, int role = Qt::DisplayRole)const override;
    QModelIndex     index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex     parent(const QModelIndex &index) const override;
    QVariant        headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void            sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

public:
    explicit stemmodel(CStemCollection* stems, QObject *parent = nullptr);
    stemmodel(QObject *parent = nullptr);
};


class stemSFProxymodel: public QSortFilterProxyModel{
    Q_OBJECT

public:
    stemSFProxymodel(QObject*);
    ~stemSFProxymodel(){};

private:
    bool lessThan(const QModelIndex & left, const QModelIndex & right) const override;

};





#endif // STEMMODEL_H
