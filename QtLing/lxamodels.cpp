#include "WordCollection.h"
#include "Word.h"
#include "mainwindow.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "QDebug"

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

void LxaStandardItemModel::load_signatures(CSignatureCollection* p_signatures)
{
    this->clear();
    m_Description = " ";
    CSignature*         sig;
    p_signatures->sort(SIG_BY_STEM_COUNT);
    m_sort_style = SIG_BY_STEM_COUNT;

    for (int signo = 0; signo<p_signatures->get_count(); signo++)
    {   sig = p_signatures->get_at_sorted(signo);
        QList<QStandardItem*> items;
        QStandardItem * item2 = new QStandardItem(QString::number(sig->get_number_of_stems()));
        QStandardItem * item3 = new QStandardItem(QString::number(sig->get_robustness()));
        items.append(new QStandardItem(sig->GetSignature()));
        items.append(item2);
        items.append(item3);
        appendRow(items);
    }
}

void LxaStandardItemModel::sort_signatures(eSortStyle sort_style)
{
    if (sort_style==SIG_BY_STEM_COUNT){

    } else if (sort_style==SIG_BY_AFFIX_COUNT){

    }

}



struct{
    bool operator ()(sig_tree_edge* a, sig_tree_edge* b) const {
     //   qDebug() << a->label()<< a->words.size() << b->label() << b->words.size();
     return a->shared_word_stems.size() > b->shared_word_stems.size();
    }
}custom_compare;
void LxaStandardItemModel::load_sig_tree_edges( QMap<QString, sig_tree_edge*> * this_sig_tree_edge_map )
{   QList<sig_tree_edge*>               temp_list;
    QMapIterator<word_t, sig_tree_edge*> * this_sig_tree_edge_iter = new QMapIterator<word_t, sig_tree_edge*>( * this_sig_tree_edge_map );
    while (this_sig_tree_edge_iter->hasNext())    {
        this_sig_tree_edge_iter->next();
       //qDebug() << this_sig_tree_edge_iter->value()->words.size();
        temp_list.append(this_sig_tree_edge_iter->value());
    }
    std::sort( temp_list.begin(),  temp_list.end(), custom_compare);
    QListIterator<sig_tree_edge*> temp_list_iter (temp_list);
    while (temp_list_iter.hasNext())
     {
        sig_tree_edge * p_sig_tree_edge = temp_list_iter.next();
        QList<QStandardItem*> items;
        QStandardItem * item1 = new QStandardItem (p_sig_tree_edge->morph);
        items.append(item1);

        QStandardItem * item2 = new QStandardItem(p_sig_tree_edge->sig_1->get_key());
        QStandardItem * item3 = new QStandardItem(p_sig_tree_edge->sig_2->get_key());
        QStandardItem * item4 = new QStandardItem(QString::number(p_sig_tree_edge->shared_word_stems.size()));
        QStandardItem * item5 = new QStandardItem(p_sig_tree_edge->label());
        items.append(item2);
        items.append(item3);
        items.append(item4);
        items.append(item5);

        appendRow(items);
     }


}

