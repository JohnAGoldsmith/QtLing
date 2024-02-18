#include <QDebug>
#include "Word.h"
#include "sigpair.h"

CWord::CWord(QString word) :  m_key(word), m_count(0), m_morpheme_parse(word)
{
    //m_Signatures.clear();
    //m_morpheme_parse  = MorphemeParse(word);
    m_compound_component = 0;

}

CWord::CWord(CWord& word):m_morpheme_parse (*word.get_morpheme_parse())
{   m_count = word.count();
    m_key      = word.get_key();
    //m_Signatures.clear();
 }


CWord::~CWord()
{
    clear_parse_triple_map();
}
QString Parse_triple::display(){
    QString result = "stem: [" + m_stem +
            "]; suffix: [" + m_suffix +
            "]; sig string: " + m_sig_string;
    return result;
}
QString parse_triple_list_display(QList<Parse_triple*> triples){
    QString result;
    foreach (Parse_triple* triple, triples){
        result += triple->display();
    }
    return result;
}
QString CWord::display(){
    QString result;
    result = "word: " + m_key + "; " +
             "count: " + QString::number(m_count) + " "; // +
            if (m_suffixal_parse_triple_list.count() > 0) {
                    result += "suffixal parse triples: " + parse_triple_list_display(m_suffixal_parse_triple_list) + " ";
            }
            if (m_prefixal_parse_triple_list.count() > 0){
                result += "prefixal parse triples: " + parse_triple_list_display(m_prefixal_parse_triple_list) + " ";
            }
            result += "morphemic splits: " + m_Morphemic_splits.join(" = ") + " " +
            //"JSON id: " + m_json_id + " " +
            "Compounds: " + m_compound_structure.first+"=" + m_compound_structure.second;
            return result;
}
/*
void CWord::add_compound(QString string){
    if (m_Compounds.contains(string)){
        return;
    }
    m_Compounds.append(string);
}
*/
void CWord::analyze_as_compound(QString c1, QString c2){
    m_compound_structure = QPair(c1, c2);
}
void CWord::IncrementWordCount (int n )
{
    m_count += n;
    Q_ASSERT (m_count > 0);
    Q_ASSERT (m_count < 1000000);
}
void CWord::add_suffixal_parse_triple(QString stem, QString affix, QString sig_string)
{
    Parse_triple * this_triple = new Parse_triple(stem, affix, sig_string);
    m_suffixal_parse_triple_list.append(this_triple);
    //qDebug()<< 65 << m_suffixal_parse_triple_list[0]->display();
}
void CWord::add_prefixal_parse_triple(QString stem, QString affix, QString sig_string)
{
    Parse_triple * this_triple = new Parse_triple(stem, affix, sig_string);
    m_prefixal_parse_triple_list.append(this_triple);
}
void CWord::add_parse_triple(QString stem, QString affix, QString sig_string, bool suffix_flag)
{
    Parse_triple * this_triple = new Parse_triple(stem, affix, sig_string);
    suffix_flag?
        m_suffixal_parse_triple_list.append(this_triple):
        m_prefixal_parse_triple_list.append(this_triple);
}
int compare_triple_stem_length(Parse_triple* parsetriple1, Parse_triple* parsetriple2) {
    return parsetriple1->m_stem.length() < parsetriple2->m_stem.length();
}
void CWord::sort_suffixal_parse_triple_list(){
      std::sort( m_suffixal_parse_triple_list.begin(), m_suffixal_parse_triple_list.end(),  compare_triple_stem_length);
}
void CWord::sort_prefixal_parse_triple_list(){
      std::sort( m_prefixal_parse_triple_list.begin(), m_prefixal_parse_triple_list.end(),  compare_triple_stem_length);
}
void CWord::clear_parse_triple_map()
{
    foreach (Parse_triple* p_parse_triple, m_suffixal_parse_triple_list) {
        delete p_parse_triple;
    }
    m_suffixal_parse_triple_list.clear();
}
QList<sig_pair*> *  CWord::get_suffixal_sigpairs(){
    if (m_suffixal_sigpairs.length() > 0) {return &m_suffixal_sigpairs;}
    if (m_suffixal_parse_triple_list.length() < 2) {return  &m_suffixal_sigpairs;}
    int n= 0;
    for (n = 0; n < m_suffixal_parse_triple_list.length()-1; n++){
        Parse_triple * thisParseTriple = m_suffixal_parse_triple_list.at(n);
        Parse_triple * nextParseTriple = m_suffixal_parse_triple_list.at(n+1);
        int diff = nextParseTriple->m_stem.length() - thisParseTriple->m_stem.length();
        QString difference = nextParseTriple->m_stem.right(diff);
        sig_pair* this_sig_pair = new sig_pair(thisParseTriple->m_stem, difference, thisParseTriple->m_sig_string, nextParseTriple->m_sig_string );
        m_suffixal_sigpairs.append(this_sig_pair);
    }
    return & m_suffixal_sigpairs;
}

bool CWord::contains_this_stem_among_parses(QString stem){
    foreach (Parse_triple* triple, m_suffixal_parse_triple_list){
       if (triple->m_stem == stem){
           return true;
       }
    }
    return false;
}
void CWord::add_morphemic_split(QString split){
    if (m_Morphemic_splits.contains(split)){
        return;
    }
    m_Morphemic_splits.append(split);
    return;
}
void CWord::remove_morphemic_split(QString morpheme_split){
    if (m_Morphemic_splits.contains(morpheme_split)){
        m_Morphemic_splits.removeAll(morpheme_split);
    }
}
