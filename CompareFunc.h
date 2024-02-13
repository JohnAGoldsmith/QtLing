#ifndef ASF_H
#define ASF_H

#include <QString>
#include <QStringList>

//class QStringList;

extern bool contains_affix_string(QString, QString);
extern bool signature_1_contains_signature_2(QString sig1, QString sig2);

// Sort indices according to values
extern void SortVector(int* out, const double* values, int len);


extern void SortQStringListFromRight(QStringList& l);


#endif // ASF_H
