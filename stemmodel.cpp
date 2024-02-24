#include "stemmodel.h"
#include "Signature.h"

stemmodel::stemmodel(QObject *parent){
    Q_UNUSED(parent);
    m_stems = nullptr;
}
stemmodel::stemmodel(CStemCollection * stems, QObject *parent)
    : QAbstractItemModel{parent}
{
    m_stems = stems->get_stem_list();
}
bool lessthan_key(CStem * a, CStem* b){
    return a->get_key() < b->get_key();
}
bool greaterthan_key(CStem* a, CStem* b){
    return a->get_key() > b->get_key();
}
bool lessthan_stem_count(CStem* a, CStem* b){
    return a->get_count() < b->get_count();
}
bool greaterthan_stem_count(CStem* a, CStem* b){
    return a->get_count() > b->get_count();
}
void stemmodel::sort(int column, Qt::SortOrder order){
    switch(column){
    case 0:{if (order == Qt::AscendingOrder) {
            std::sort(m_stems->begin(), m_stems->end(), lessthan_key);
        }else{
            std::sort(m_stems->begin(), m_stems->end(), greaterthan_key);
        }
        break;
    }
    case 1:{if (order == Qt::AscendingOrder) {
            std::sort(m_stems->begin(), m_stems->end(), lessthan_stem_count);
        } else {
            std::sort(m_stems->begin(), m_stems->end(), greaterthan_stem_count);
        }
        break;
    }
    default:
        break;
    }
    emit layoutChanged();
}
int stemmodel::rowCount(const QModelIndex &parent)const {
    Q_UNUSED(parent);
    if (m_stems) {
        return m_stems->size();
    }
    return 0;
}
int stemmodel::columnCount(const QModelIndex &parent)const {
    Q_UNUSED(parent);
    return 4;
}
QModelIndex stemmodel::index(int row, int column, const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return createIndex(row, column, nullptr);
}
QVariant stemmodel::data(const QModelIndex & index, int role)const {
    if (!index.isValid())
        return QVariant();
    int row = index.row();
    int column = index.column();
    if (row > m_stems->size()){ return QVariant();}
    if (!m_stems){return QVariant();}
    CStem* stem = m_stems->at(row);
    switch (column) {
    case 0:
        if (role==Qt::DisplayRole){             
            return QVariant(stem->get_key());
        }
        break;
    case 1:
        if (role==Qt::DisplayRole){
            return QVariant(stem->get_word_type_count());
        }
        break;
    case 2:
        if (role==Qt::DisplayRole){
            return QVariant(); // TO DO
           return QVariant(stem->get_last_signature()->get_key());
        }
        break;
    case 3:
        if (role == Qt::DisplayRole){
            if (stem->is_compound()){
                return QVariant (stem->display_compound_structure());
            }
        }
    default:
        return QVariant();
        break;
    }
    return QVariant();
}
QModelIndex stemmodel::parent(const QModelIndex &index) const {
    Q_UNUSED(index);
    return QModelIndex();
}
QVariant stemmodel::headerData(int section, Qt::Orientation orientation, int role ) const {
    if (role != Qt::DisplayRole) {return    QVariant();}
    if (orientation == Qt::Horizontal){
        if (section == 0) return QVariant("Suffixal stem" );
        if (section == 1) return QVariant("word type count" );
        if (section == 2) return QVariant("signature");
        if (section == 3) return QVariant("corpus count" );
    }
    return QVariant();
}
stemSFProxymodel::stemSFProxymodel(QObject* parent ):QSortFilterProxyModel (parent){

}

bool stemSFProxymodel::lessThan(const QModelIndex & left, const QModelIndex & right) const {
    int column = left.column();
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);
    switch (column){
    case 0:{
        return leftData.toString() < rightData.toString();
        break;
    }
    case 1:{
        return leftData.toInt() < rightData.toInt();
        break;
    }
    case 2:{
        return leftData.toString() < rightData.toString();
        break;
    }
    default:
        return true;
    }
}
