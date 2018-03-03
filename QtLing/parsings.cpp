#include <QString>
#include "parsings.h"
parsings::parsings(QString base_string)
{
    m_base_string = base_string;
}

parsings::~parsings()
{
    for (int parseno = 0; parseno < m_parses.count(); parseno++){
        for (int stringno = 0; stringno < m_parses[parseno]->length(); stringno++){
            delete m_parses[parseno]->at(stringno);
        }
        delete m_parses[parseno];
    }
}
void copy_parse(QList<QString*> * to_parse, QList<QString*> * from_parse){
    for (int pieceno = 0; pieceno < from_parse->count(); pieceno++){
        QString* new_string = new QString (*from_parse->at(pieceno));
        to_parse->append(new_string);
    }
}
void parsings::snip(QString affix)
{
    QList<QList<QString*>*> new_parsings;
    for (int parseno = 0; parseno < m_parses.count(); parseno++)
    {
        QList<QString*> * this_parse = m_parses[parseno];
        if (m_directionality == Dir_left_to_right){ // when we parse left to right, we are always working on the left end of the last piece.
            QString * last_piece = this_parse->back();
            if (last_piece->startsWith(affix)){
                QList<QString*> * new_parse = new QList<QString*>;
                copy_parse(new_parse, this_parse);
                new_parse->removeLast();
                new_parse->append(&affix);
                last_piece->remove(0, affix.length());
                new_parse->append(last_piece);
                new_parsings.append(new_parse);
            }

        } else{ //-->   right to left (suffixes, probably) <--//
            QString * first_piece = this_parse->front();
            if (first_piece->endsWith(affix)){
                QList<QString*> * new_parse = new QList<QString*>;
                new_parse->removeFirst();
                new_parse->prepend(&affix);
                first_piece->chop(affix.length());
                new_parse->prepend(first_piece);
                new_parsings.append(new_parse);
            }
        }
    }
    m_parses += new_parsings;
}
void parsings::snip(QStringList list)
{
    for (int listno = 0; listno < list.count(); listno++){
        snip(list[listno]);
    }
}

