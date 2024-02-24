#ifndef HYPOTHESISMODEL_H
#define HYPOTHESISMODEL_H
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include "hypothesis.h"




class hypothesismodel : public QAbstractItemModel
{
    QList<CHypothesis*> *        m_hypotheses;

public:
    hypothesismodel(QList<CHypothesis*> * hypotheses);
    int             rowCount(const QModelIndex &parent = QModelIndex())const override;
    int             columnCount(const QModelIndex &parent = QModelIndex())const override;
    QVariant        data(const QModelIndex & index, int role = Qt::DisplayRole)const override;
    QModelIndex     index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex     parent(const QModelIndex &index) const override;
    QVariant        headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
//    void            sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
};

#endif // HYPOTHESISMODEL_H
