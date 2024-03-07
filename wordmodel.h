#ifndef WORDMODEL_H
#define WORDMODEL_H
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include "WordCollection.h"


class wordmodel : public QAbstractItemModel
{

    CWordCollection * m_words;
    int             m_number_of_columns;

    int             rowCount(const QModelIndex &parent = QModelIndex())const override;
    int             columnCount(const QModelIndex &parent = QModelIndex())const override;
    QVariant        data(const QModelIndex & index, int role = Qt::DisplayRole)const override;
    QModelIndex     index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex     parent(const QModelIndex &index) const override;
    QVariant        headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void            sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
public:
    explicit wordmodel(CWordCollection* words, QObject *parent = nullptr);
    void     beginResetModel();
};

class wordSFProxymodel: public QSortFilterProxyModel{
    Q_OBJECT

public:
    wordSFProxymodel(QObject*);

private:
    bool lessThan(const QModelIndex & left, const QModelIndex & right) const override;

};


#endif // WORDMODEL_H
