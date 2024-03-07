#include "hypothesismodel.h"

hypothesismodel::hypothesismodel(QList<CHypothesis*> * hypotheses) {
    m_hypotheses = hypotheses;
}

int             hypothesismodel::rowCount(const QModelIndex &parent )const {
    Q_UNUSED(parent);
    return m_hypotheses->count();
}
int             hypothesismodel::columnCount(const QModelIndex &parent )const {
    Q_UNUSED(parent);
    return m_hypotheses->count();
}
QVariant        hypothesismodel::data(const QModelIndex & index, int role )const {
    if (!index.isValid())
        return QVariant();
    int row = index.row();
    int column = index.column();
    CHypothesis* hypothesis(m_hypotheses->at(row));
    if (row > m_hypotheses->count()){ return QVariant();}
    switch (column) {
    case 0:
        if (role==Qt::DisplayRole){
            return QVariant(hypothesis->get_morpheme());
        }
        break;
    case 1:{
        if (role==Qt::DisplayRole){
            return QVariant ("=>");
        }
        break;
    }
    case 2:{
        if (role==Qt::DisplayRole){
            return QVariant(hypothesis->get_sig1());
        }
        break;
    }
    case 3:{
        if (role==Qt::DisplayRole){
            return QVariant(hypothesis->get_sig2());
        }
        break;
    }
    case 4:{
        if (role==Qt::DisplayRole){
            return QVariant(hypothesis->get_number_of_words_saved());
        }
        break;
    }
    default:
        return QVariant();
    }
    return QVariant();
}
QModelIndex     hypothesismodel::index(int row, int column, const QModelIndex &parent ) const {
    Q_UNUSED(parent);
  return createIndex(row, column, nullptr);
}
QModelIndex     hypothesismodel::parent(const QModelIndex &index) const {
    Q_UNUSED(index);
    return QModelIndex();
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
bool less_than_affix(CHypothesis* hypothesis1, CHypothesis* hypothesis2){
    return hypothesis1->get_morpheme() < hypothesis2->get_morpheme();
}
bool greater_than_affix(CHypothesis* hypothesis1, CHypothesis* hypothesis2){
    return hypothesis1->get_morpheme() > hypothesis2->get_morpheme();
}
bool less_than_sig1(CHypothesis* hypothesis1, CHypothesis* hypothesis2){
    return hypothesis1->get_sig1() < hypothesis2->get_sig1();
}
bool greater_than_sig1(CHypothesis* hypothesis1, CHypothesis* hypothesis2){
    return hypothesis1->get_sig1() > hypothesis2->get_sig1();
}
bool less_than_sig2(CHypothesis* hypothesis1, CHypothesis* hypothesis2){
    return hypothesis1->get_sig2() < hypothesis2->get_sig2();
}
bool greater_than_sig2(CHypothesis* hypothesis1, CHypothesis* hypothesis2){
    return hypothesis1->get_sig2() > hypothesis2->get_sig2();
}
void hypothesismodel::sort(int column, Qt::SortOrder order ) {
    switch (column){
    case 0:{
        if (order == Qt::AscendingOrder){
            std::sort(m_hypotheses->begin(), m_hypotheses->end(), less_than_affix);
        } else{
            std::sort(m_hypotheses->begin(), m_hypotheses->end(), greater_than_affix);
        }
        break;
    }
    case 2:{
        if (order == Qt::AscendingOrder){
            std::sort(m_hypotheses->begin(), m_hypotheses->end(), less_than_sig1);
        } else{
            std::sort(m_hypotheses->begin(), m_hypotheses->end(), greater_than_sig1);
        }
        break;
    }
    case 3:{
        if (order == Qt::AscendingOrder){
            std::sort(m_hypotheses->begin(), m_hypotheses->end(), less_than_sig2);
        } else{
            std::sort(m_hypotheses->begin(), m_hypotheses->end(), greater_than_sig2);
        }
        break;
    }
    }









    emit layoutChanged();
};
