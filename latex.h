#ifndef LATEX_H
#define LATEX_H
#include <QStringList>

class CSignature;
class CWordCollection;

class latex
{
    QStringList line_list;

public:
    latex();
    QStringList tabular(CSignature*);
    QStringList tabular_beginning(QString alignments);
    QStringList tabular_ending();
    QStringList display(){return line_list;};
    QStringList tabular_word_counts(  CWordCollection*, int number_of_columns);
};

#endif // LATEX_H
