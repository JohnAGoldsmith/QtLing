#include "Signature.h"
#include "Suffix.h"
#include <QDebug>
CSignature::CSignature(QString signature_string, bool suffix_flag)
{
  m_Signature = signature_string;
  m_Stems = new CStem_ptr_list();
  m_Suffixes = new CSuffix_ptr_list;
  m_Prefixes = new CPrefix_ptr_list;
  m_SuffixFlag = suffix_flag;
}

CSignature::CSignature(CSignature& signature) {
    m_Signature = signature.GetSignature();
    m_Prefixes = signature.get_prefix_list();
    m_Suffixes = signature.get_suffix_list();
    m_Stems = new CStem_ptr_list();
    m_SuffixFlag = signature.get_suffix_flag();
}
CSignature::~CSignature()
{
  delete m_Stems;
  delete m_Prefixes;
  delete m_Suffixes;
}



void CSignature::add_stem_pointer(CStem* pStem)
{
    if (m_Stems->contains(pStem)){
        return;
    }
    m_Stems->append(pStem);
}

bool compare_stems_of_sig(const CStem* pStem1, const CStem* pStem2)
{
 return  pStem1->get_key() < pStem2->get_key();
}
void CSignature::sort_stems(){
    qSort(m_Stems->begin(), m_Stems->end(), compare_stems_of_sig);
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
int CSignature::get_robustness() const
{
    int robustness = 0;
    int stem_letters = 0;
    int suffix_letters = 0;
    if (m_SuffixFlag){
        for (int suffix_no=0; suffix_no < m_Suffixes->size(); suffix_no++){
            if (m_Suffixes->at(suffix_no)->get_key() == "NULL"){
                suffix_letters += 1;
            } else{
                suffix_letters += m_Suffixes->at(suffix_no)->get_key().length();
            }
        }
    }
    for (int stem_no=0;stem_no<m_Stems->size();stem_no++){
        stem_letters += m_Stems->at(stem_no)->get_key().length();
    }
    robustness = stem_letters + suffix_letters;
    return robustness;
}
 int CSignature::get_number_of_affixes() const
{    if (m_SuffixFlag){
         if (m_Suffixes->count() > 0)
         {return m_Suffixes->count();}
         else {return m_Signature.count("="); }
     }
     else{
         if (m_Prefixes->count() > 0)
         {return m_Prefixes->count();}
         else {return m_Signature.count("="); }
     }
}

 bool CSignature::contains(CSignature *other) {
    if (m_SuffixFlag){
        for (int i  = 0; i < other->get_number_of_affixes(); i++){
            CSuffix* pSuffix = other->get_suffix_list()->at(i);
            if (! get_suffix_list()->contains (pSuffix)){
                return false;
            }
        }

    return true;
    } else {            // -->  prefixes <-- //
    for (int i  = 0; i < other->get_number_of_affixes(); i++){
            CPrefix* pPrefix = other->get_prefix_list()->at(i);
            if (! get_prefix_list()->contains (pPrefix)){
                return false;
            }
    }
    return true;
    }
}
