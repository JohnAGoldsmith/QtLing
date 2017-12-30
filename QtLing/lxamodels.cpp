#include <QPair>
#include "WordCollection.h"
#include "Word.h"
#include "mainwindow.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "QDebug"
#include "hypothesis.h"

LxaStandardItemModel::LxaStandardItemModel()
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


void LxaStandardItemModel::sort(int column_no, Qt::SortOrder order)
{
    if (column_no ==0  ){
        QStandardItemModel::sort(column_no);
    }
    else{

    }
}



void LxaStandardItemModel::load_words(CWordCollection* p_words)
{
    this->clear();
    m_Description = QString (" ");
    QMapIterator<word_t, CWord*> word_iter ( * p_words->get_map() );
    while (word_iter.hasNext())
    {   word_iter.next();
        CWord* pWord = word_iter.value();
        word_t this_word = word_iter.key();
        QList<QStandardItem*> item_list;

        QStandardItem* pItem = new QStandardItem(this_word);
        item_list.append(pItem);

        QStandardItem* pItem2 = new QStandardItem(QString::number(pWord->get_word_count()));
        item_list.append(pItem2);

        QListIterator<QPair<CStem*,CSignature*>*> sig_iter(*pWord->GetSignatures());
        while (sig_iter.hasNext()){
            QStandardItem* pItem3 = new QStandardItem(sig_iter.next()->second->GetSignature());
            item_list.append(pItem3);
        }
        appendRow(item_list);
    }
}


void LxaStandardItemModel::load_stems(CStemCollection * p_stems)
{
    CStem*                          stem;
    QMapIterator<QString, CStem*>  iter ( * p_stems->get_map() ) ;

    this->clear();
    while (iter.hasNext())
    {
        stem = iter.next().value();
        QStandardItem *item = new QStandardItem(stem->get_key());
        QList<QStandardItem*> item_list;
        item_list.append(item);
        QListIterator<QString> sig_iter(*stem->GetSignatures());
        while (sig_iter.hasNext()){
           sigstring_t sig = sig_iter.next();
           QStandardItem *item = new QStandardItem(sig);
           item_list.append(item);
        }
        appendRow(item_list);
    }
}


void LxaStandardItemModel::load_suffixes(CSuffixCollection * p_suffixes)
{
    this->clear();
    //map_string_to_suffix_ptr_iter suffix_iter(*p_suffixes->get_map());
    CSuffix_ptr_list_iterator suffix_iter(*p_suffixes->get_sorted_list());
    while (suffix_iter.hasNext())
    {
        CSuffix* pSuffix = suffix_iter.next();
        QStandardItem *item = new QStandardItem(pSuffix->GetSuffix());
        QStandardItem *item2 = new QStandardItem(QString::number(pSuffix->get_count()));
        QList<QStandardItem*> item_list;
        item_list.append(item);
        item_list.append(item2);
        appendRow(item_list);
    }
}
void LxaStandardItemModel::load_signatures(CSignatureCollection* p_signatures, eSortStyle this_sort_style)
{
    this->clear();
    m_Signatures = p_signatures;
    m_Description = "suffix signatures";
    CSignature*         sig;
    p_signatures->sort(this_sort_style);
    m_sort_style = this_sort_style;

    for (int signo = 0; signo<p_signatures->get_count(); signo++)
    {   sig = p_signatures->get_at_sorted(signo);
        QList<QStandardItem*> items;
        QStandardItem * item2 = new QStandardItem(QString::number(sig->get_number_of_stems()));
        QStandardItem * item3 = new QStandardItem(QString::number(sig->get_robustness()));
        QStandardItem * item4 = new QStandardItem(QString::number(sig->get_stem_entropy()));
        items.append(new QStandardItem(sig->GetSignature()));
        items.append(item2);
        items.append(item3);
        items.append(item4);
        appendRow(items);
    }
}




void LxaStandardItemModel::load_positive_signatures(CSignatureCollection* p_signatures)
{
    this->clear();
    m_Signatures = p_signatures;
    m_Description = "positive suffix signatures";
    CSignature*         sig;
    p_signatures->sort(SIG_BY_STEM_COUNT);
    m_sort_style = SIG_BY_STEM_COUNT;
    double threshold = p_signatures->get_lexicon()->get_entropy_threshold_for_positive_signatures();
    for (int signo = 0; signo<p_signatures->get_count(); signo++)
    {   sig = p_signatures->get_at_sorted(signo);
        if (sig->get_stem_entropy() < threshold){continue;}
        QList<QStandardItem*> items;
        QStandardItem * item2 = new QStandardItem(QString::number(sig->get_number_of_stems()));
        QStandardItem * item3 = new QStandardItem(QString::number(sig->get_robustness()));
        QStandardItem * item4 = new QStandardItem(QString::number(sig->get_stem_entropy()));
        items.append(new QStandardItem(sig->GetSignature()));
        items.append(item2);
        items.append(item3);
        items.append(item4);
        appendRow(items);
    }
}
void LxaStandardItemModel::load_parasignatures(CSignatureCollection* p_signatures)
{
    this->clear();
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
        QStandardItem * item3 = new QStandardItem(QString::number(sig->get_robustness()));
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
    this->clear();
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

    for (int hypno = 0; hypno<p_hypotheses->count(); hypno++)
    {   hypothesis = p_hypotheses->at(hypno);
        QList<QStandardItem*> items;
        QStringList pieces = hypothesis->express();
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
    qDebug() << "sort_signatures" << 178;
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
            if (a->sig_1 == b->sig_1){
                return a->sig_2 < b->sig_2;
            }else
            {
                return a->sig_1 < b->sig_1;
            }
        }  else
        {
            return a->morph < b->morph;
        }
    }
}custom_compare_2;

void LxaStandardItemModel::load_sig_graph_edges( QMap<QString, sig_graph_edge*> * this_sig_graph_edge_map )
{   QList<sig_graph_edge*>               temp_list;
    int MINIMUM_NUMBER_OF_SHARED_WORDS = 1;
    QMapIterator<word_t, sig_graph_edge*> * this_sig_graph_edge_iter = new QMapIterator<word_t, sig_graph_edge*>( * this_sig_graph_edge_map );
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
        QList<QStandardItem*> items;
        QStandardItem * item1 = new QStandardItem (p_sig_graph_edge->morph);
        items.append(item1);

        QStandardItem * item2 = new QStandardItem(p_sig_graph_edge->sig_1->get_key());
        QStandardItem * item3 = new QStandardItem(QString::number(p_sig_graph_edge->sig_1->get_stem_entropy()));
        QStandardItem * item4 = new QStandardItem(p_sig_graph_edge->sig_2->get_key());
        QStandardItem * item5 = new QStandardItem(QString::number(p_sig_graph_edge->shared_word_stems.size()));
        QStandardItem * item6 = new QStandardItem(p_sig_graph_edge->label());
        items.append(item2);
        items.append(item3);
        items.append(item4);
        items.append(item5);
        items.append(item6);
        appendRow(items);
     }


};

