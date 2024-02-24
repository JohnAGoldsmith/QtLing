#include "hypothesismodel.h"

hypothesismodel::hypothesismodel(QList<CHypothesis*> * hypotheses) {
    m_hypotheses = hypotheses;
}

int             hypothesismodel::rowCount(const QModelIndex &parent )const {
    return 5;
}
int             hypothesismodel::columnCount(const QModelIndex &parent )const {
    return m_hypotheses->count();
}
QVariant        hypothesismodel::data(const QModelIndex & index, int role )const {
    if (!index.isValid())
        return QVariant();
    int row = index.row();
    int column = index.column();
    if (row > m_hypotheses->count()){ return QVariant();}
    switch (column) {
    case 0:
        if (role==Qt::DisplayRole){
            return QVariant(m_hypotheses->at(row)->express_as_string());
        }
        break;
    default:
        return QVariant();
    }
}
QModelIndex     hypothesismodel::index(int row, int column, const QModelIndex &parent ) const {
  return createIndex(row, column, nullptr);
}
QModelIndex     hypothesismodel::parent(const QModelIndex &index) const {

}
QVariant        hypothesismodel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {return QVariant();}
    if (orientation == Qt::Horizontal){
        if (section == 0) return QVariant("Hypothesis" );
        /*
        if (section == 1) return QVariant("stem count");
        if (section == 2) return QVariant("robustness");
        if (section == 3) return QVariant("fullness");
        if (section == 4) return QVariant("# affixes");
        */
    }
    return QVariant();
}
//void            hypothesismodel::sort(int column, Qt::SortOrder order ) {
//
//};
