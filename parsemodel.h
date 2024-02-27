#ifndef PARSEMODEL_H
#define PARSEMODEL_H
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>

class CParse;

class parsemodel : public QAbstractItemModel
{
    QList<CParse*>* m_parses;
    int             m_number_of_columns;
    int             rowCount(const QModelIndex &parent = QModelIndex())const override;
    int             columnCount(const QModelIndex &parent = QModelIndex())const override;
    QVariant        data(const QModelIndex & index, int role = Qt::DisplayRole)const override;
    QModelIndex     index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex     parent(const QModelIndex &index) const override;
    QVariant        headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  //void            sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

public:
    parsemodel(QMap<QString,CParse*> * parses, QObject * parent);
};

#endif // PARSEMODEL_H
