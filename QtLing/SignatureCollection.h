#ifndef CSIGNATURECOLLECTION_H
#define CSIGNATURECOLLECTION_H

#include <QList>
#include <iterator>
#include <QMap>
#include "generaldefinitions.h"
#include "Signature.h"
#include "Typedefs.h"


class CLexicon;
//class CSignature;

class CSignatureCollection
{
protected:
    map_string_to_sig               m_SignatureMap;
    int                             m_CorpusCount;
    QString                         m_MemberName;
    QList<CSignature*>              m_SortList;
    bool                            m_SortValidFlag;
    enum  eSortStyle                m_SortStyle;
    map_sigstring_to_sig_ptr_iter * m_MapIterator;
    QListIterator<CSignature*>  *   m_SortedListIterator;
    QMap<CSignature*,QList<CSignature*>*>  m_ContainmentMap;


public:
    CSignatureCollection();
    ~CSignatureCollection();

// disable copy
private:
    CSignatureCollection(const CSignatureCollection& x);
    CSignatureCollection& operator=(const CSignatureCollection& x);

public:
    friend class CLexicon;
    CSignature*                             operator<< ( QString );
    void                                    operator<< ( CSignature* );
    CSignature*                             operator^= ( QString );
    CSignature*                             operator[] (int n) { return m_SortList[n];}
    CSignature*                             find_or_add ( QString); // same as operatorˆ=
    CSignature*                             get_at_sorted( uint n ) { return m_SortList[n];}
    int                                     get_count() const       { return m_SignatureMap.size(); }
    void                                    clear();
    CSignature*                             get_signature(QString sig) {return m_SignatureMap.value(sig); }
    map_sigstring_to_sig_ptr_iter *         get_map_iterator() ;
    map_string_to_sig*                      get_map()                   { return & m_SignatureMap;}
    QListIterator<CSignature*>   *          get_sorted_list_iterator();
    //void                                    make_sorted_list_iterator(map_string_to_word_ptr_iter&);
    bool                                    contains (sigstring_t);
    void                                    sort();
    void                                    sort_signatures_by_affix_count();
    void                                    compute_containment_list(); //this gives   for each signature the largest signatures it contains.

};
#endif // CSIGNATURECOLLECTION_H
