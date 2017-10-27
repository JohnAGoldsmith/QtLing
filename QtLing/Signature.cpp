#include "Signature.h"
#include "Suffix.h"
#include <QDebug>
CSignature::CSignature(QString signature_string)
{
  m_Signature = signature_string;
  m_Stems = new CStem_ptr_list();
  m_Affixes = new CSuffix_ptr_list;
}

CSignature::CSignature(CSignature& signature) {
    m_Signature = signature.GetSignature();
    m_Affixes = signature.GetList();
    m_Stems = new CStem_ptr_list();
}
CSignature::~CSignature()
{
  delete m_Stems;
  delete m_Affixes;
}



void CSignature::add_stem_pointer(CStem* pStem)
{
    m_Stems->append(pStem);
}

void  add_stem (QString);

void CSignature::dump_string_set_of_suffixes(QSet<QString> & this_StringSet)
{   QString affix;
    QStringList affixlist = m_Signature.split("=");
    foreach (affix, affixlist){
        this_StringSet.insert(affix);
    }


}

QString CSignature::display()
{
    return m_Signature;
}

QString CSignature::display_stems()
{
    QString outstring;
    CStem *qStem;
    for (int stem_no = 0; stem_no < m_Stems->size(); stem_no ++ ){
        outstring += m_Stems->at(stem_no)->display();
    }
    return outstring;
}
int CSignature::get_robustness()
{
    int robustness = 0;
    int stem_letters = 0;
    int suffix_letters = 0;
    for (int suffix_no=0; suffix_no < m_Affixes->size(); suffix_no++){
        if (m_Affixes->at(suffix_no)->get_key() == "NULL"){
            suffix_letters += 1;
        } else{
            suffix_letters += m_Affixes->at(suffix_no)->get_key().length();
        }
    }
    for (int stem_no=0;stem_no<m_Stems->size();stem_no++){
        stem_letters += m_Stems->at(stem_no)->get_key().length();
    }
    robustness = stem_letters + suffix_letters;
    return robustness;
}
 int CSignature::get_number_of_affixes() const
{   if (m_Affixes->count() > 0)
     {return m_Affixes->count();}
     else {return m_Signature.count("="); }
}

     /*
int CSignature::number_of_true_suffixes()
{   CSuffixCollection* my_suffixes = get_signatures()->get_lexicon()->get_suffixes();
    QListIterator<CSuffix*> suffix_iter_1(* get_suffixes(my_suffixes));
    QMapIterator<QString,CSuffix*> suffix_iter_2();

    int suffix_count = 0;
    while (suffix_iter_1.hasNext()){
        suffix_iter_1.next();
        suffix_iter_2 = my_suffixes->contains(suffix_iter_1.get_key());

    }

}
*/
