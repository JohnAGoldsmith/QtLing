#ifndef CSTEMCOLLECTION_H
#define CSTEMCOLLECTION_H

#include <QList>
#include <iterator>
#include "generaldefinitions.h"
#include "Stem.h"


class CLexicon;


class CStemCollection
{
protected:

    map_string_to_stem *        m_StringToStemMap;
    int                         m_CorpusCount;
    QString                     m_MemberName;
    QList<CStem*>               m_SortList;
    bool                        m_SortValidFlag;
    enum   eSortStyle           m_SortStyle;
    CLexicon*                   m_Lexicon;
public:
    CStemCollection(CLexicon*);
    ~CStemCollection();

// disable copy
private:
    CStemCollection(const CStemCollection& x);
    CStemCollection& operator=(const CStemCollection& x);

public:
    CStem*                          add (QString);  // deprecated
    void                            add_stem_pointer (CStem* pStem);
    void                            clear();
    CStem*                          find_or_add (QString);  // same as <<
    CStem*                          find_or_fail (QString); // same as ^=
    CStem*                          GetAtKey( QString);
    int                             get_count() const { return m_StringToStemMap->size(); }
    map_string_to_stem *            get_map() { return m_StringToStemMap;}         //       FIX THIS< DO IT RIGHT;
    QListIterator<CStem*> *         get_sorted_list_iterator();
    map_string_to_stem *            get_stem_map() { return m_StringToStemMap;}

    CStem*                          operator<< ( QString );
    CStem*                          operator^= ( QString );
};

#endif // CSTEMCOLLECTION_H
