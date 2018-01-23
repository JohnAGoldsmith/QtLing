#include "lexicon.h"


void MainWindow::write_stems_and_words()
{
    CLexicon* lexicon = get_lexicon();
    QString file_name;

    QFile this_file (file_name);
    QTextStream out(this_file);


}
