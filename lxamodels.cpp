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
            return QVariant(sigpair->get_sig_2()->get_stem_entropy());
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

void  LxaStandardItemModel::load_category(QString , eComponentType)
{

}

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

void LxaStandardItemModel::load_protostems(QMap<QString, protostem *>* p_protostems)
{
    typedef QStandardItem QSI;
    clear();
    protostem* curr_protostem;
    QMapIterator<QString, protostem*> iter(*p_protostems);
    QStringList labels;
    labels << "Protostem" << "Word Count";
    setHorizontalHeaderLabels(labels);
    while (iter.hasNext()) {
        curr_protostem = iter.next().value();
        QList<QSI*> item_list;
        QSI* item1 = new QSI(curr_protostem->get_stem());
        item_list.append(item1);
        QSI* item2 = new QSI();
        int word_count = curr_protostem->get_end_word()
                - curr_protostem->get_start_word() + 1;
        item2->setData(word_count, Qt::DisplayRole); // --- Numerical data --- //
        item_list.append(item2);
        appendRow(item_list);
    }
}



void LxaStandardItemModel::load_parasignatures(CSignatureCollection* p_signatures)
{
    clear();
    m_Description = " ";
    CSignature*         sig;
    p_signatures->sort(SIG_BY_AFFIX_COUNT);
    m_sort_style = SIG_BY_AFFIX_COUNT;
    stem_t  this_stem;
    QList<QStandardItem*> items;
    QStandardItem * item1 = new QStandardItem("stem");
    QStandardItem * item2 = new QStandardItem("robustness");
    QStandardItem * item3 = new QStandardItem("signature");
    items.append(item1);
    items.append(item2);
    items.append(item3);
    appendRow(items);

    for (int signo = 0; signo<p_signatures->get_count(); signo++)
    {   sig = p_signatures->get_at_sorted(signo);
        QList<QStandardItem*> items;
        QStandardItem * item1 = new QStandardItem(sig->get_stems()->first()->get_key());
        QStandardItem * item3 = new QStandardItem();
        item3->setData(sig->get_robustness(), Qt::DisplayRole); // --- Numerical data --- //
        items.append(item1);
        items.append(item3);
        items.append(new QStandardItem(sig->get_key()));
        appendRow(items);
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
void LxaStandardItemModel::load_hypotheses(QList<CHypothesis*>* p_hypotheses)
{
    clear();
    m_Description = " ";
    CHypothesis*         hypothesis;
    QMap<QString,QList<CHypothesis*> >  Hypothesis_map;
    QMap<QString,int>                   Key_counts;
    QList<QPair<QString,int>*>           pairs;

    for (int hypno = 0; hypno<p_hypotheses->count(); hypno++)
    {   hypothesis = p_hypotheses->at(hypno);
        QString key = hypothesis->get_key();
        if (Key_counts.contains(key))  {
                Key_counts[key] += hypothesis->get_number_of_words_saved();
            } else{
                Key_counts[key] = hypothesis->get_number_of_words_saved();
            }
    }
    foreach (QString key, Key_counts.keys()){
            QPair<QString,int> * pair = new QPair<QString, int>(key, Key_counts[key]);
        pairs.append(pair);
    }
    std::sort(pairs.begin(),pairs.end(),sort_function_1);
    QListIterator<QPair<QString,int>*> iter (pairs);
    while (iter.hasNext()){
        QList<QStandardItem*> items;
        QPair<QString,int>* pPair = iter.next();
        QStandardItem* item1 = new QStandardItem();
        item1->setData(pPair->second, Qt::DisplayRole);     // --- Numerical data --- //
        items.append(item1);
        QString this_key = pPair->first;
        QStringList key_list = this_key.split("@");
        foreach (QString key, key_list){
            QStandardItem * this_item = new QStandardItem(key);
            items.append(this_item);        }
        appendRow(items);
    }
}
void LxaStandardItemModel::load_hypotheses_2(QList<CHypothesis*>* p_hypotheses)
{
    clear();
    m_Description = " ";
    CHypothesis*         hypothesis;
    QMap<QString,QList<CHypothesis*> >  Hypothesis_map;
    QMap<QString,int>                   Key_counts;
    QList<QPair<QString,int>*>           pairs;

    QStringList labels;
    labels  <<  "rule input" << "" << "" << "original sig 1"
             << "word count";
    setHorizontalHeaderLabels(labels);
    for (int hypno = 0; hypno<p_hypotheses->count(); hypno++)
    {   hypothesis = p_hypotheses->at(hypno);
        QList<QStandardItem*> items;
        QStandardItem * item1 = new QStandardItem(hypothesis->get_morpheme());
        items.append(item1);
        item1 = new QStandardItem("=>");
        items.append(item1);
        item1 = new QStandardItem(hypothesis->get_sig1());
        items.append(item1);
        item1 = new QStandardItem(hypothesis->get_sig2());
        items.append(item1);
        item1 = new QStandardItem();
        item1->setData(hypothesis->get_number_of_words_saved(), Qt::DisplayRole);
        items.append(item1);
        appendRow(items);
    }
}
*/




void LxaStandardItemModel::sort_signatures(eSortStyle sort_style)
{
    if (sort_style==SIG_BY_STEM_COUNT){
        m_Signatures->sort(SIG_BY_AFFIX_COUNT);
    } else if (sort_style==SIG_BY_AFFIX_COUNT){
        m_Signatures->sort(SIG_BY_AFFIX_COUNT);
    }
    //qDebug() << "sort_signatures" << 178;
}


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

