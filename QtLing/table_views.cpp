#include <QDebug>
#include "Lexicon.h"
#include "mainwindow.h"
#include "Word.h"
#include "WordCollection.h"
#include "graphics.h"

LowerTableView::LowerTableView()
{
   m_my_current_model =new  QStandardItemModel();
   m_number_of_columns = 20;
}

LowerTableView::LowerTableView(MainWindow * window)
{
   m_my_current_model =new  QStandardItemModel();
   m_parent_window = window;
   m_number_of_columns = 20;
}

 void LowerTableView::display_this_item( const  QModelIndex & index )
 {
     eDocumentType              UpperView_type = m_parent_window->m_tableView_upper->get_document_type();
     QString                    component = m_parent_window->m_tableView_upper->get_content();
     QString                    word, stem, prefix, suffix, signature;
     CLexicon *                 this_lexicon = get_parent_window()->get_lexicon();
     int                        row, column;
     QStandardItem*             p_item;
     QList<QStandardItem*>      item_list;

    //  ---------------------------------------------------//
     if (UpperView_type == WORDS){
    //  ---------------------------------------------------//
        if (index.isValid()){
             //word = index.data().toString();
             row = index.row();
        }
        QString word = index.sibling(row,0).data().toString();
        CWord* pWord = this_lexicon->get_words()->get_word(word);
        table_word(pWord);
        setModel( m_my_current_model);
    }
     //  ---------------------------------------------------//
     else if (UpperView_type == SIGNATURES){
     //  ---------------------------------------------------//
         if (index.isValid()) {row = index.row();}
         signature = index.sibling(row,0).data().toString();
         CSignature*  pSig = this_lexicon->get_signatures()->get_signature(signature);

        // -->  If Graphics is NOT showing, we display the information about the signature <-- //
        if (m_parent_window->m_graphic_display_flag == false){
            table_signature(pSig);
            setModel( m_my_current_model);
        } else // -->   Now, graphics display IS showing<-- //
        {   qDebug() << "trying to display a focus signature";
            m_parent_window->get_graphics_scene_1()->set_focus_signature(pSig);
            m_parent_window->get_graphics_scene_1()->display_focus_signature();
        }
    }
     //  ---------------------------------------------------//
     else if (UpperView_type == PREFIX_SIGNATURES){
     //  ---------------------------------------------------//
        item_list.clear();
        if (index.isValid()) {row = index.row();}
        signature = index.sibling(row,0).data().toString();
        CSignature*           pSig = this_lexicon->get_prefix_signatures()->get_signature(signature);
       // -->  If Graphics is NOT showing, we display the information about the signature <-- //
       if (m_parent_window->m_graphic_display_flag == false){
           table_signature(pSig);
           setModel( m_my_current_model);
       } // -->   Now, graphics display IS showing<-- //
       {
           qDebug() << "trying to display a focus signature";
           m_parent_window->get_graphics_scene_1()->set_focus_signature(pSig);
           m_parent_window->get_graphics_scene_1()->display_focus_signature();
       }
    }
     //  ---------------------------------------------------//
     else if (UpperView_type == RESIDUAL_PARASIGNATURES){
     //  ---------------------------------------------------//
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
    }
     //  ---------------------------------------------------//
     else if (UpperView_type == SIGNATURE_TREE_EDGES){
     //  ---------------------------------------------------//
        item_list.clear();
        if (index.isValid()){
             row = index.row();
             column = index.column();
        }
        QString               edge_key = index.sibling(row,4).data().toString();
        sig_tree_edge*        this_edge = this_lexicon->get_sig_tree_edge_map()->value(edge_key);
        sig_tree_edge *       pSig_tree_edge;
        CSignature*           pSig;
        CSignature*           pSig1 = this_edge->sig_1;
        CSignature*           pSig2 = this_edge->sig_2;
        QStandardItem*        p_item;
        CStem*                p_Stem;
        QStringList           sig1_stems;
        QStringList           sig2_stems;
        QStringList           words;
        word_stem_struct *    this_word_stem_item;

        if (m_parent_window->m_graphic_display_flag){
            //-->  Graphic display in lower right window <--//
            if (column == 1){
                pSig = pSig1;
            } else { pSig = pSig2;}
            graphics_sig_tree_edges(pSig, this_lexicon );
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
        }
    }
 }

 LeftSideTreeView::LeftSideTreeView(MainWindow* window)
 {
     m_parent_window = window;
 }

UpperTableView::UpperTableView (MainWindow* window)
{
        m_parent_window = window;
        m_signature_sort_style = UNSPECIFIED;
}
void UpperTableView::ShowModelsUpperTableView(const QModelIndex& index)
{
    QString component;
    qDebug() << "show model upper table" << index.data().toString();
    if (index.isValid()){
        component = index.data().toString();
    }
    if (component == "Words"){
        setModel(m_parent_window->m_Models["Words"]);
        set_document_type( WORDS );
        set_content_type( "words");
    }
    else     if (component == "Stems"){
        setModel(m_parent_window->m_Models["Stems"]);
        set_document_type( STEMS );
        set_content_type( "stems");
    }
    else     if (component == "Suffixes"){
        setModel(m_parent_window->m_Models["Suffixes"]);
        set_document_type( SUFFIXES );
        set_content_type( "suffixes");
    }
    else     if (component == "Signatures"){
        setModel(m_parent_window->m_Models["Signatures"]);
        set_document_type( SIGNATURES );
        m_parent_window->set_current_graphics_scene ( m_parent_window->m_graphics_scene_1 );
        set_content_type( "signatures");
    }
    else     if (component == "Prefix signatures"){
        setModel(m_parent_window->m_Models["Prefix signatures"]);
        set_document_type( PREFIX_SIGNATURES );
        set_content_type( "prefix signatures");
    }
    else     if (component == "Signature tree edges"){
        setModel(m_parent_window->m_Models["SigTreeEdges"]);
        set_document_type( SIGNATURE_TREE_EDGES );
        m_parent_window->set_current_graphics_scene ( m_parent_window->m_graphics_scene_2 );
        //set_content_type( "sigtreeedges");
        sortByColumn(-1);
    }
    else     if (component == "Residual parasignatures"){
        setModel(m_parent_window->m_Models["Residual parasignatures"]);
        set_document_type( SIGNATURE_RESIDUES );
        sortByColumn(1);
    }
    else     if (component == "Parasuffixes"){
        setModel(m_parent_window->m_Models["Parasuffixes"]);
        set_document_type( PARASUFFIXES );
        sortByColumn(1);
    }
    else     if (component == "Singleton signatures"){
        set_document_type( SINGLETON_SIGNATURES );
        sortByColumn(1);
    }
    resizeColumnsToContents();
}

void LowerTableView::graphics_sig_tree_edges(CSignature* pSig, CLexicon* p_lexicon)
{
    sig_tree_edge *       pSig_tree_edge;
    //-->  Graphic display in lower right window <--//
    // "Signatures" is what should be sent to the Model in the mainwindow.
    CSignatureCollection Signatures;
    Signatures << pSig;
    // We iterate through the SigTreeEdges in the SigTreeEdgeMap. If its Sig1 is pSig, then we enter it into Signatures;
    QMap<QString,sig_tree_edge*>::iterator edge_iter = p_lexicon->get_sig_tree_edge_map()->begin();
    while (edge_iter !=  p_lexicon->get_sig_tree_edge_map()->constEnd()){
        pSig_tree_edge = edge_iter.value();
        if (pSig_tree_edge->sig_1 == pSig){
            Signatures << pSig_tree_edge->sig_2;
        }
        ++edge_iter;
    }

    m_parent_window->m_graphics_scene_2 = new lxa_graphics_scene(m_parent_window, &Signatures);
    m_parent_window->m_graphics_scene_2->place_signatures();
    m_parent_window->m_graphics_view->setScene(m_parent_window->m_graphics_scene_2);
}

void LowerTableView::table_word(CWord* pWord ){
    QList<QStandardItem*>      item_list;
    QStandardItem *            p_item;

    if (m_my_current_model){
        delete m_my_current_model;
    }
    m_my_current_model = new QStandardItemModel();
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
}
void LowerTableView::table_signature(CSignature* pSig ){

    QStandardItem*             p_item;
    QList<QStandardItem*>      item_list;

    item_list.clear();
    CStem*                p_Stem;
    CStem_ptr_list    *   sig_stems = pSig->get_stems();
    if (m_my_current_model) { delete m_my_current_model;}
    m_my_current_model = new QStandardItemModel();
    foreach (p_Stem, *sig_stems)  {
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
}

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
