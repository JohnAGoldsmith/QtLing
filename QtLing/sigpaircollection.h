#ifndef SIGPAIRCOLLECTION_H
#define SIGPAIRCOLLECTION_H

#include "sigpair.h"


#include "Typedefs.h"
class SigPairCollection
{
    QList<word_sig_pair*>  m_WordSigPairList;
    sig_pair_map           m_SigPairMap;

public:
    SigPairCollection();

    int     count() {return m_SigPairMap.count();}
    sig_pair_map *  get_map() {return & m_SigPairMap;}
};

#endif // SIGPAIRCOLLECTION_H
