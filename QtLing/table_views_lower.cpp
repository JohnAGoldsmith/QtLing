#include <QDebug>
#include <QMap>
#include <QFont>
#include <QHeaderView>
#include "Lexicon.h"
#include "mainwindow.h"
#include "Word.h"
#include "WordCollection.h"
#include "graphics.h"
#include "lxamodels.h"
#include "compound.h"
class sig_graph_edge;

/**
 * @brief LowerTableView::LowerTableView
 * This is the view on the lower right part of the screen.
 * It usually gives additional information when the user clicks on the Upper Table View.
 */
LowerTableView::LowerTableView()
{
   m_my_current_model =new  QStandardItemModel();
   m_number_of_columns = 8;
   QFont sansFont("Ariel", 20);
   setFont(sansFont);


}
LowerTableView::LowerTableView(MainWindow * window)
{
   m_my_current_model =new  QStandardItemModel();
   m_parent_window = window;
   m_number_of_columns = 8;
   m_lexicon = window->get_lexicon();
   m_current_sortstyle = 0;
   QFont sansFont("Ariel", 20);
   setFont(sansFont);

}
/**
  * @brief LowerTableView::display_this_item
  * @param index
  * This is the normal way to display something in this view. It starts with information
  * from the user clicking the upper TableView. The UpperTable may be display many different
  * kinds of things, so this function is broken up into pieces, each depending on what
  * the Upper TableView is currently displaying.
  * NB: This View can either by a Table or a Graphical View.
  * The choice of these two options is controlled by keypress: Ctrl-G (for Graphics).
  */
void LowerTableView::display_this_item( const  QModelIndex & index )
{
    eDataType                  UpperView_data_type = m_parent_window->m_tableView_upper_left->get_data_type();
    QString                    word, stem, prefix, suffix, signature;
    CLexicon *                 this_lexicon = get_parent_window()->get_lexicon();
    int                        row(0), column;
    //QStandardItem*             p_item;
    QList<QStandardItem*>      item_list;

    switch (UpperView_data_type){
    case e_data_words:{
        if (index.isValid()) {row = index.row();}
        QString word = index.sibling(row,0).data().toString();
        CWord* pWord = this_lexicon->get_words()->get_word(word);
        table_word(pWord);
        setModel( m_my_current_model);
        break;
    }
    case e_data_stems:
    case e_suffixal_stems:
    case e_prefixal_stems:{
        if (index.isValid()) {row = index.row();}
        QString stem = index.sibling(row,0).data().toString();
        table_stem(stem, this_lexicon);
        setModel( m_my_current_model);
        break;
    }
        //  ---------------------------------------------------//
    case  e_data_suffixal_signatures:
    case  e_data_epositive_suffixal_signatures:{
        if (index.isValid()) {row = index.row();}
        signature = index.sibling(row,0).data().toString();
        CSignature*  pSig = this_lexicon->get_signatures()->get_signature(signature);
        table_signature(pSig);
        setModel( m_my_current_model);
        break;}
        //  ---------------------------------------------------//
    case e_data_prefixal_signatures:
    case e_data_epositive_prefixal_signatures:{
        item_list.clear();
        if (index.isValid()) {row = index.row();}
        signature = index.sibling(row,0).data().toString();
        CSignature*           pSig = this_lexicon->get_prefix_signatures()->get_signature(signature);
        table_signature(pSig);
        setModel( m_my_current_model);
        break;}
        //  ---------------------------------------------------//
    case e_data_residual_signatures:{
        if (index.isValid()){row = index.row();}
        item_list.clear();
        signature = index.sibling(row,0).data().toString();
        CSignature*           pSig = this_lexicon->get_signatures()->get_signature(signature);
        CStem*                p_Stem;
        CStem_ptr_list     *  sig_stems = pSig->get_stems();
        QStandardItem*        p_item;
        if (m_my_current_model) {
            delete m_my_current_model;
        }
        m_my_current_model = new QStandardItemModel();
        foreach (p_Stem, *sig_stems)  {
            p_item = new QStandardItem(p_Stem->get_key() );
            item_list.append(p_item);
            if (item_list.length() >= m_number_of_columns){
                m_my_current_model->appendRow(item_list);
                item_list.clear();
            }
        }
        setModel( m_my_current_model);
        break;}
        //  ---------------------------------------------------//
    case e_data_hollow_signatures:{
        item_list.clear();
        if (index.isValid()){
            row = index.row();
            column = index.column();
        }
        if (m_my_current_model) {
            delete m_my_current_model;
        }
        m_my_current_model = new QStandardItemModel();
        sig_string sig = index.sibling(row,0).data().toString();
        CSignature* pSig;
        this_lexicon->get_suffix_flag()?
                    pSig = this_lexicon->get_signatures()->get_signature(sig):
                pSig = this_lexicon->get_prefix_signatures()->get_signature(sig);
        table_passive_signature(pSig);
        setModel( m_my_current_model);
        break;}
        //  ---------------------------------------------------//
    case e_data_suffixes:{
        break;
    }
        //  ---------------------------------------------------//

    case  e_data_signatures_graph_edges:{
        item_list.clear();
        if (index.isValid()){
            row = index.row();
            column = index.column();
        }
        QString               edge_key = index.sibling(row,6).data().toString();
        sig_graph_edge*       this_edge = this_lexicon->get_sig_graph_edge_map()->value(edge_key);
        //sig_graph_edge *      psig_graph_edge;
        CSignature*           pSig;
        CSignature*           pSig1 = this_edge->m_sig_1;
        CSignature*           pSig2 = this_edge->m_sig_2;
        QStandardItem*        p_item;
        //CStem*                p_Stem;
        QStringList           sig1_stems;
        QStringList           sig2_stems;
        QStringList           words;
        word_stem_struct *    this_word_stem_item;


        if (m_parent_window->m_graphic_display_flag){
            //-->  Graphic display in lower right window <--//
            if (column == 1){
                pSig = pSig1;
            } else { pSig = pSig2;}
            graphics_sig_graph_edges(pSig, this_lexicon );
        } else
            // -->   Tabular display in lower right window <--//
        {   foreach (this_word_stem_item, this_edge->shared_word_stems){
                words.append(     this_word_stem_item->word);
                sig1_stems.append(this_word_stem_item->stem_1);
                sig2_stems.append(this_word_stem_item->stem_2);
            }
            if (m_my_current_model) {
                delete m_my_current_model;
            }
            m_my_current_model = new QStandardItemModel();
            // --> first signature <-- //
            table_one_signature(pSig1, sig1_stems );
            // --> second signature <-- //
            table_one_signature(pSig2, sig2_stems );
            // --> words <-- //
            m_my_current_model->appendRow(item_list);    // blank row in table.
            item_list.clear();
            m_my_current_model->appendRow(item_list);
            item_list.clear();
            for (int wordno= 0; wordno< words.size();wordno++)  {
                p_item = new QStandardItem(words[wordno]);
                item_list.append(p_item);
                if (item_list.length() >= m_number_of_columns){
                    m_my_current_model->appendRow(item_list);
                    item_list.clear();
                }
            }
            if (item_list.size() > 0 ){
                m_my_current_model->appendRow(item_list);
                item_list.clear();
            }
            // --> end of words <-- //
            setModel( m_my_current_model);
            resizeColumnsToContents();
        } // end of tabular display, lower right window
        break;}
    case  e_data_hypotheses:
        if (m_parent_window->m_graphic_display_flag){
            if (index.isValid()) {row = index.row();}
            QString hypothesis_label = index.sibling(row,6).data().toString();
        }
        else{ //tabular info, not graphics.

        }
        break;

        // add component 9
        if (m_parent_window->m_graphic_display_flag){
            if (index.isValid()) {row = index.row();}
            QString hypothesis_label = index.sibling(row,6).data().toString();
            //CHypothesis*  pHypothesis = this_lexicon->get_hypothesis(hypothesis_label);
            qDebug() << 207 << hypothesis_label;
        }
        else{ //tabular info, not graphics.

        }
        break;

        // add component 9
    case e_data_suffixal_protostems:
    case e_data_prefixal_protostems: {
        if (index.isValid()) {row = index.row();}
        QString str_protostem = index.sibling(row,0).data().toString();
        protostem* p_protostem = UpperView_data_type == e_data_suffixal_protostems ?
                    this_lexicon->get_suffixal_protostems()->value(str_protostem):
                    this_lexicon->get_prefixal_protostems()->value(str_protostem);
        table_protostem(p_protostem);
        setModel(m_my_current_model);
        break;
    }

    case e_data_compound_words: {
        if (index.isValid()) {
            row = index.row();
            column = index.column();
            if (column == 0)
                column = 1;
        }
        QString str_compound = index.sibling(row, 0).data().toString();
        CompoundWord* p_compound = this_lexicon->get_compounds()
                ->get_compound_word(str_compound);
        if (p_compound == NULL) {
            qDebug() << "LowerTableView::display_this_item: "
                        "Cannot find compound word!";
            break;
        }
        table_compound_composition(p_compound, column-1);
        setModel(m_my_current_model);
        break;

    }

    default:
        break;
    }


    resizeColumnsToContents();
}
//------------------->         <---------------------------------------------------//
 /**
  * @brief LeftSideTreeView::LeftSideTreeView
  * @param window
  *
  */
 LeftSideTreeView::LeftSideTreeView(MainWindow* window)
 {
     m_parent_window = window;
     setEditTriggers(QAbstractItemView::NoEditTriggers);
 }



/**
 * @brief LowerTableView::graphics_sig_graph_edges
 * @param pSig
 * @param p_lexicon
 *
 *
 */
void LowerTableView::graphics_sig_graph_edges(CSignature* pSig, CLexicon* p_lexicon)
{
    sig_graph_edge *       psig_graph_edge;
    //-->  Graphic display in lower right window <--//
    // "Signatures" is what should be sent to the Model in the mainwindow.
    CSignatureCollection Signatures(p_lexicon);
    Signatures << pSig;
    // We iterate through the SigTreeEdges in the SigTreeEdgeMap. If its Sig1 is pSig, then we enter it into Signatures;
    QMap<QString,sig_graph_edge*>::iterator edge_iter = p_lexicon->get_sig_graph_edge_map()->begin();
    while (edge_iter !=  p_lexicon->get_sig_graph_edge_map()->constEnd()){
        psig_graph_edge = edge_iter.value();
        if (psig_graph_edge->m_sig_1 == pSig){
            Signatures << psig_graph_edge->m_sig_2;
        }
        ++edge_iter;
    }

    // put this back in when things are figured out **** BUT we don't want to get rid of old graphics scene, just
    // clear it out. The old one is still connected with a signal / slot connection.
//    m_parent_window->m_graphics_scene = new lxa_graphics_scene(m_parent_window,  &Signatures,  DT_sig_graph_edges);
//    m_parent_window->m_graphics_scene->place_signatures();
//    m_parent_window->m_graphics_view->setScene(m_parent_window->m_graphics_scene);
}

/**
 * @brief LowerTableView::table_word
 * @param pWord
 * What follows is a set of functions that display different kinds of user-requested information on the Lower Table View.
 */

void LowerTableView::table_word(CWord* pWord){
    QList<QStandardItem*>      item_list;
    QStandardItem *            p_item, *q_item;
    QString                     word_t = pWord->get_key();

    // Create a clean model.
    if (m_my_current_model){
        delete m_my_current_model;
    }
    m_my_current_model = new QStandardItemModel();
    /*
    // Find the word's autobiography and set it, line by line, in the lower TableView.
    QListIterator<QString> line_iter(*pWord->get_autobiography());
    while (line_iter.hasNext()){
        QString report_line = line_iter.next();
        item_list.clear();
        QStringList report_line_items = report_line.split("=");
        for (int i = 0; i < report_line_items.size(); i++){
            p_item = new QStandardItem(report_line_items[i]);
            if (i == 0 && report_line_items[i][0] == "*"){
                p_item->setBackground(Qt::red);
            } else{
                p_item->setBackground(Qt::white);
            }
            item_list.append(p_item);
        }
        m_my_current_model->appendRow(item_list);
    }
    // Display the signatures this word bears:
    for (int signo = 0; signo < pWord->m_Signatures.size(); signo++){
        QString sig = pWord->get_signatures()->at(signo)->second->get_key();
        QString stem = pWord->get_signatures()->at(signo)->first->get_key();
        item_list.clear();
        p_item = new QStandardItem(stem);
        item_list.append(p_item);
        q_item = new QStandardItem(sig);
        item_list.append(q_item);
        m_my_current_model->appendRow(item_list);
    }
    */
    CLexicon* lexicon = m_lexicon;
    if (lexicon->word_autobiographies_contains(word_t)) {
        //qDebug() << 339 << "true";
        QListIterator<QString> line_iter(*(lexicon->get_word_autobiography(word_t)));
        while (line_iter.hasNext()){
            QString report_line = line_iter.next();
            item_list.clear();
            QStringList report_line_items = report_line.split("=");
            for (int i = 0; i < report_line_items.size(); i++){
                p_item = new QStandardItem(report_line_items[i]);
                if (i == 0 && report_line_items[i][0] == "*"){
                    p_item->setBackground(Qt::red);
                } else{
                    p_item->setBackground(Qt::white);
                }
                item_list.append(p_item);

            }
            m_my_current_model->appendRow(item_list);
        }
    }

}

/**
 * @brief LowerTableView::table_word
 * @param pWord
 * What follows is a set of functions that display different kinds of user-requested information on the Lower Table View.
 */
/*
void LowerTableView::table_word(QString word, CLexicon* Lexicon){
    QList<QStandardItem*>      item_list;
    QStandardItem *            p_item, *q_item;

    // Create a clean model.
    if (m_my_current_model){
        delete m_my_current_model;
    }
    m_my_current_model = new QStandardItemModel();

    // Find the word's autobiography and set it, line by line, in the lower TableView.
    if (Lexicon->word_autobiographies_contains(word)){
        QListIterator<QString> line_iter(*Lexicon->get_word_autobiography(word));
        while (line_iter.hasNext()){
            QString report_line = line_iter.next();
            item_list.clear();
            QStringList report_line_items = report_line.split("=");
            for (int i = 0; i < report_line_items.size(); i++){
                p_item = new QStandardItem(report_line_items[i]);
                if (i == 0 && report_line_items[i][0] == "*"){
                    p_item->setBackground(Qt::red);
                } else{
                    p_item->setBackground(Qt::white);
                }
                item_list.append(p_item);
            }
            m_my_current_model->appendRow(item_list);
        }
    }

}
*/
/**
 * @brief LowerTableView::table_stem
 * @param pWord
 * What follows is a set of functions that display different kinds of user-requested information on the Lower Table View.
 */
void LowerTableView::table_stem(stem_t stem, CLexicon* Lexicon){
    QList<QStandardItem*>      item_list;
    QStandardItem *            p_item, *q_item;

    // Create a clean model.
    if (m_my_current_model){
        delete m_my_current_model;
    }
    m_my_current_model = new QStandardItemModel();

    // Find the stem's autobiography and set it, line by line, in the lower TableView.
    if (Lexicon->stem_autobiographies_contains(stem)) {
        //qDebug() << 339 << "true";
        QListIterator<QString> line_iter(*Lexicon->get_stem_autobiography(stem));
        while (line_iter.hasNext()){
            QString report_line = line_iter.next();
            item_list.clear();
            QStringList report_line_items = report_line.split("=");
            for (int i = 0; i < report_line_items.size(); i++){
                p_item = new QStandardItem(report_line_items[i]);
                if (i == 0 && report_line_items[i][0] == "*"){
                    p_item->setBackground(Qt::red);
                } else{
                    p_item->setBackground(Qt::white);
                }
                item_list.append(p_item);

            }
            m_my_current_model->appendRow(item_list);
        }
    }

}



void LowerTableView::table_signature(CSignature* pSig ){

    QStandardItem*             pItem1, * pItem2;
    QList<QStandardItem*>      item_list;

    item_list.clear();
    //CStem_ptr_list    *   sig_stems = pSig->get_stems();
    if (m_my_current_model) { delete m_my_current_model;}
    m_my_current_model = new QStandardItemModel();

    pItem1 = new QStandardItem("Stem final letter entropy");

    m_my_current_model->appendRow(item_list);
    item_list.append(pItem1);
    pItem1 = new QStandardItem(QString::number(pSig->get_stem_entropy()));
    item_list.append(pItem1);
    m_my_current_model->appendRow(item_list);

    item_list.clear();
/*    foreach (p_Stem, *sig_stems)  {
        p_item = new QStandardItem(p_Stem->get_key() );
        item_list.append(p_item);
        if (item_list.length() >= m_number_of_columns){
            m_my_current_model->appendRow(item_list);
            item_list.clear();
        }
    }
    if (item_list.size() > 0){
        m_my_current_model->appendRow(item_list);
    }

    QList<int> count_list;
    qDebug() << pSig->get_key();
    for (int stemno = 0; stemno< pSig->get_number_of_stems(); stemno++){
        count_list << pSig->get_stems()->at(stemno)->get_count();
    }
    qSort(count_list);
*/

    CStem* pStem;
    pSig->sort_stems_by_count();
    for (int stemno = 0; stemno< pSig->get_number_of_stems(); stemno++){
        pStem = pSig->get_stems()->at(stemno);
        pItem1 = new QStandardItem(pStem->get_key() );
        item_list.append(pItem1);
        pItem2 = new QStandardItem(QString::number(pStem->get_count() ));
        item_list.append(pItem2);
        m_my_current_model->appendRow(item_list);
        item_list.clear();    }


    resizeColumnsToContents();

}

/**
 * @brief LowerTableView::table_one_signature
 * @param pSig
 * @param stems
 *
 * This displays information about a single signature.
 */
void LowerTableView::table_one_signature(CSignature* pSig, QStringList stems)
{
    QStandardItem*             p_item;
    QList<QStandardItem*>      item_list;
    CStem*                     p_Stem;

    p_item = new QStandardItem(pSig->get_key());
    item_list.append(p_item);
    m_my_current_model->appendRow(item_list);
    item_list.clear();
    foreach(p_Stem, *pSig->get_stems()){
        stem_t this_stem_t = p_Stem->get_key();
        p_item = new QStandardItem(this_stem_t);
        if (stems.contains(this_stem_t)){
            p_item->setBackground(Qt::red);
        }
        item_list.append(p_item);
        if (item_list.length() >= m_number_of_columns){
            m_my_current_model->appendRow(item_list);
            item_list.clear();
        }
    }
    if (item_list.size() > 0 ){
        m_my_current_model->appendRow(item_list);
        item_list.clear();
    }
}
/**
 * @brief LowerTableView::table_passive_signature
 * @param p_this_sig
 * A passive signature is a signature in which the set of stems all end (in the case of a suffixal system) with the same letter, or nearly so.
 * Such signatures are unhealthy but sometimes very useful.
 */
void LowerTableView::table_passive_signature(CSignature *p_this_sig)
{
    QStandardItem*              p_item;
    QList<QStandardItem*>       item_list;
    //sig_graph_edge *             p_edge;
    QMap<CSignature*, int>      stem_counter; // key is signature, value is number of stems shared with p_this_sig;
    QList<CSignature*>          sig_list;
    m_my_current_model =        new QStandardItemModel();
    QString                     morph;
    QMap<CSignature*, QString>  Morphs;

    // put signatures in a Map, values are numbers of stems shared
    QMap<QString, sig_graph_edge*> * pMap = get_lexicon()->get_sig_graph_edge_map();
    QMapIterator<QString, sig_graph_edge*> this_sig_graph_edge_iter (*pMap);
    while (this_sig_graph_edge_iter.hasNext()){
        sig_graph_edge * p_edge  = this_sig_graph_edge_iter.next().value();
        if (p_this_sig == p_edge->m_sig_1){
            stem_counter[p_edge->m_sig_2] = p_edge->get_number_of_words();
            Morphs[p_edge->m_sig_2] = p_edge->morph;
        }
    }

    QStringList         temp_signatures;
    QList<CSignature*>  sorted_signatures;
    QList<int>          counts = stem_counter.values();
    std::sort(counts.begin(), counts.end());
    int                 count;

    for (int i = 0; i < counts.size(); i++){
            count = counts[i];
            QList<CSignature*> signatures =stem_counter.keys(count);
            std::sort(signatures.begin(), signatures.end());
            foreach (CSignature* pSig, signatures){
                sorted_signatures.append(pSig);
            }
     }

    for (int signo = sorted_signatures.count()-1; signo >= 0 ;signo-- ){
        CSignature* pSig2 = sorted_signatures[signo];
        sig_string sig2 = pSig2->get_key();
        item_list.clear();
        p_item = new QStandardItem(Morphs[pSig2]);
        item_list.append(p_item);
        p_item = new QStandardItem(sig2);
        item_list.append(p_item);
        p_item = new QStandardItem(QString::number(stem_counter[pSig2]) );
        item_list.append(p_item);
        m_my_current_model->appendRow(item_list);
        }
}

// for protostem display
void LowerTableView::table_protostem(protostem *p_protostem)
{
    typedef QStandardItem QSI;
    CLexicon* this_lexicon = get_parent_window()->get_lexicon();
    m_my_current_model = new QStandardItemModel();

    QString str_protostem = p_protostem->get_stem();
    int start_word_index = p_protostem->get_start_word();
    int end_word_index = p_protostem->get_end_word();
    QStringList labels;
    labels << "" << str_protostem;
    m_my_current_model->setHorizontalHeaderLabels(labels);

    for (int wordno = start_word_index; wordno <= end_word_index; wordno++) {
        QList<QSI*> item_list;
        QSI* item1 = new QSI();
        item1->setData(wordno, Qt::DisplayRole);
        QString str_word = this_lexicon->get_words()->get_word_string(wordno);
        QSI* item2 = new QSI(str_word);
        item_list.append(item1);
        item_list.append(item2);
        m_my_current_model->appendRow(item_list);
    }

}

void LowerTableView::table_compound_composition(CompoundWord* p_compound, int composition_i)
{
    typedef  QStandardItem QSI;
    typedef  CompoundWord::CompoundComposition CompoundComposition;
    typedef  CompoundComponent::CompoundConnectionMap ConnectionMap;
    m_my_current_model = new QStandardItemModel();

    QStringList labels;
    labels << "Component of compound";

    CompoundComposition* p_composition
            = p_compound->get_compositions()[composition_i];
    CompoundComponent* p_component;
    foreach (p_component, *p_composition) {
        QList<QSI*> item_list;
        QSI* item0 = new QSI(p_component->get_word());
        item0->setBackground(QBrush(QColor(200, 200, 200)));
        item_list.append(item0);
        ConnectionMap::ConstIterator connection_iter;
        const ConnectionMap& connection_map = p_component->get_connections();
        for (connection_iter = connection_map.constBegin();
             connection_iter != connection_map.constEnd();
             connection_iter++) {
            QSI* connection_item = new QSI(connection_iter.key());
            item_list.append(connection_item);
        }
        m_my_current_model->appendColumn(item_list);
    }
}
