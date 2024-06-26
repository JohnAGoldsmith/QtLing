#include <QDebug>
#include "Word.h"
#include "WordCollection.h"
#include "mainwindow.h"

// not currently used

void MainWindow::analyze_corpus()
{
    while (!m_corpus.isEmpty()){
        QString word = m_corpus.first();
        m_corpus.removeFirst();
        CWord* pWord = get_lexicon()->get_words()->find_or_fail(word);

        if (pWord && pWord->get_parse_triple_list()->count() > 0){
           // qDebug() << pWord->get_key() << pWord->get_parse_triples()->first()->p_signature->get_key();
        } else
        {
          //  qDebug() << word << "not found";
        }
    }
}
