#include "parsemodel.h"
#include "cparse.h"

parsemodel::parsemodel(QList<CParse*> * parses, QObject * parent): QAbstractItemModel {parent} {
    m_parses = parses;
}

int parsemodel::rowCount(const QModelIndex &parent )const{
    return m_parses->count();
}
int parsemodel::columnCount (const QModelIndex & parent) const{
    return 3;
}
QVariant parsemodel::data(const QModelIndex & index, int role)const{
    if (!index.isValid())
        return QVariant();
    int row = index.row();
    int column = index.column();
    if (row > m_parses->count()){ return QVariant();}
}
QModelIndex parsemodel::index(int row, int column, const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return createIndex(row, column, nullptr);
}
QModelIndex parsemodel::parent(const QModelIndex &index) const {
    Q_UNUSED(index);
    return QModelIndex();
}
QVariant parsemodel::headerData(int section, Qt::Orientation orientation, int role ) const {
    if (role != Qt::DisplayRole) {return    QVariant();}
    if (orientation == Qt::Horizontal){
        if (section == 0) return QVariant("Stem" );
        if (section == 1) return QVariant("Affix");
        if (section == 2) return QVariant("Affix side");
    }else{if(orientation==Qt::Vertical){
            return QVariant(section);
        }
    }
    return QVariant();

}
bool lessthan_stem(CParse* parse1, CParse* parse2){
    return parse1->get_stem() < parse2->get_stem();
}
bool greaterthan_stem(CParse* parse1, CParse* parse2){
    return parse1->get_stem() > parse2->get_stem();
}
bool lessthan_affix(CParse* parse1, CParse* parse2){
    return parse1->get_affix() < parse2->get_affix();
}
bool greaterthan_affix(CParse* parse1, CParse* parse2){
    return parse1->get_affix() > parse2->get_affix();
}
void parsemodel::sort(int column, Qt::SortOrder order){
    switch(column){
    case 0:{if (order == Qt::AscendingOrder) {
            std::sort(m_parses->begin(), m_parses->end(), lessthan_stem);
        }else{
            std::sort(m_parses->begin(), m_parses->end(), greaterthan_stem);
        }
        break;
    }
    case 1:{if (order == Qt::AscendingOrder) {
            std::sort(m_parses->begin(), m_parses->end(), lessthan_affix);
        }else{
            std::sort(m_parses->begin(), m_parses->end(), greaterthan_affix);
        }
        break;
    }
    default:
        break;
    }
    emit layoutChanged();
}
