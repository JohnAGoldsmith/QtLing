#include <QPair>
#include <Qt>
#include <QCoreApplication>
#include "WordCollection.h"
#include "Word.h"
#include "mainwindow.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "QDebug"
#include "hypothesis.h"
#include "lxamodels.h"
#include "evaluation.h"
#include "compound.h"
#include "sigpair.h"
#include "sigpaircollection.h"


SigPairModel::SigPairModel(SigPairCollection* sigpairs){
    sig_pairs = sigpairs;
}

SigPairModel::~SigPairModel(){
    delete sig_pairs;
}
int SigPairModel::rowCount(const QModelIndex &parent) const {
    return sig_pairs->count();
}
int SigPairModel::columnCount(const QModelIndex &parent ) const {
    return 5;
}
QVariant SigPairModel::data (const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (role == Qt::DisplayRole){
        sig_pair * sigpair (sig_pairs->get_map()->values().at(index.row()));
        switch (index.column() ){
        case 0:
            return QVariant(sigpair->get_sig1_string());
            break;
        case 1:
            return QVariant(sigpair->get_sig2_string());
            break;
        case 2:
            return QVariant(sigpair->get_morph());
            break;
        case 3:
            return QVariant(sigpair->get_my_stems().count());
            break;
        case 4:
            return QVariant(sigpair->m_sig_2_entropy);
        }
    }
    return QVariant();
}
QVariant SigPairModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    QStringList these_headers;
    these_headers  << "Sig 1" << "Sig 2" << "Diff" << "count" << "Sig 2 fullness";
    if (orientation == Qt::Orientation::Horizontal){
        return QVariant(these_headers[section]);
    }
    return QVariant();
}

sigpairproxymodel::sigpairproxymodel(QObject* parent ):QSortFilterProxyModel (parent){

}

bool sigpairproxymodel::lessThan(const QModelIndex & left, const QModelIndex & right) const {
    int column = left.column();
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);
    return true;
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



LxaStandardItemModel::LxaStandardItemModel(MainWindow* main_window): QStandardItemModel(main_window)
{
    m_sort_style = UNSPECIFIED;
}
LxaStandardItemModel::LxaStandardItemModel(QString shortname)
{
    m_ShortName = shortname;
    m_sort_style = UNSPECIFIED;
}

LxaStandardItemModel::~LxaStandardItemModel()
{
}

LxaSortFilterProxyModel::LxaSortFilterProxyModel(QObject * parent) : QSortFilterProxyModel (parent)
{
    //nothing here.
}

bool LxaSortFilterProxyModel::lessThan(const QModelIndex & left, const QModelIndex & right) const
{   //int left_row, right_row;

    QVariant leftdata = sourceModel()->data(left);
    QVariant rightdata = sourceModel()->data(right);

    if (leftdata.type() == QVariant::String && rightdata.type() == QVariant::String) {
        QString sig1 = leftdata.toString();
        QString sig2 = rightdata.toString();
        int len1 = sig1.split('=').length();
        int len2 = sig2.split('=').length();
        if (len1 != len2)
            return len1 < len2;
        else
            return sig1 < sig2;
    } else return QSortFilterProxyModel::lessThan(left, right);
};


void LxaStandardItemModel::load_compounds(CompoundWordCollection *p_compounds)
{
    typedef QStandardItem QSI;
    typedef CompoundWord::CompoundComposition CompoundComposition;
    QStringList labels;
    labels << "Compound word" << "Possible Compositions";
    setHorizontalHeaderLabels(labels);
    clear();
    QMap<QString, CompoundWord*>::ConstIterator compound_iter;
    const QMap<QString, CompoundWord*>& ref_map = p_compounds->get_map();
    for (compound_iter = ref_map.constBegin();
         compound_iter != ref_map.constEnd();
         compound_iter++) {
        QList<QSI*> item_list;
        QSI* item0 = new QSI(compound_iter.key());
        item_list.append(item0);
        CompoundWord* p_compoundword = compound_iter.value();
        const QList<CompoundComposition*>& composition_list = p_compoundword->get_compositions();
        QList<CompoundComposition*>::ConstIterator composition_iter;
        for (composition_iter = composition_list.constBegin();
             composition_iter != composition_list.constEnd();
             composition_iter++) {
            QSI* composition_item = new QSI(p_compoundword->composition_to_str(*composition_iter));
            item_list.append(composition_item);
        }
        appendRow(item_list);
    }
}



void LxaStandardItemModel::load_parasuffixes(QMap<QString, QStringList *> *  continuations ){
    QStringList labels = {tr("continuation"), tr("word count"), tr("??")};
    setHorizontalHeaderLabels(labels);
    m_Description = QString (" ");
    clear();
    QMapIterator<QString, QStringList*> iter(*continuations);
    while (iter.hasNext())
    {   iter.next();
        QList<QStandardItem*> item_list;
        item_list.append(new QStandardItem(iter.key()));

        QStandardItem* pItem2 = new QStandardItem();
        pItem2->setData(iter.value()->length(), Qt::DisplayRole); // --- Numerical data
        item_list.append(pItem2);
        appendRow(item_list);
    }
}
bool sort_function_1(const QPair<QString ,int >  * a ,  QPair<QString , int >  * b)
{
    if (a->second==b->second){return a->first < b->first;}
    else{
        return a->second > b->second;
    }
};






/*
struct{
    bool operator ()(sig_graph_edge* a, sig_graph_edge* b) const {
     return a->shared_word_stems.size() > b->shared_word_stems.size();
    }
}custom_compare;
*/

struct{
    bool operator ()(sig_pair* a, sig_pair* b) const
    {  if (a->m_stem_difference == b->m_stem_difference)
        {
                return a->m_sig_1 < b->m_sig_1;
        }
          else
        {
            return a->m_stem_difference < b->m_stem_difference;
        }
    }
}custom_compare_2;


void LxaStandardItemModel::load_sig_graph_edges( QMap<QString, sig_pair*> * this_sig_graph_edge_map, int size )
{
    clear();
    QList<sig_pair*>               temp_list;
    int MINIMUM_NUMBER_OF_SHARED_WORDS = 3;
    QMapIterator<word_t, sig_pair*> * this_sig_graph_edge_iter
            = new QMapIterator<word_t, sig_pair*>( * this_sig_graph_edge_map );
    while (this_sig_graph_edge_iter->hasNext())    {
        this_sig_graph_edge_iter->next();

        if (this_sig_graph_edge_iter->value()->shared_word_stems.count() >= MINIMUM_NUMBER_OF_SHARED_WORDS) {
            temp_list.append(this_sig_graph_edge_iter->value());
        }
    }
    std::sort( temp_list.begin(),  temp_list.end(), custom_compare_2);
    QListIterator<sig_pair*> temp_list_iter (temp_list);
    while (temp_list_iter.hasNext())
     {
        sig_pair * p_sig_graph_edge = temp_list_iter.next();
        if (size == 1 && p_sig_graph_edge->m_stem_difference.length() > 1){
            continue;
        }else if (size == 2 && p_sig_graph_edge->m_stem_difference.length()< 2){
            continue;
        }
        QStandardItem * item1 = new QStandardItem(p_sig_graph_edge->m_stem_difference);
        QStandardItem * item2 = new QStandardItem(p_sig_graph_edge->m_sig_string_1); // changed here
        QStandardItem * item3 = new QStandardItem();
        if (p_sig_graph_edge->m_sig_1)              // --- Numerical data --- //
            item3->setData(p_sig_graph_edge->m_sig_1->get_stem_entropy(), Qt::DisplayRole);
        else
            item3->setData("N/A", Qt::DisplayRole); // --- Numerical data --- //
        QStandardItem * item4 = new QStandardItem(p_sig_graph_edge->m_sig_string_2);
        QStandardItem * item5 = new QStandardItem();
        if (p_sig_graph_edge->m_sig_2)
            item5->setData(p_sig_graph_edge->m_sig_2->get_stem_entropy(), Qt::DisplayRole);
        else
            item5->setData("N/A", Qt::DisplayRole); // --- Numerical data --- //
        QStandardItem * item6 = new QStandardItem();// --- Numerical data --- //
        item6->setData(p_sig_graph_edge->shared_word_stems.size(), Qt::DisplayRole);
        QStandardItem * item7 = new QStandardItem(p_sig_graph_edge->label());
        QList<QStandardItem*> items;
        items.append(item1);
        items.append(item2);
        items.append(item3);
        items.append(item4);
        items.append(item5);
        items.append(item6);
        items.append(item7);

        appendRow(items);
     }


}

void LxaStandardItemModel::load_parsemap_from_gs(GoldStandard* p_gs, ParseMapHandler parsemap, const QString& type)
{
    clear();
    QStringList labels;
    labels << type << "parses";
    setHorizontalHeaderLabels(labels);

    typedef QStandardItem QSI;
    GoldStandard::Word_to_parse_triple_collection_map::ConstIterator gs_iter, pm_iter;
    GoldStandard::Parse_triple_collection::ConstIterator ptm_iter;
    ParseMapHandler p_all_word_gsm = p_gs->get_gs_parses();

    for (gs_iter = p_all_word_gsm->constBegin(); gs_iter != p_all_word_gsm->constEnd(); gs_iter++) {
        QList<QSI*> items;
        QString this_word = gs_iter.key();
        QSI* word_item = new QSI(this_word);
        items.append(word_item);
        pm_iter = parsemap->find(this_word);
        if (pm_iter != parsemap->constEnd()) {
            GoldStandard::Parse_triple_collection* ptm = pm_iter.value();
            for (ptm_iter = ptm->constBegin(); ptm_iter != ptm->constEnd(); ptm_iter++) {
                Parse_triple* this_pt = ptm_iter.value();
                QString this_parse = this_pt->m_stem + "=" + this_pt->m_suffix;
                items.append(new QStandardItem(this_parse));
            }
        }
        appendRow(items);
    }
}

