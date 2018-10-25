#ifndef CSTEM_H
#define CSTEM_H

#include <QString>
#include <QChar>
#include <QList>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>
#include "generaldefinitions.h"

class CSuffix;
class CSignature;
class CJsonInfo;

class CStem
{
protected:
    QString             m_key;                  // json tag: "stem"
    QList<CSignature*>  m_Signatures;           // json tag: "signatures"
    QList<CSuffix*>     m_parasignature;        // json tag: "parasignature"
    // a parasignature is made up of approved affixes,
    // but the parasignature has not yet been approved.
    int                 m_count;                // json tag: "count"
    QStringList         m_Autobiography;        // json tag: "autobiography"
    int                 m_word_list_start_point;
    int                 m_word_list_end_point;

    int                 m_json_id;
public:
    CStem (QString);
    CStem(CStem&);
public:
    //Accessors
    void                add_memo (QString memo) {m_Autobiography.append(memo);}
    void                add_signature(CSignature* pSig);
    void                add_suffix_to_parasignature(CSuffix* pSuffix) { m_parasignature.append(pSuffix);}
    QString             display();
    QString             get_key()               const{return m_key;}
    QString             get_stem()              const   { return m_key; }
    QList<CSignature*> * GetSignatures()                { return &m_Signatures; }
    CSignature*         get_last_signature()            { return m_Signatures.last(); }
    int                 get_count()             const   {return m_count;}
    void                set_count (int n)               {m_count = n;}

    void                write_json(QJsonObject& ref_json, eJsonType json_type = INDEXED) const;
    void                read_json_1(const QJsonObject& ref_json);
    void                read_json_2(const QJsonObject& ref_json, const CJsonInfo& ref_pointers);
    int                 get_json_id() const {return m_json_id;}
    void                set_json_id(const int id) {m_json_id = id;}
};

#endif // CSTEM_H
