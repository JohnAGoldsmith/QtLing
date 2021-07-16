#ifndef PARSINGS_H
#define PARSINGS_H
#include <QString>
#include <QStringList>
#include "generaldefinitions.h"

class parsings
{
    QString                     m_base_string;
    eDirectionality             m_directionality;
    QList<QStringList>       m_parses;
    int                         m_dot; // a pointer to a spot in the string, like in a parser.

public:
    parsings(QString base_string);
    ~parsings();
};

#endif // PARSINGS_H
