#include "signaturemodel_by_subsets.h"

bool helper ( QPair<int ,int > pair1 , QPair<int,int> pair2) {
    return        pair1.second > pair2.second;
}

signaturemodel_by_subsets::signaturemodel_by_subsets(CSignatureCollection * signatures, bool suffix_flag, QObject *parent)
    : QAbstractItemModel{parent}
{   m_suffix_flag = suffix_flag;
    m_entries.clear(); // worry about memory leaks here.
    m_signatures = signatures;
    int number_of_rows = 0;
    QList<CSignature*> * sig_list;
    CSignature* sig;
    QList<QList<CSignature*>* > * list_by_subset = signatures->get_sort_list_by_subsets();
    QList<QPair<int, int >> sigindex2robustness_list;
    QMap<QString, int> sig2robustness_map;
    for (int n = 0; n < list_by_subset->count(); n++){
        sig_list = list_by_subset->at(n);
        int robustness (0);
        QString first_sig_key = sig_list->at(0)->get_key();
        foreach(CSignature* sig, *sig_list){
            robustness += sig->get_robustness();
        }
        sigindex2robustness_list.append(QPair<int,int>( n, robustness) );
        sig2robustness_map[first_sig_key] = robustness;
    }

    std::sort(sigindex2robustness_list.begin(), sigindex2robustness_list.end(), helper);

    for (int n= 0; n < sigindex2robustness_list.count(); n++){
        int subset_index = sigindex2robustness_list[n].first;
        sig_list = list_by_subset->at(subset_index);
        for (int m = 0; m < sig_list->count(); m++){
            QStringList * table_row;
            table_row = new QStringList();
            qDebug() <<  37 << "";
            sig = sig_list->at(m);
            if (m==0) {
                int total_robustness = sig2robustness_map[sig->get_key()];
                table_row->append(QString::number(total_robustness ) );
            } else {
                table_row->append(QString());
            }
            qDebug() << "";
            table_row->append(sig->display());
            qDebug() << 47  << sig->display();
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
        if (section == 0) return QVariant("Robustness" );
        if (section == 1) return QVariant("Signature");

    }
    return QVariant();

}


