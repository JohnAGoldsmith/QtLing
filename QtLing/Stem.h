#ifndef CSTEM_H
#define CSTEM_H

//#include "Parse.h"
#include <QString>
#include <QChar>
#include <QList>
#include <QMap>
//#include "generaldefinitions.h"
class CSuffix;
class CSignature;

class CStem
{
protected:
    QString m_key;
    QList<QString>      m_Signatures;
    QList<CSuffix*>     m_parasignature; // a parasignature is made up of approved affixes, but the parasignature has not yet been approved.
    int                 m_count;
    QStringList         m_Autobiography;
    int                 m_word_list_start_point;
    int                 m_word_list_end_point;
public:
    CStem (QString);
    CStem(CStem&);
public:
    //Accessors
    QString             get_key()       const{return m_key;}
    QString             get_stem()       const { return m_key; }
    QList<QString>*     GetSignatures()  { return &m_Signatures; }
    void                add_signature(QString sig_string ) {m_Signatures.append(sig_string); } 
    QString             display();
    int                 get_count() {return m_count;}
    void                set_count (int n) {m_count = n;}
    void                add_suffix_to_parasignature(CSuffix* pSuffix) { m_parasignature.append(pSuffix);}
    void                add_memo (QString memo) {m_Autobiography.append(memo);}
};

#endif // CSTEM_H
