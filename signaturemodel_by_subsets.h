#ifndef SIGNATUREMODEL_BY_SUBSETS_H
#define SIGNATUREMODEL_BY_SUBSETS_H
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include "SignatureCollection.h"

class signaturemodel_by_subsets: public QAbstractItemModel
{

    CSignatureCollection*  m_signatures;
    bool                   m_suffix_flag;
    int                    rowCount(const QModelIndex &parent = QModelIndex())const override;
    int                    columnCount(const QModelIndex &parent = QModelIndex())const override;
    QVariant               data(const QModelIndex & index, int role = Qt::DisplayRole)const override;
    QModelIndex            index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex            parent(const QModelIndex &index) const override;
    QVariant               headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;



public:
    signaturemodel_by_subsets(CSignatureCollection *, bool suffix_flag, QObject *parent = nullptr);
};

#endif // SIGNATUREMODEL_BY_SUBSETS_H
