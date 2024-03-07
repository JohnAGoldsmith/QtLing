#ifndef CSIGNATURECOLLECTION_H
#define CSIGNATURECOLLECTION_H

#include <QList>
#include <iterator>
#include <QMap>
#include "generaldefinitions.h"
#include "Signature.h"
#include "Typedefs.h"


class CLexicon;


class CSignatureCollection : public QObject
{  //October 2021 test:
    Q_OBJECT
protected:
    //QMap<QString, CSignature*>
    map_string_to_sig                       m_signature_map;
    int                                     m_CorpusCount; // What is this used for?
    QString                                 m_MemberName; // what is this used for?
    QList<CSignature*>                      m_sort_list; // not clear that we are using this, since the work is done by the Models generally
    QList<CSignature*>                      m_signature_list;
    QStringList                             m_sort_list_by_subsets;
    //QList<CSignature*>                      m_temp_list_for_tree_display();

    bool                                    m_SortValidFlag;
    enum  eSortStyle                        m_SortStyle;
    map_sigstring_to_sig_ptr_iter *         m_MapIterator;
    QListIterator<CSignature*>  *           m_SortedListIterator;
    QMap<CSignature*,QList<CSignature*>*>   m_ContainmentMap;
    bool                                    m_suffix_flag;
    CLexicon*                               m_Lexicon;
    QList<CSignature*>                      m_minimal_cover;
    int                                     m_minimum_number_of_stems_for_minimal_cover;
public:
    CSignatureCollection(CLexicon*, bool suffix_flag = true);
    ~CSignatureCollection();

private:
//    CSignatureCollection(const CSignatureCollection& x);
    CSignatureCollection& operator=(const CSignatureCollection& x);

public:
    // Many functions below used to take the type `QString` as arguments.
    // Changed them to `const QString&` for possibly greater efficiency.
    // Changed made by Hanson 7.30
    friend class CLexicon;

    CSignature*                             operator<< (const QString&);
    void                                    operator<< ( CSignature* );
    CSignature*                             operator^= (const QString&);
    CSignature*                             operator[] (int n)              { return m_sort_list[n];}
    CSignature*                             at(int n) {return m_sort_list[n];}

    void                                    add_this_and_all_subsignatures(QString sig_string, int stem_count, QStringList & signature_check_list);
    void                                    calculate_stem_entropy();
    void                                    clear();
    void                                    compute_containment_list(); //this gives   for each signature the largest signatures it contains.
    void                                    compute_signature_transforms(bool suffix_flag, QTextStream & sig_transforms);

    bool                                    contains (sigstring_t);
    CSignature*                             find_or_add (const QString&);         // same as operatorˆ=
    CSignature *                            find_or_fail(const QString&);
    void                                    find_minimal_cover(); // not used, should be removed
    CSignature*                             get_at_sorted( uint n )         { return m_sort_list[n];}
    QMap<CSignature*,QList<CSignature*>*> * get_containment_map()           {return & m_ContainmentMap;}
    QList<word_and_count_list*> *           get_count_vectors(QList<word_and_count_list*> * count_vectors);
    int                                     get_count()             const   { return m_signature_map.size(); }
    void                                    get_epositive_signatures(QMap<CSignature*,int>);
    CLexicon*                               get_lexicon()                   { return m_Lexicon; }
    map_string_to_sig*                      get_map()                       { return & m_signature_map;}
    QList<CSignature*> *                    get_minimal_cover()             {return & m_minimal_cover;}
    int                                     get_number_of_epositive_signatures();
    CSignature*                             get_signature(QString sig)      {return m_signature_map.value(sig); }
    QList<CSignature*> *                    get_signature_list()            { return & m_signature_list;}
    QStringList                             get_sort_list_by_subsets()      {return  m_sort_list_by_subsets;}
    QMap<QString, CSignature*> *            get_signature_map()             {  return & m_signature_map;}
    QListIterator<CSignature*>   *          get_sorted_list_iterator();
    QList<CSignature*> *                    get_sort_list()                 {return & m_sort_list;}
    bool                                    get_suffix_flag()               { return m_suffix_flag;}
    int                                     get_total_count_of_letters_in_all_signatures();
    double                                  get_description_length();
    void                                    sort(eSortStyle=SIG_BY_REVERSE_ROBUSTNESS);
    void                                    sort_signatures_by_affix_count();
    void                                    sort_each_signatures_stems_alphabetically();
    void                                    sort_signatures_by_secondary_stem_count();
    void                                    sort_signates_by_subsets();


    void                                    assign_json_id();
    void                                    write_json(QJsonObject& ref_json, eJsonType json_type = INDEXED) const;
    void                                    check_singleton_signatures(const QString& message);

    void                                    calculate_sig_robustness();
    void                                    produce_latex();
};
#endif // CSIGNATURECOLLECTION_H
