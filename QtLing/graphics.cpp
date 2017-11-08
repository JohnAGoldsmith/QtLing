#include <QDebug>
#include "graphics.h"
#include "generaldefinitions.h"
#include "SignatureCollection.h";
#include "mainwindow.h"

lxa_graphics_view::lxa_graphics_view(MainWindow* this_window, lxa_graphics_scene* my_scene)
{
    m_main_window = this_window;
    m_graphics_scene = my_scene;

    setScene(m_graphics_scene);

};

lxa_graphics_scene::lxa_graphics_scene (MainWindow * window)
{
    m_main_window = window;

};
lxa_graphics_scene::~lxa_graphics_scene ()
{

    for (int itemno = 0; itemno < m_signature_lattice.size(); itemno ++ ){
        delete m_signature_lattice[itemno];
    }
    m_signature_lattice.clear();

};
void lxa_graphics_scene::set_graphics_view(lxa_graphics_view * this_graphics_view)
{
    m_graphics_view  = this_graphics_view;
}
void lxa_graphics_scene::ingest_signatures(CSignatureCollection* signatures){
    qDebug() << "Ingesting signatures.";
    int max_size = 0;
    int sig_size;
    CSignature * pSig;
    map_sigstring_to_sig_ptr_iter sig_iter(*signatures->get_map());
    while(sig_iter.hasNext()){
        int this_size = sig_iter.next().value()->get_number_of_affixes();
        if (this_size > max_size){ max_size = this_size;}
    }
    for (int size = 0; size <= max_size; size++){
        QList<CSignature*> * signature_list = new QList<CSignature*>;
        m_signature_lattice.append(signature_list);
    }
    sig_iter.toFront();
    while(sig_iter.hasNext()){
        pSig = sig_iter.next().value();
        sig_size = pSig->get_number_of_affixes();
        m_signature_lattice[sig_size]->append(pSig);
        m_map_from_sig_to_column_no[pSig] = m_signature_lattice[sig_size]->size()-1;
    }
}
void lxa_graphics_scene::place_signatures()
{
    int row_delta = 60;
    int col_delta = 30;
    int number_of_rows = m_signature_lattice.size();
    int bottom_row = row_delta * number_of_rows;
    int sig_radius = 20;

    for (int row = 2; row <m_signature_lattice.size(); row++){
        CSignature_ptr_list_iterator sig_iter(*m_signature_lattice[row]);
        int col = 0;
        while (sig_iter.hasNext()){
            CSignature* pSig = sig_iter.next();
            int x = col * col_delta;
            int y = bottom_row - (row-2) * row_delta;
            addEllipse(x,y,sig_radius,sig_radius,QPen(),QBrush(Qt::red));
            col++;
        }
    }
}
void lxa_graphics_scene::place_containment_edges(CSignatureCollection* pSignatures){
    for (int i= 0; i < m_signature_containment_edges.size(); i++){

    }
}
