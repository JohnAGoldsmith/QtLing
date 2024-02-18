#include "wordmodel.h"

wordmodel::wordmodel(CWordCollection * words, QObject *parent)
    : QAbstractItemModel{parent}
{
    m_words = words;
    int number_of_analyses = 0;
    foreach (CWord* word, *m_words->get_word_list()){
        if (word->get_parse_triple_list()->count() > number_of_analyses){
            number_of_analyses = word->get_parse_triple_list()->count();
        }
    }
    m_number_of_columns = number_of_analyses + 1;
}
bool lessthan_key(CWord* a, CWord* b){
    return a->get_key() < b->get_key();
}
bool greaterthan_key(CWord* a, CWord* b){
    return a->get_key() > b->get_key();
}
bool lessthan_word_count(CWord* a, CWord* b){
    return a->count() < b->count();
}
bool greaterthan_word_count(CWord* a, CWord* b){
    return a->count() > b->count();
}
void wordmodel::sort(int column, Qt::SortOrder order){
    switch(column){
    case 0:{if (order == Qt::AscendingOrder) {
            std::sort(m_words->get_word_list()->begin(), m_words->get_word_list()->end(), lessthan_key);
        }else{
            std::sort(m_words->get_word_list()->begin(), m_words->get_word_list()->end(), greaterthan_key);
        }
        break;
    }
    case 1:{if (order == Qt::AscendingOrder) {
            std::sort(m_words->get_word_list()->begin(), m_words->get_word_list()->end(), lessthan_word_count);
        } else {
            std::sort(m_words->get_word_list()->begin(), m_words->get_word_list()->end(), greaterthan_word_count);
        }
        break;
    }
    default:
        break;
    }
    emit layoutChanged();
}
int wordmodel::rowCount(const QModelIndex &parent)const {
    Q_UNUSED(parent);
    return m_words->get_count();
}
int wordmodel::columnCount(const QModelIndex &parent)const {
     Q_UNUSED(parent);

     return m_number_of_columns;
}
QModelIndex wordmodel::index(int row, int column, const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return createIndex(row, column, nullptr);
}
QVariant wordmodel::data(const QModelIndex & index, int role)const {
    if (!index.isValid())
        return QVariant();
    int row = index.row();
    int column = index.column();
    if (row > m_words->get_count()){ return QVariant();}
    switch (column) {
    case 0:{
        if (role==Qt::DisplayRole){
            return QVariant(m_words->get_word(row)->get_key());
        } else{
            return QVariant();
        }
        if (role == Qt::BackgroundRole){
            return   QColor(255, 0, 0);
        }
        break;
    }
    case 1:{
        if (role==Qt::DisplayRole){
            return QVariant(m_words->get_word(row)->count());
        } else{
            return QVariant();
        }
        break;
    }
    case 2:{

    }
    default:
        return QVariant();
        break;
    }
}
QModelIndex wordmodel::parent(const QModelIndex &index) const {
     Q_UNUSED(index);
    return QModelIndex();
}
QVariant wordmodel::headerData(int section, Qt::Orientation orientation, int role ) const {
    if (role != Qt::DisplayRole) {return    QVariant();}
    if (orientation == Qt::Horizontal){
        if (section == 0) return QVariant("Word" );
        if (section == 1) return QVariant("word count");
    }else{if(orientation==Qt::Vertical){
            return QVariant(section);
        }
    }
    return QVariant();

}
void wordmodel::beginResetModel(){
    QAbstractItemModel::beginResetModel();
}

wordSFProxymodel::wordSFProxymodel(QObject* parent ):QSortFilterProxyModel (parent){

}

bool wordSFProxymodel::lessThan(const QModelIndex & left, const QModelIndex & right) const {
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
    default:
        return true;
    }
}
