#ifndef CSIGNATURECOLLECTION_H
#define CSIGNATURECOLLECTION_H

#include <QList>
#include <iterator>
#include <QMap>
#include "generaldefinitions.h"
#include "Signature.h"
#include "Typedefs.h"


class CLexicon;


class CSignatureCollection
{
protected:
    map_string_to_sig                       m_SignatureMap;
    int                                     m_CorpusCount;
    QString                                 m_MemberName;
    QList<CSignature*>                      m_SortList;
    bool                                    m_SortValidFlag;
    enum  eSortStyle                        m_SortStyle;
    map_sigstring_to_sig_ptr_iter *         m_MapIterator;
    QListIterator<CSignature*>  *           m_SortedListIterator;
    QMap<CSignature*,QList<CSignature*>*>   m_ContainmentMap;
    bool                                    m_suffix_flag;
    CLexicon*                               m_Lexicon;
public:
    CSignatureCollection(CLexicon*, bool suffix_flag = true);
    ~CSignatureCollection();

private:
//    CSignatureCollection(const CSignatureCollection& x);
    CSignatureCollection& operator=(const CSignatureCollection& x);

public:
    friend class CLexicon;
    CSignature*                             operator<< ( QString );
    void                                    operator<< ( CSignature* );
    CSignature*                             operator^= ( QString );
    CSignature*                             operator[] (int n)              { return m_SortList[n];}
    CSignature*                             find_or_add ( QString);         // same as operatorˆ=
    CSignature*                             get_at_sorted( uint n )         { return m_SortList[n];}
    int                                     get_count()             const   { return m_SignatureMap.size(); }
    void                                    clear();
    CLexicon*                               get_lexicon()                   { return m_Lexicon; }
    CSignature*                             get_signature(QString sig)      {return m_SignatureMap.value(sig); }
    map_sigstring_to_sig_ptr_iter *         get_map_iterator() ;
    map_string_to_sig*                      get_map()                       { return & m_SignatureMap;}
    QListIterator<CSignature*>   *          get_sorted_list_iterator();
    bool                                    contains (sigstring_t);
    void                                    sort(eSortStyle);
    void                                    sort_signatures_by_affix_count();
    void                                    compute_containment_list(); //this gives   for each signature the largest signatures it contains.
    QMap<CSignature*,QList<CSignature*>*> * get_containment_map()           {return & m_ContainmentMap;}
    void                                    sort_each_signatures_stems_alphabetically();
    QList<word_and_count_list*> *           get_count_vectors(QList<word_and_count_list*> * count_vectors);
    void                                    calculate_stem_entropy();
    int                                     get_number_of_epositive_signatures();
};
#endif // CSIGNATURECOLLECTION_H
