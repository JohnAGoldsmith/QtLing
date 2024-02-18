#include "latex.h"
#include "Signature.h"
#include "WordCollection.h"

latex::latex() {}

QStringList latex::tabular_beginning(QString alignments){
    line_list.clear();
    line_list << "\\begin{center}";
    line_list << "\\begin{tabular}" + alignments;
    return line_list;
}
QStringList latex::tabular_ending(){
    line_list << "\\end{tabular}";
    line_list << "\\end{center}";
    return line_list;
}

QStringList latex::tabular(CSignature* sig){
    QStringList temp;
    temp    << sig->get_key();
    temp << QString::number(sig->get_stems()->count());
    temp << QString::number(sig->get_robustness());
    temp << QString::number(sig->get_stem_entropy());
    temp << QString::number(sig->get_number_of_affixes());
    line_list.append(temp.join(" & "));
    return line_list;
}
QStringList latex::tabular_word_counts( CWordCollection* words, int number_of_columns  ){
    QStringList temp;
    int colno = 0;
    for (int n = 0; n< words->get_count(); n++) {
        CWord* word = words->get_word(n);
        temp << word->get_key() << QString::number(word->count());
        if (colno == number_of_columns || n == words->get_word_count() ){
            line_list.append (temp.join(" & ") + " \\\\");
            temp.clear();
            colno = 0;
        }
    }
    return line_list;
}
