#ifndef CSIGNATURE_H
#define CSIGNATURE_H

#include "Stem.h"
#include <QString>
#include <QChar>
#include <QList>
#include <QMap>
#include "Typedefs.h"

class CSuffix;
class CPrefix;
class CStem;
class CSignatureCollection;
class QJsonObject;

sigstring_t restructure_signature(sigstring_t, QString morph, QStringList new_affixes);

class CSignature : public QObject
{

    //Oct 2021 test:
    Q_OBJECT

protected:
    QString                         m_Signature;
    QList<CSuffix*>                 m_Suffixes;
    QList<CPrefix*>                 m_Prefixes;
    QList<CStem*>                   m_Stems;
    QStringList                     m_Autobiography;
    bool                            m_SuffixFlag;
    CSignatureCollection*           m_SignatureCollection;
    double                          m_stem_entropy;
    int                             m_json_id;
    int                             m_secondary_stem_count;
    int                             m_robustness;

public:

    CSignature(CSignature&);
    CSignature( QString, bool suffix_flag = true, QObject *parent = 0);
    ~CSignature();
public:

    // functions dealing with string containment:
    void                        compute_signature_transforms(bool suffix_flag, QTextStream & sig_transforms);
    bool                        contains_affix_string (affix_t affix);
    bool                        contains (CSignature* other);
    int                         intersection_with(Affix_list& in, Affix_list& intersection);
    int                         get_size_of_intersection(CSignature* othersig);
    int                         get_size_of_intersection(QList<CSuffix*>*);
    void                        add_stem (QString);
    void                        add_stem_pointer(CStem*);
    void                        add_memo (QString memo)                 { m_Autobiography.append(memo);}
    void                        add_affix_ptr (CSuffix* pSuffix);       // { m_Suffixes->append(pSuffix);  }
    void                        add_affix_ptr (CPrefix* pPrefix);       // { m_Prefixes->append(pPrefix);  }
    void                        add_affix_string (QString);
    double                      calculate_stem_entropy();
    QString                     display();
    QString                     display_stems() ;
    void                        dump_string_set_of_suffixes(QSet<QString> & this_StringSet);
    int                         get_number_of_stems() const             { return m_Stems.count();}
    int                         get_number_of_affixes() const ;
    CSignatureCollection*       get_signatures()            const       {return m_SignatureCollection;}
    QString                     get_key()                   const       { return m_Signature;}
    int                         get_letter_count_in_stems();
    int                         get_letter_count_in_affixes();
    int                         get_letter_count_in_stems_and_affixes();
    QList<CPrefix*>*            get_prefix_list()                       {return &m_Prefixes;}
    double                      get_stem_entropy();
    QList<CStem*>*              get_stems()                             { return   &m_Stems;}
    QStringList&                get_stem_strings(QStringList&);
    QStringList                 get_affix_string_list();
    int                         get_secondary_stem_count()              {return m_secondary_stem_count;}
    bool                        get_suffix_flag()                       { return m_SuffixFlag;}
    void                        set_suffix_flag(bool flag)              { m_SuffixFlag = flag;}
    QList<CSuffix*>*            get_suffix_list()                       {return &m_Suffixes;}
    word_and_count_list *       get_word_and_count_vectors(word_and_count_list* );
    void                        increment_secondary_stem_count(int increment) { m_secondary_stem_count += increment;}

    void                        remove_suffix(suffix_t);
    void                        remove_prefix(prefix_t);
    void                        sort_stems();
    void                        sort_stems_by_count();

    void                        set_json_id(const int id) {m_json_id = id;}
    int                         get_json_id() const {return m_json_id;}
    void                        write_json(QJsonObject& ref_json, eJsonType json_type = INDEXED) const;

    void                        calculate_robustness();
    int                         calculate_secondary_robustness(); // does not take into consideration the number of letters in each stem, just *how many* there are and affix letters
    int                         get_robustness()  ;
    void                        increment_robustness(int increment) ;
    QString                     get_highfreq_edge_letters(float frequency_threshold);
};

#endif // CSIGNATURE_H
