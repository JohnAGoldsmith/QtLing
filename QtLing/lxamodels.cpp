#include <QPair>
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
#include <QCoreApplication>

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

void LxaStandardItemModel::load_words(CWordCollection* p_words)
{
    QStringList labels;
    clear();
    labels  << tr("word") << "word count" << "signatures";
    setHorizontalHeaderLabels(labels);
    m_Description = QString (" ");
    QMapIterator<word_t, CWord*> word_iter ( * p_words->get_map() );
    while (word_iter.hasNext())
    {   word_iter.next();
        CWord* pWord = word_iter.value();
        word_t this_word = word_iter.key();
        QList<QStandardItem*> item_list;
        QStandardItem* pItem = new QStandardItem(this_word);
        item_list.append(pItem);
        // changed here: let data have type int //
        QStandardItem* pItem2 = new QStandardItem();
        pItem2->setData(pWord->get_word_count(), Qt::DisplayRole);
        item_list.append(pItem2);
        // changed here //
        QMapIterator<stem_t, Parse_triple*> parse_3_iter(*pWord->get_parse_triple_map());
        int tempcount = 0;
        while (parse_3_iter.hasNext()){
            QStandardItem* pItem3 = new QStandardItem(parse_3_iter.next().value()->p_sig_string) ;
            item_list.append(pItem3);
            tempcount++;
        }
        appendRow(item_list);
    }
}

void LxaStandardItemModel::load_compounds(CompoundWordCollection *p_compounds)
{
    typedef QStandardItem QSI;
    typedef CompoundWord::CompoundComposition CompoundComposition;

    QStringList labels;
    labels << "Compound word" << "Possible Compositions";
    setHorizontalHeaderLabels(labels);

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
        item2->setData(word_count, Qt::DisplayRole);
        item_list.append(item2);
        appendRow(item_list);
    }
}

void LxaStandardItemModel::load_stems(CStemCollection * p_stems)
{
    CStem*                          stem;
    QMapIterator<QString, CStem*>  iter ( * p_stems->get_map() ) ;

    clear();
    while (iter.hasNext())
    {
        QList<QStandardItem*> item_list;

        stem = iter.next().value();
        QStandardItem *item = new QStandardItem(stem->get_key());
        item_list.append(item);

        QStandardItem *item2 = new QStandardItem();
        item2->setData(stem->get_count(), Qt::DisplayRole);
        // changed: let data have type int //
        item_list.append(item2);

        QListIterator<CSignature*> sig_iter(*stem->GetSignatures());
        while (sig_iter.hasNext()){
           sigstring_t sig = sig_iter.next()->get_key();
           QStandardItem *item = new QStandardItem(sig);
           item_list.append(item);
        }
        appendRow(item_list);
    }
}


void LxaStandardItemModel::load_suffixes(CSuffixCollection * p_suffixes)
{
    clear();
    //map_string_to_suffix_ptr_iter suffix_iter(*p_suffixes->get_map());
    CSuffix_ptr_list_iterator suffix_iter(*p_suffixes->get_sorted_list());
    while (suffix_iter.hasNext())
    {
        CSuffix* pSuffix = suffix_iter.next();
        QStandardItem *item = new QStandardItem(pSuffix->GetSuffix());
        QStandardItem *item2 = new QStandardItem();
        item2->setData(pSuffix->get_count(), Qt::DisplayRole);
        QList<QStandardItem*> item_list;
        item_list.append(item);
        item_list.append(item2);
        appendRow(item_list);
    }
}
void LxaStandardItemModel::load_prefixes(CPrefixCollection * p_prefixes)
{
    clear();
    //map_string_to_suffix_ptr_iter suffix_iter(*p_suffixes->get_map());

    double totalcount = 0;
    CPrefix_ptr_list_iterator prefix_iter1(*p_prefixes->get_sorted_list());
    while (prefix_iter1.hasNext())
    {
        CPrefix* pPrefix = prefix_iter1.next();
        totalcount += pPrefix->get_count();
    }

    CPrefix_ptr_list_iterator prefix_iter(*p_prefixes->get_sorted_list());
    while (prefix_iter.hasNext())
    {
        CPrefix* pPrefix = prefix_iter.next();
        QStandardItem *item = new QStandardItem(pPrefix->GetPrefix());
        QStandardItem *item2 = new QStandardItem();
        item2->setData(pPrefix->get_count(), Qt::DisplayRole);
        QStandardItem *item3 = new QStandardItem();
        item3->setData(pPrefix->get_count()/totalcount, Qt::DisplayRole);
        QList<QStandardItem*> item_list;
        item_list.append(item);
        item_list.append(item2);
        item_list.append(item3);
        appendRow(item_list);
    }
}
void LxaStandardItemModel::load_signatures(CSignatureCollection* p_signatures, eSortStyle this_sort_style)
{
    clear();
    m_Signatures = p_signatures;
    CSignature*         sig;
    p_signatures->sort(this_sort_style);
    m_sort_style = this_sort_style;

    QStringList labels;
    labels  << tr("signature") << "stem count" << "robustness"<< "fullness";
    setHorizontalHeaderLabels(labels);

    //qDebug() << 133 << "number of signatures"<< p_signatures->get_count() <<  "in Models file";
    for (int signo = 0; signo<p_signatures->get_count(); signo++)
    {
        sig = p_signatures->get_at_sorted(signo);
        QList<QStandardItem*> items;

        const QString& str_sig = sig->GetSignature();
        QStandardItem * item1 = new QStandardItem(str_sig);
        QStandardItem * item2 = new QStandardItem();
        QStandardItem * item3 = new QStandardItem();
        QStandardItem * item4 = new QStandardItem();
        item2->setData(sig->get_number_of_stems(), Qt::DisplayRole);
        item3->setData(sig->get_robustness(), Qt::DisplayRole);
        item4->setData(sig->get_stem_entropy(), Qt::DisplayRole);

        items.append(item1);
        items.append(item2);
        items.append(item3);
        items.append(item4);
        appendRow(items);
    }
}
void LxaStandardItemModel::load_positive_signatures(CSignatureCollection* p_signatures, eSortStyle this_sort_style)
{
    clear();
    QStringList labels;
    labels  << tr("signature") << "stem count" << "robustness"<< "fullness";
    setHorizontalHeaderLabels(labels);
    m_Signatures = p_signatures;
    m_Description = "positive suffix signatures";
    CSignature*         sig;
    p_signatures->sort(this_sort_style);
    m_sort_style = this_sort_style;
    double threshold = p_signatures->get_lexicon()->get_entropy_threshold_for_positive_signatures();
    for (int signo = 0; signo<p_signatures->get_count(); signo++)
    {   sig = p_signatures->get_at_sorted(signo);
        if (sig->get_stem_entropy() < threshold){continue;}

        QList<QStandardItem*> items;
        QStandardItem * item2 = new QStandardItem();
        QStandardItem * item3 = new QStandardItem();
        QStandardItem * item4 = new QStandardItem();
        item2->setData(sig->get_number_of_stems(), Qt::DisplayRole);
        item3->setData(sig->get_robustness(), Qt::DisplayRole);
        item4->setData(sig->get_stem_entropy(), Qt::DisplayRole);

        items.append(new QStandardItem(sig->GetSignature()));
        items.append(item2);
        items.append(item3);
        items.append(item4);
        appendRow(items);




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
        item3->setData(sig->get_robustness(), Qt::DisplayRole);
        items.append(item1);
        items.append(item3);
        items.append(new QStandardItem(sig->GetSignature()));
        appendRow(items);
    }
}

bool sort_function_1(const QPair<QString ,int >  * a ,  QPair<QString , int >  * b)
{
    if (a->second==b->second){return a->first < b->first;}
    else{
        return a->second > b->second;
    }
};
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
        item1->setData(pPair->second, Qt::DisplayRole);
        items.append(item1);

        QString this_key = pPair->first;
        QStringList key_list = this_key.split("@");
        foreach (QString key, key_list){
            QStandardItem * this_item = new QStandardItem(key);
            items.append(this_item);
        }
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
/*
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
        QStandardItem* item1 = new QStandardItem(QString::number(pPair->second));
        items.append(item1);

        QString this_key = pPair->first;
        QStringList key_list = this_key.split("@");
        foreach (QString key, key_list){
            QStandardItem * this_item = new QStandardItem(key);
            items.append(this_item);
        }
        appendRow(items);
    }
*/
    QStringList labels;
    labels  <<  "rule input"<<"" << " rule output (orig sig 2)" << "original sig 1"
             << "modified sig 1"
             << "word count";
    setHorizontalHeaderLabels(labels);


    for (int hypno = 0; hypno<p_hypotheses->count(); hypno++)
    {   hypothesis = p_hypotheses->at(hypno);
        QList<QStandardItem*> items;
        QStringList pieces = hypothesis->express();
        pieces << hypothesis->express_as_string();
        //qDebug() << 316 <<"lxamodels"<< pieces;
        for (int i = 0; i < pieces.count(); i++){
             QStandardItem * item1 = new QStandardItem(pieces[i]);
             items.append(item1);
        }
        appendRow(items);
    }
}



// add component 12



void LxaStandardItemModel::sort_signatures(eSortStyle sort_style)
{
    if (sort_style==SIG_BY_STEM_COUNT){
        m_Signatures->sort(SIG_BY_AFFIX_COUNT);
    } else if (sort_style==SIG_BY_AFFIX_COUNT){
        m_Signatures->sort(SIG_BY_AFFIX_COUNT);
    }
    //qDebug() << "sort_signatures" << 178;
}



struct{
    bool operator ()(sig_graph_edge* a, sig_graph_edge* b) const {
     return a->shared_word_stems.size() > b->shared_word_stems.size();
    }
}custom_compare;
struct{
    bool operator ()(sig_graph_edge* a, sig_graph_edge* b) const
    {
        if (a->morph == b->morph){
            if (a->m_sig_1 == b->m_sig_1){
                return a->m_sig_2 < b->m_sig_2;
            }else
            {
                return a->m_sig_1 < b->m_sig_1;
            }
        }  else
        {
            return a->morph < b->morph;
        }
    }
}custom_compare_2;

void LxaStandardItemModel::load_sig_graph_edges( QMap<QString, sig_graph_edge*> * this_sig_graph_edge_map, int size )
{
    QList<sig_graph_edge*>               temp_list;
    int MINIMUM_NUMBER_OF_SHARED_WORDS = 3;
    QMapIterator<word_t, sig_graph_edge*> * this_sig_graph_edge_iter
            = new QMapIterator<word_t, sig_graph_edge*>( * this_sig_graph_edge_map );
    while (this_sig_graph_edge_iter->hasNext())    {
        this_sig_graph_edge_iter->next();

        if (this_sig_graph_edge_iter->value()->shared_word_stems.count() >= MINIMUM_NUMBER_OF_SHARED_WORDS) {
            temp_list.append(this_sig_graph_edge_iter->value());
        }
    }
    std::sort( temp_list.begin(),  temp_list.end(), custom_compare_2);
    QListIterator<sig_graph_edge*> temp_list_iter (temp_list);
    while (temp_list_iter.hasNext())
     {
        sig_graph_edge * p_sig_graph_edge = temp_list_iter.next();
        if (size == 1 && p_sig_graph_edge->morph.length() > 1){
            continue;
        }else if (size == 2 && p_sig_graph_edge->morph.length()< 2){
            continue;
        }
        QStandardItem * item1 = new QStandardItem(p_sig_graph_edge->morph);
        QStandardItem * item2 = new QStandardItem(p_sig_graph_edge->m_sig_string_1); // changed here
        QStandardItem * item3 = new QStandardItem();
        if (p_sig_graph_edge->m_sig_1)
            item3->setData(p_sig_graph_edge->m_sig_1->get_stem_entropy(), Qt::DisplayRole);
        else
            item3->setData("N/A", Qt::DisplayRole);
        QStandardItem * item4 = new QStandardItem(p_sig_graph_edge->m_sig_string_2);
        QStandardItem * item5 = new QStandardItem();
        if (p_sig_graph_edge->m_sig_2)
            item5->setData(p_sig_graph_edge->m_sig_2->get_stem_entropy(), Qt::DisplayRole);
        else
            item5->setData("N/A", Qt::DisplayRole);
        QStandardItem * item6 = new QStandardItem();
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
    GoldStandard::ParseMap::ConstIterator gs_iter, pm_iter;
    GoldStandard::Parse_triple_map::ConstIterator ptm_iter;
    ParseMapHandler p_all_word_gsm = p_gs->get_gs_parses();

    for (gs_iter = p_all_word_gsm->constBegin(); gs_iter != p_all_word_gsm->constEnd(); gs_iter++) {
        QList<QSI*> items;
        QString this_word = gs_iter.key();
        QSI* word_item = new QSI(this_word);
        items.append(word_item);
        pm_iter = parsemap->find(this_word);
        if (pm_iter != parsemap->constEnd()) {
            GoldStandard::Parse_triple_map* ptm = pm_iter.value();
            for (ptm_iter = ptm->constBegin(); ptm_iter != ptm->constEnd(); ptm_iter++) {
                Parse_triple* this_pt = ptm_iter.value();
                QString this_parse = this_pt->p_stem + "=" + this_pt->p_suffix;
                items.append(new QStandardItem(this_parse));
            }
        }
        appendRow(items);
    }
}

