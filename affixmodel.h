#ifndef AFFIXMODEL_H
#define AFFIXMODEL_H

#include <QAbstractItemModel>
#include <QSortFilterProxyModel>

#include "SuffixCollection.h"


class affixmodel : public QAbstractItemModel
{
    QList<CPrefix*> * m_prefixes;
    QList<CSuffix*>  * m_suffixes;
    int             rowCount(const QModelIndex &parent = QModelIndex())const override;
    int             columnCount(const QModelIndex &parent = QModelIndex())const override;
    QVariant        data(const QModelIndex & index, int role = Qt::DisplayRole)const override;
    QModelIndex     index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex     parent(const QModelIndex &index) const override;
    QVariant        headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void            sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

public:
    explicit affixmodel(QList<CPrefix*>* words, QObject *parent = nullptr);
    explicit affixmodel(QList<CSuffix*>* words, QObject *parent = nullptr);

    void     beginResetModel();
};

class affixSFProxymodel: public QSortFilterProxyModel{
    Q_OBJECT

public:
    affixSFProxymodel(QObject*);

private:
    bool lessThan(const QModelIndex & left, const QModelIndex & right) const override;

};










#endif // AFFIXMODEL_H
