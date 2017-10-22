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
    //map_string_to_sig           m_SignatureMap;
    QMap<QString, CSignature*>  m_SignatureMap;
    int                         m_CorpusCount;
    QString                     m_MemberName;
    QList<CSignature*>          m_SortList;
    bool                        m_SortValidFlag;
    enum  eSortStyle            m_SortStyle;

public:
    CSignatureCollection();
//    CSignatureCollection(CLexicon* Lex, QString MemberName = QString());
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
    CSignature*                             GetAt( uint );
    int                                     get_count() const       { return m_SignatureMap.size(); }
    CSignature*                             get_signature(QString sig) {return m_SignatureMap.value(sig); }
    QMapIterator<QString,CSignature*> *     get_map_iterator() ;
    QListIterator<CSignature*>        *     get_sorted_list_iterator();
    void                                    make_sorted_list_iterator(map_string_to_word_iter&);
    bool                                    contains (sigstring_t);
    void                                    sort();
    void                                    sort_signatures_by_affix_count();

};
#endif // CSIGNATURECOLLECTION_H
