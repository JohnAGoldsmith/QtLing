#include "signaturemodel.h"

signaturemodel::signaturemodel(CSignatureCollection * signatures, QObject *parent)
    : QAbstractItemModel{parent}
{
    m_signatures = signatures;
}



int signaturemodel::rowCount(const QModelIndex &parent)const {
    return m_signatures->get_count();
}
int signaturemodel::columnCount(const QModelIndex &parent)const {
    return 5;
}
QModelIndex signaturemodel::index(int row, int column, const QModelIndex &parent) const {
    return createIndex(row, column, nullptr);
}
QVariant signaturemodel::data(const QModelIndex & index, int role)const {
    // this may be modified when we add the sortfilter object;

    if (!index.isValid())
        return QVariant();
    int row = index.row();
    int column = index.column();
    if (row > m_signatures->get_count()){ return QVariant();}
    switch (column) {
    case 0:
        if (role==Qt::DisplayRole){
            return QVariant(m_signatures->at(row)->display());
        }
        break;
    case 1:
        if (role==Qt::DisplayRole){
            return QVariant(m_signatures->at(row)->get_number_of_stems());
        }
        break;
    case 2:
        if (role==Qt::DisplayRole){
            return QVariant(m_signatures->at(row)->get_robustness());
        }
        break;
    case 3:
        if (role==Qt::DisplayRole){
            return QVariant(m_signatures->at(row)->get_stem_entropy());
        }
        break;
    case 4:
        if (role==Qt::DisplayRole){
            return QVariant(m_signatures->at(row)->get_number_of_affixes());
        }
        break;
    default:
        return QVariant();
        break;
    }
    return QVariant();
}
QModelIndex signaturemodel::parent(const QModelIndex &index) const {

}
QVariant signaturemodel::headerData(int section, Qt::Orientation orientation, int role ) const {
    if (role != Qt::DisplayRole) {return QVariant();}
    if (orientation == Qt::Horizontal){
        if (section == 0) return QVariant("Signature" );
        if (section == 1) return QVariant("stem count");
        if (section == 2) return QVariant("robustness");
        if (section == 3) return QVariant("fullness");
        if (section == 4) return QVariant("# affixes");

    }
    return QVariant();

}

signatureSFProxymodel::signatureSFProxymodel(QObject* parent ):QSortFilterProxyModel (parent){

}

bool signatureSFProxymodel::lessThan(const QModelIndex & left, const QModelIndex & right) const {
    int column = left.column();
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);
    switch (column){
    case 0:{
        return leftData.toString() < rightData.toString();
        break;
    }
    case 1: case 2: case 4:{
        return leftData.toInt() < rightData.toInt();
        break;
    }
    case 3:{
        return leftData.toFloat() < rightData.toFloat();
    }
    default:
        return true;
    }
}
