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


void LxaStandardItemModel::sort(int column_no, Qt::SortOrder)
{
    if (column_no ==0  ){
        QStandardItemModel::sort(column_no);
    }
    else{

    }
}

LxaSortFilterProxyModel::LxaSortFilterProxyModel(QObject * parent) : QSortFilterProxyModel (parent)
{
    //nothing here.
}

bool LxaSortFilterProxyModel::lessThan(const QModelIndex & left, const QModelIndex & right) const
{   //int left_row, right_row;

    QVariant leftdata = sourceModel()->data(left);
    QVariant rightdata = sourceModel()->data(right);

    QString sig1 = leftdata.toString();
    QString sig2 = leftdata.toString();

   return sig1.split("=").length() > sig2.split("=").length();

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
        QStandardItem* pItem2 = new QStandardItem(QString::number(pWord->get_word_count()));
        item_list.append(pItem2);
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

        QStandardItem *item2 = new QStandardItem(QString::number(stem->get_count()));
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
        QStandardItem *item2 = new QStandardItem(QString::number(pSuffix->get_count()));
        QList<QStandardItem*> item_list;
        item_list.append(item);
        item_list.append(item2);
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
{   QList<sig_graph_edge*>               temp_list;
    int MINIMUM_NUMBER_OF_SHARED_WORDS = 3;
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
        if (size == 1 && p_sig_graph_edge->morph.length() > 1){
            continue;
        }else if (size == 2 && p_sig_graph_edge->morph.length()< 2){
            continue;
        }
        QStandardItem * item1 = new QStandardItem(p_sig_graph_edge->morph);
        QStandardItem * item2 = new QStandardItem(p_sig_graph_edge->m_sig_1->get_key());
        QStandardItem * item3 = new QStandardItem(QString::number(p_sig_graph_edge->m_sig_1->get_stem_entropy()));
        QStandardItem * item4 = new QStandardItem(p_sig_graph_edge->m_sig_2->get_key());
        QStandardItem * item5 = new QStandardItem(QString::number(p_sig_graph_edge->m_sig_2->get_stem_entropy()));
        QStandardItem * item6 = new QStandardItem(QString::number(p_sig_graph_edge->shared_word_stems.size()));
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

void remove_item_from_tree(const QString name, QStandardItem* item)
{
    for (int row_i = 0; row_i < item->rowCount(); ) {
        QString data = item->child(row_i, 0)->data(Qt::DisplayRole).toString();
        if (data == name) {
            item->removeRow(row_i);
        } else {
            row_i++;
        }
    }
}

void MainWindow::append_eval_results(EvaluationResults& results, QStandardItem* parent_item)
{
    typedef QStandardItem QSI;
    QList<QSI*> word_items;
    QSI* word_item = new QSI(QString("Gold Standard Words"));
    int gs_word_count = results.get_gs_word_count();
    QSI* word_count_item = new QSI(QString::number(gs_word_count));
    word_items.append(word_item);
    word_items.append(word_count_item);

    QList<QSI*> retrieved_word_items;
    QSI* retrieved_word_item = new QSI(QString("Retrieved Words"));
    int retrieved_word_count = results.get_retrieved_word_count();
    QSI* retrieved_word_count_item = new QSI(QString::number(retrieved_word_count));
    retrieved_word_items.append(retrieved_word_item);
    retrieved_word_items.append(retrieved_word_count_item);

    QList<QSI*> precision_items;
    QSI* precision_item = new QSI(QString("Precision"));
    double precision = results.get_total_precision();
    QSI* precision_value_item = new QSI(QString::number(precision));
    precision_items.append(precision_item);
    precision_items.append(precision_value_item);

    QList<QSI*> recall_items;
    QSI* recall_item = new QSI(QString("Recall"));
    double recall = results.get_total_recall();
    QSI* recall_value_item = new QSI(QString::number(recall));
    recall_items.append(recall_item);
    recall_items.append(recall_value_item);

    QList<QSI*> true_positive_items;
    QSI* true_positive_item = new QSI(QString("True Positive Parses"));
    int true_positive_count = results.get_true_positive_count();
    QSI* true_positive_count_item = new QSI(QString::number(true_positive_count));
    true_positive_items.append(true_positive_item);
    true_positive_items.append(true_positive_count_item);

    QList<QSI*> correct_items;
    QSI* correct_item = new QSI(QString("Gold Standard Parses"));
    int correct_count = results.get_gs_parse_count();
    QSI* correct_count_item = new QSI(QString::number(correct_count));
    correct_items.append(correct_item);
    correct_items.append(correct_count_item);

    QList<QSI*> retrieved_items;
    QSI* retrieved_item = new QSI(QString("Retrieved Parses"));
    int retrieved_count = results.get_retrieved_parse_count();
    QSI* retrieved_count_item = new QSI(QString::number(retrieved_count));
    retrieved_items.append(retrieved_item);
    retrieved_items.append(retrieved_count_item);

    parent_item->appendRow(word_items);
    parent_item->appendRow(retrieved_word_items);
    parent_item->appendRow(precision_items);
    parent_item->appendRow(recall_items);
    parent_item->appendRow(true_positive_items);
    parent_item->appendRow(correct_items);
    parent_item->appendRow(retrieved_items);
}

void MainWindow::update_TreeModel_for_gs(CLexicon* lexicon)
{
    typedef QStandardItem QSI;
    QSI* parent = m_treeModel->invisibleRootItem();
    int parent_row_count = parent->rowCount();
    QSI* curr_lexicon_item = parent->child(parent_row_count-1, 0);

    remove_item_from_tree("Gold Standard", curr_lexicon_item);

    QSI* gs_item = new QSI(QString("Gold Standard"));

    append_eval_results(lexicon->get_goldstandard()->get_results(), gs_item);

    curr_lexicon_item->appendRow(gs_item);
}

void MainWindow::update_TreeModel_for_eval(CLexicon *lexicon)
{
    if (lexicon->get_eval_parses() == NULL) {
        qDebug() << "MainWindow::update_TreeModel_for_eval: "
                    "EvalParses not loaded";
        return;
    }

    typedef QStandardItem QSI;
    QSI* parent = m_treeModel->invisibleRootItem();
    int parent_row_count = parent->rowCount();
    QSI* curr_lexicon_item = parent->child(parent_row_count-1, 0);

    QSI* eval_item = new QSI(QString("Morfessor Parses"));

    append_eval_results(lexicon->get_eval_parses()->get_results(), eval_item);

    curr_lexicon_item->appendRow(eval_item);
}

void MainWindow::create_or_update_TreeModel(CLexicon* lexicon)
{
    // possible take them out
     QStandardItem * parent = m_treeModel->invisibleRootItem();

     QStandardItem * command_item = new QStandardItem(QString("Keyboard commands"));

     QStandardItem * ctrl_1 = new QStandardItem(QString("Prefixes step 2"));
     QStandardItem * ctrl_1_key = new QStandardItem("Ctrl 1");

     QStandardItem * ctrl_2 = new QStandardItem(QString("Suffixes step 2"));
     QStandardItem * ctrl_2_key = new QStandardItem("Ctrl 2");

     QStandardItem * ctrl_3 = new QStandardItem(QString("Read project file"));
     QStandardItem * ctrl_3_key = new QStandardItem("Ctrl 3");

     QStandardItem * ctrl_4 = new QStandardItem(QString("Read corpus"));
     QStandardItem * ctrl_4_key = new QStandardItem("Ctrl 4");

     QStandardItem * ctrl_5 = new QStandardItem(QString("Sublexicon"));
     QStandardItem * ctrl_5_key = new QStandardItem("Ctrl 5");



    //  this pair of lines must stay here after experiment:
    QStandardItem * lexicon_item = new QStandardItem(QString("Lexicon"));
    QStandardItem * lexicon_count_item = new QStandardItem(QString("1"));

    QStandardItem * suffix_flag_item;
    if (lexicon->get_suffix_flag()){
        suffix_flag_item = new QStandardItem(QString("Suffixes"));
    } else {
        suffix_flag_item = new QStandardItem(QString("Prefixes"));
    }

    // will be eliminated by the experiment:
    QStandardItem * word_item = new QStandardItem(QString("Words"));
    QStandardItem * word_count_item = new QStandardItem(QString::number(lexicon->get_word_collection()->get_count()));

    QStandardItem * suffixal_stem_item = new QStandardItem(QString("Suffixal stems"));
    QStandardItem * suffixal_stem_count_item = new QStandardItem(QString::number(lexicon->get_suffixal_stems()->get_count()));

    QStandardItem * prefixal_stem_item = new QStandardItem(QString("Prefixal stems"));
    QStandardItem * prefixal_stem_count_item = new QStandardItem(QString::number(lexicon->get_prefixal_stems()->get_count()));


    QStandardItem * suffix_item = new QStandardItem(QString("Suffixes"));
    QStandardItem * suffix_count_item = new QStandardItem(QString::number(lexicon->get_suffixes()->get_count()));

    QStandardItem * sig_item = new QStandardItem(QString("Signatures"));
    QStandardItem * sig_count_item = new QStandardItem(QString::number(lexicon->get_signatures()->get_count()));

    QStandardItem * pos_sig_item = new QStandardItem(QString("EPositive signatures"));
    QStandardItem * pos_sig_count_item = new QStandardItem(QString::number(lexicon->get_signatures()->get_number_of_epositive_signatures()));

    QStandardItem * prefix_sig_item = new QStandardItem(QString("Prefix signatures"));
    QStandardItem * prefix_sig_count_item = new QStandardItem(QString::number(lexicon->get_prefix_signatures()->get_count()));

    QStandardItem * pos_prefix_sig_item = new QStandardItem(QString("EPositive prefix signatures"));
    QStandardItem * pos_prefix_sig_count_item = new QStandardItem(QString::number(lexicon->get_prefix_signatures()->get_number_of_epositive_signatures()));

    QStandardItem * residual_sig_item = new QStandardItem(QString("Residual parasignatures"));
    QStandardItem * residual_sig_count_item = new QStandardItem(QString::number(lexicon->get_residual_signatures()->get_count()));

    QStandardItem * parasuffix_item = new QStandardItem(QString("Parasuffixes"));
    QStandardItem * parasuffix_count_item = new QStandardItem(QString::number(lexicon->get_parasuffixes()->get_count()));

    QStandardItem * sig_graph_edge_item = new QStandardItem(QString("Signature graph edges"));
    QStandardItem * sig_graph_edge_count_item = new QStandardItem(QString::number(lexicon->get_sig_graph_edge_map()->size()));

    QStandardItem * passive_signature_item = new QStandardItem(QString("Passive signatures"));
    QStandardItem * passive_signature_count_item = new QStandardItem(QString::number(lexicon->get_passive_signatures()->get_count()));

    QStandardItem * hypothesis_item = new QStandardItem(QString("Hypotheses"));
    QStandardItem * hypothesis_count_item = new QStandardItem(QString::number(lexicon->get_hypotheses()->count()));

// This is part of an experiment:
//  This code deals with the components in the Lexicon, so that that set can be easily updated by the programmer.
//  it will eliminate above code and below code too:
//
//          QList<QStandardItem*>                   lexicon_items;
//                                                  lexicon_items.append(lexicon_item);
//                                                  lexicon_items.append(lexicon_count_item);
    //                                              parent->appendRow(lexicon_items);
    //      QMapIterator<QString,eComponentType>    iter (lexicon->get_category_types());
    //      while (iter.hasNext()){
    //
    //          QString             component_name = iter.next().key();
    //          eComponentType      this_component_type = iter.value();
    //          QList<QStandardItem*> this_list_of_standard_items;
    //
    //          QStandardItem *     component_item = new QStandardItem(component_name);
    //          QStandardItem *     component_count_item = new QStandardItem(component_count_name);
    //            this_list_of_standard_items.append(component_item);
    //            this_list_of_standard_items.append(component_count_item);
    //            lexicon_item.appendRow(this_list_of_standard_items);
    //    }
    // end of experiment




// add component 6



    QList<QStandardItem*> keyboard_1;
    keyboard_1.append(ctrl_1);
    keyboard_1.append(ctrl_1_key);

    QList<QStandardItem*> keyboard_2;
    keyboard_2.append(ctrl_2);
    keyboard_2.append(ctrl_2_key);

    QList<QStandardItem*> keyboard_3;
    keyboard_3.append(ctrl_3);
    keyboard_3.append(ctrl_3_key);

    QList<QStandardItem*> keyboard_4;
    keyboard_4.append(ctrl_4);
    keyboard_4.append(ctrl_4_key);

    QList<QStandardItem*> keyboard_5;
    keyboard_5.append(ctrl_5);
    keyboard_5.append(ctrl_5_key);



    QList<QStandardItem*> lexicon_items;
    lexicon_items.append(lexicon_item);
    lexicon_items.append(lexicon_count_item);

    QList<QStandardItem*> word_items;
    word_items.append(word_item);
    word_items.append(word_count_item);

    QList<QStandardItem*> suffixal_stem_items;
    suffixal_stem_items.append(suffixal_stem_item);
    suffixal_stem_items.append(suffixal_stem_count_item);

    QList<QStandardItem*> prefixal_stem_items;
    prefixal_stem_items.append(prefixal_stem_item);
    prefixal_stem_items.append(prefixal_stem_count_item);


    QList<QStandardItem*> suffix_items;
    suffix_items.append(suffix_item);
    suffix_items.append(suffix_count_item);

    QList<QStandardItem*> sig_items;
    sig_items.append(sig_item);
    sig_items.append(sig_count_item);

    QList<QStandardItem*> pos_sig_items;
    pos_sig_items.append(pos_sig_item);
    pos_sig_items.append(pos_sig_count_item);

    QList<QStandardItem*> prefix_sig_items;
    prefix_sig_items.append(prefix_sig_item);
    prefix_sig_items.append(prefix_sig_count_item);

    QList<QStandardItem*> pos_prefix_sig_items;
    pos_prefix_sig_items.append(pos_prefix_sig_item);
    pos_prefix_sig_items.append(pos_prefix_sig_count_item);

    QList<QStandardItem*> residual_sig_items;
    residual_sig_items.append(residual_sig_item);
    residual_sig_items.append(residual_sig_count_item);

    QList<QStandardItem*> parasuffix_items;
    parasuffix_items.append(parasuffix_item);
    parasuffix_items.append(parasuffix_count_item);

    QList<QStandardItem*> sig_graph_edge_items;
    sig_graph_edge_items.append(sig_graph_edge_item);
    sig_graph_edge_items.append(sig_graph_edge_count_item);

    QList<QStandardItem*> passive_signature_items;
    passive_signature_items.append(passive_signature_item);
    passive_signature_items.append(passive_signature_count_item);



    QList<QStandardItem*> hypothesis_items;
    hypothesis_items.append(hypothesis_item);
    hypothesis_items.append(hypothesis_count_item);

// add component 7

    parent->appendRow(lexicon_items);
    lexicon_item->appendRow(keyboard_1);
    lexicon_item->appendRow(keyboard_2);
    lexicon_item->appendRow(keyboard_3);
    lexicon_item->appendRow(keyboard_4);
    lexicon_item->appendRow(keyboard_5);
    lexicon_item->appendRow(suffix_flag_item);
    lexicon_item->appendRow(word_items);
    lexicon_item->appendRow(suffixal_stem_items);
    lexicon_item->appendRow(prefixal_stem_items);
    lexicon_item->appendRow(suffix_items);
    lexicon_item->appendRow(sig_items);
    lexicon_item->appendRow(pos_sig_items);
    lexicon_item->appendRow(prefix_sig_items);
    lexicon_item->appendRow(pos_prefix_sig_items);
    lexicon_item->appendRow(sig_graph_edge_items);
    lexicon_item->appendRow(residual_sig_items);
    lexicon_item->appendRow(parasuffix_items);
    lexicon_item->appendRow(passive_signature_items);
    lexicon_item->appendRow(hypothesis_items);
// add component 8
}
