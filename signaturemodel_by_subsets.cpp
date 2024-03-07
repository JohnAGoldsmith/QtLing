
#include "signaturemodel_by_subsets.h"

signaturemodel_by_subsets::signaturemodel_by_subsets(CSignatureCollection * signatures, bool suffix_flag, QObject *parent)
    : QAbstractItemModel{parent}
{   m_suffix_flag = suffix_flag;
    m_entries.clear(); // worry about memory leaks here.
    m_signatures = signatures;
    int number_of_rows = 0;
    QList<CSignature*> * sig_list;
    CSignature* sig;
    QList<QList<CSignature*>* > * list_by_subset = signatures->get_sort_list_by_subsets();
    for (int n = 0; n < list_by_subset->count(); n++){
        sig_list = list_by_subset->at(n);
        int robustness (0);
        foreach(CSignature* sig, *sig_list){
            robustness += sig->get_robustness();
        }
        for(int n = 0; n < sig_list->count(); n++ ){
            QStringList * table_row;
            table_row = new QStringList();
            sig = sig_list->at(n);
            if (n==0) {
                table_row->append(QString::number(robustness));
            } else {
                table_row->append(QString());
            }
            table_row->append(sig->display());
            m_entries.append(table_row);
            number_of_rows++;
        }
        QStringList * temp;
        temp = new QStringList();
        *temp << QString() << QString();
        m_entries.append(temp);
        number_of_rows++;
    }
}


int signaturemodel_by_subsets::rowCount(const QModelIndex &parent)const {
    Q_UNUSED(parent);
    return m_signatures->get_count();
}
int signaturemodel_by_subsets::columnCount(const QModelIndex &parent)const {
    Q_UNUSED(parent);
    return 2;
}
QModelIndex signaturemodel_by_subsets::index(int row, int column, const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return createIndex(row, column, nullptr);
}
QVariant signaturemodel_by_subsets::data(const QModelIndex & index, int role)const {
    // this may be modified when we add the sortfilter object;

    if (!index.isValid())
        return QVariant();
    int row = index.row();
    int column = index.column();
    QStringList * entry;
    entry = m_entries.at(row);
    if (row > m_entries.count()){ return QVariant();}
    switch (column) {
    case 0:
        if (role==Qt::DisplayRole){
            return QVariant(entry->at(0));
        }
        break;
    case 1:
        if (role==Qt::DisplayRole){
            return QVariant(entry->at(1));
        }
        break;

    default:
        return QVariant();
        break;
    }
    return QVariant();
}
QModelIndex signaturemodel_by_subsets::parent(const QModelIndex &index) const {
    Q_UNUSED(index);
    return QModelIndex();
}
QVariant signaturemodel_by_subsets::headerData(int section, Qt::Orientation orientation, int role ) const {
    if (role != Qt::DisplayRole) {return QVariant();}
    if (orientation == Qt::Horizontal){
        if (section == 0) return QVariant("Signature" );
        if (section == 1) return QVariant("stem count");

    }
    return QVariant();

}


