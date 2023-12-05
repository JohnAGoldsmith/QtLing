#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QString>
#include "iostream"
#include "iomanip"
#include "Lexicon.h"
#include "mainwindow.h"
#include "WordCollection.h"
#include "Word.h"
#include "graphics.h"
#include "cparse.h"

void MainWindow::write_stems_and_words()
{
    CLexicon* lexicon = get_lexicon();
    QString file_name;
    file_name = "../../../../Dropbox/data/english/lxa/parses.txt";
    //CStem *     pStem;

    QFile out_file (file_name);

    if (! out_file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "************  no file opened for writing output, file output.cpp";
        return;
    }

    QTextStream textstream1 (&out_file);
    textstream1.setFieldAlignment(QTextStream::AlignRight);

    for (int n=0; n < lexicon->get_words()->get_word_count(); n++   ){
        CWord* pWord = lexicon->get_words()->get_word(n);
        for (int i =0; i<pWord->get_parse_triple_list()->size(); i++){
                Parse_triple *  this_triple =pWord->get_parse_triple_list()->at(i);
                textstream1.setFieldWidth(20);
                textstream1 << pWord->get_key() << "+" <<  this_triple->m_stem  << "+"<< this_triple->m_suffix << Qt::endl ;
        }
    }
}

void MainWindow::read_stems_and_words()
{
    CLexicon* lexicon = get_lexicon();
    QFile in_file (m_name_of_project_file);
    QTextStream out_corpus_costs(&in_file);

    if (! in_file.open(QFile::ReadOnly | QIODevice::Text))
    {
        qDebug() << "************  no file opened ";
        return;
    }
    while (! in_file.atEnd()){
        QString line = in_file.readLine();
        line = line.trimmed();
        QStringList words = line.split("+");
        if (words.size() == 3){
            CParse* pParse = new CParse(words[1], words[2]);
            lexicon->add_parse(pParse);
        }
        lexicon->add_word(words[0]);
    }
    lexicon->step2_from_protostems_to_parses();
    lexicon->step3_from_parses_to_stem_to_sig_maps("Reading stems and words");
    load_models(lexicon);
    create_or_update_TreeModel(lexicon);

    get_graphics_scene()->set_signature_collection(lexicon->get_active_signature_collection());
    m_leftTreeView->expandAll();
    m_leftTreeView->resizeColumnToContents(0);

}
void MainWindow::read_corpus()
{
    QString file_name = "../../../../Dropbox/data/english/TomSawyer.txt";
    QString word;
    QFile in_file (file_name);
    if (! in_file.open(QFile::ReadOnly | QIODevice::Text))
    {
        qDebug() << "************  no corpus file opened ";
        return;
    }
    int word_count_limit = 1000;
    int word_count = 0;
    while (! in_file.atEnd() && word_count <= word_count_limit){
        QString line = in_file.readLine();
        line = line.trimmed();
        QStringList words = line.split(" ");
        while (!words.isEmpty()){
            word_count++;
            word = words[0];
            words.pop_front();
            m_corpus.append(word);

        }
    }
}


