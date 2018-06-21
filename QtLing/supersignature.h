#ifndef SUPERSIGNATURE_H
#define SUPERSIGNATURE_H

#include "generaldefinitions.h"
#include "Typedefs.h"

class CSuffix;
class CSignature;
class suffix_array;

class pair_of_suffixes
{
    friend suffix_array;

    CSuffix* pSuffix_1;
    CSuffix* pSuffix_2;
    suffix_t suffix_1;
    suffix_t suffix_2;
public:
    CSuffix* get_suffix_1_ptr() {return pSuffix_1;}
    CSuffix* get_suffix2_ptr() {return pSuffix_2;}
};


class suffix_array {
    QList< pair_of_suffixes > m_suffix_pair_list;

public:
    QList<pair_of_suffixes>*      get_suffix_pair_list() {return & m_suffix_pair_list;}
    pair_of_suffixes*             get_suffix_pair (int n) {return & m_suffix_pair_list[n]; }
    CSuffix*                     get_at_suffix1_ptr (int n)   {return m_suffix_pair_list[n].pSuffix_1; }
    CSuffix*                     get_at_suffix2_ptr (int n)   {return m_suffix_pair_list[n].pSuffix_2; }
};


class CSupersignature
{
    suffix_array     m_signature_array;
    CSignature*      m_sig_1;
    CSignature*      m_sig_2;
public:
    CSupersignature(CSignature*, CSignature*);
    ~CSupersignature();
    void test();
    QString         get_label() {return m_sig_1->get_key() + " = " + m_sig_2->get_key(); }
};


#endif // SUPERSIGNATURE_H
