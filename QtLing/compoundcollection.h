#ifndef COMPOUNDCOLLECTION_H
#define COMPOUNDCOLLECTION_H

#include "compound.h"

class CompoundComponentCollection
{
    QMap<QString, CompoundComponent*>   m_map;
    CompoundWordCollection*             m_word_collection;
public:
    CompoundComponentCollection();
};

class CompoundWordCollection
{
    QMap<QString, CompoundWord*>        m_map;
    CompoundComponentCollection*        m_component_collection;
public:
    CompoundWordCollection();

    CompoundWord*                       add_compound_word(QString& word, QList<QString>& composition);

}

#endif // COMPOUNDCOLLECTION_H
