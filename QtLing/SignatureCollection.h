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
    QList<CSignature*>                      m_signature_list;
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

    void                                    calculate_stem_entropy();
    void                                    clear();
    void                                    compute_containment_list(); //this gives   for each signature the largest signatures it contains.
    bool                                    contains (sigstring_t);
    CSignature*                             find_or_add ( QString);         // same as operatorˆ=
    CSignature*                             get_at_sorted( uint n )         { return m_SortList[n];}
    QMap<CSignature*,QList<CSignature*>*> * get_containment_map()           {return & m_ContainmentMap;}
    QList<word_and_count_list*> *           get_count_vectors(QList<word_and_count_list*> * count_vectors);
    int                                     get_count()             const   { return m_SignatureMap.size(); }
    void                                    get_epositive_signatures(QMap<CSignature*,int>);
    CLexicon*                               get_lexicon()                   { return m_Lexicon; }
    map_string_to_sig*                      get_map()                       { return & m_SignatureMap;}
    map_sigstring_to_sig_ptr_iter *         get_map_iterator() ;
    int                                     get_number_of_epositive_signatures();
    CSignature*                             get_signature(QString sig)      {return m_SignatureMap.value(sig); }
    QList<CSignature*> *                    get_signature_list()            { return & m_signature_list;}
    QMap<QString, CSignature*> *            get_signature_map()             {  return & m_SignatureMap;}
    QListIterator<CSignature*>   *          get_sorted_list_iterator();

    void                                    sort(eSortStyle);
    void                                    sort_signatures_by_affix_count();
    void                                    sort_each_signatures_stems_alphabetically();


};
#endif // CSIGNATURECOLLECTION_H
