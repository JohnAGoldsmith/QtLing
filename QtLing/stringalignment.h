#ifndef STRINGALIGNMENT_H
#define STRINGALIGNMENT_H

#include <QString>
#include <QStringList>
#include <QtMath>
#include "generaldefinitions.h"


/*
 * This class is used to compare two lists of strings. Typically each list is a signature.
 * This class can find the best alignment between the two lists.
 * It can also find the shortest peripheral substring that distinguishes a list-member
 * from all the other list-members.
 */
class StringAlignment
{

    QStringList m_strings1;
    QStringList m_strings2;

    QVector<int>  my_best_match;
    QVector<int> other_best_match;
    double tightness;

    QStringList m_edge_strings1;
    QStringList m_edge_strings2;

    eDirectionality  m_directionality;

   public:
    StringAlignment(QStringList, QStringList, eDirectionality = Dir_left_to_right);
    void find_edge_strings (); // no longer used?

    void match_up();
    QString display();
    bool properly_aligned();

   private:
    void end_sort();
    void beginning_sort();

};

#endif // STRINGALIGNMENT_H
