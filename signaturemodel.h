    #ifndef SIGNATUREMODEL_H
#define SIGNATUREMODEL_H

#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include "SignatureCollection.h"


class signaturemodel : public QAbstractItemModel
{   CSignatureCollection * m_signatures;
    bool                   m_suffix_flag;
    int                    rowCount(const QModelIndex &parent = QModelIndex())const override;
    int                    columnCount(const QModelIndex &parent = QModelIndex())const override;
    QVariant               data(const QModelIndex & index, int role = Qt::DisplayRole)const override;
    QModelIndex            index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex            parent(const QModelIndex &index) const override;
    QVariant               headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;


public:
    explicit signaturemodel(CSignatureCollection *, bool suffix_flag, QObject *parent = nullptr);
};

class signatureSFProxymodel: public QSortFilterProxyModel{
    Q_OBJECT

public:
    signatureSFProxymodel(QObject*);
    ~signatureSFProxymodel(){};

private:
    bool lessThan(const QModelIndex & left, const QModelIndex & right) const override;

};






#endif // SIGNATUREMODEL_H
