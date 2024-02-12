#ifndef MORPHEMEPARSE_H
#define MORPHEMEPARSE_H
#include <QStringList>

class MorphemeParse
{
    QStringList morphs;
    int         first_index;
    int         last_index;
    int         root_index;
public:
    MorphemeParse(QStringList, int root_index);
    MorphemeParse(QString);
    QString     get_root() {return morphs[root_index];}
    QString     get_morph(int n);
    int         get_number_of_suffixes() {return last_index - root_index;}
    int         get_number_of_prefixes() {return root_index - first_index;}
    QString     display() {return morphs.join(" ");}
    void        copy (MorphemeParse*);
};

#endif // MORPHEMEPARSE_H
