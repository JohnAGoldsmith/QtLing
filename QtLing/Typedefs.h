#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#include <QSetIterator>
#include <QMapIterator>
#include "CParse.h"
class CStem;
class CWord;
class CPrefix;
class sig_graph_edge;
class CSuffix;

//class CSignature;

// These typedefs make it easier to write types with long names.
// Linguistica is optimized for reading, not writing, though, so
// in the end these should be replaced with either their long
// names or names that better describe their specific use.

#include "generaldefinitions.h"
template<class K, class V> class QMap;
template<class V> class Q3PtrVector;
template<class V> class Q3PtrList;
template<class V> class Q3Dict;

typedef  QPair<QString, int>        word_and_count;
typedef  QList<word_and_count*>     word_and_count_list;


// Home-style type-specifications. "_t" stands for "type".
typedef QString                             word_t;
typedef QString                             morph_t;
typedef QString                             stem_t;
typedef QString                             sigstring_t;
typedef QString                             suffix_t;
typedef QString                             prefix_t;
typedef QString                             affix_t;

typedef  QString                                    sig_string;
typedef  QPair<CStem*,CSignature*>                  stem_sig_pair;
typedef  QPair<stem_sig_pair*,                      stem_sig_pair*> pair_of_stem_sig_pairs;
typedef  QPair<stem_t,stem_t>                       stem_string_pair;
typedef  QPair<word_t, stem_string_pair*>           word_stem_pair_pair;

typedef QList<CParse>                      parse_list;
typedef QList<stem_t>                       stem_list;
typedef QList<suffix_t>                     suffix_list;
typedef QList<affix_t>                      affix_list;
typedef QList<QString>                      word_list;
typedef QList<CSignature*>                  SignatureList;
typedef QList<CSuffix*>                     CSuffix_ptr_list;
typedef QList<CStem*>                       CStem_ptr_list;
typedef QList<CSuffix*>                     CSuffix_ptr_list;
typedef QList<CPrefix*>                     CPrefix_ptr_list;
typedef QListIterator<suffix_t>             suffix_list_iterator;
typedef QListIterator<stem_t>               stem_list_iterator;
typedef QListIterator<CSuffix*>             CSuffix_ptr_list_iterator;
typedef QListIterator<CPrefix*>             CPrefix_ptr_list_iterator;
typedef QListIterator<CStem*>               CStem_ptr_list_iterator;
typedef QListIterator<CSignature*>          CSignature_ptr_list_iterator;

typedef QSet<morph_t>                       morph_set;
typedef QSet<stem_t>                        stem_set;
typedef QSet<suffix_t>                      suffix_set;
typedef QSet<affix_t>                       affix_set;
typedef QSetIterator<suffix_t>              suffix_set_iter;
typedef QSet<prefix_t>                      prefix_set;
typedef QSetIterator<prefix_t>              prefix_set_iter;
typedef QSet<affix_t>                       affix_set;
typedef QSetIterator<affix_t>               affix_set_iter;

typedef QMap<suffix_t, int>                 map_suffix_to_int;
typedef QMap<stem_t,suffix_set>             map_stem_t_to_suffix_set;
typedef QMap<stem_t,affix_set*>              map_stem_t_to_affix_set;
typedef QMap<stem_t, int>                   map_stem_to_int;
typedef QMap<sigstring_t, suffix_set*>      map_sigstring_to_suffix_set;
typedef QMap<sigstring_t, morph_set*>       map_sigstring_to_morph_set;
typedef QMap<sigstring_t, suffix_list*>     map_sigstring_to_suffix_list;
typedef QMap<sigstring_t, stem_list*>       map_sigstring_to_stem_list;
typedef QMap<stem_t, CStem*>                map_string_to_stem;
typedef QMap<QString, CWord*>               map_string_to_word;
typedef QMap<QString, CSignature*>          map_string_to_sig;
typedef QMap<int, QString>                  IntToString;
typedef QMap< QString, CStem>               StringToStem;
typedef QMap<unsigned int, unsigned int>    PtrToPtr;
typedef QMap<QString, int>                  StringToInt;
typedef QMapIterator<QString,CSignature*>   map_sigstring_to_sig_ptr_iter;
typedef QMapIterator<QString, CWord*>       map_string_to_word_ptr_iter;
typedef QMapIterator<QString, CSuffix*>     map_string_to_suffix_ptr_iter;
typedef QMapIterator<QString, CStem*>       map_string_to_stem_ptr_iter;

typedef QMap<QString,sig_graph_edge*>        lxa_sig_graph_edge_map;
typedef QMapIterator<QString,sig_graph_edge*>        lxa_sig_graph_edge_map_iter;

typedef  QMap<stem_t , QSet<affix_t>* >     Protosigs;

struct Sig_to_stems
{
    QMap<sigstring_t,QSet<stem_t>*>    m_core;
    QSet<stem_t>*  get_stem_set (sigstring_t this_sigstring) { return m_core[this_sigstring]; }
    bool contains_signature (sig_string sig) { return m_core.contains(sig); }
    bool contains_sig_and_stem (sig_string sig, stem_t this_stem) {
        if (m_core[sig]->contains(this_stem)){
            return true;
        }
        else{return false;}
    }
    void attach_stem_to_signature(stem_t this_stem, sigstring_t this_sig){
        if (! m_core.contains(this_sig)){
            m_core[this_sig] = new QSet<QString>;
        }
        m_core[this_sig]->insert(this_stem);
    }
    void clear() {m_core.clear();}
};



struct word_stem_struct
{
    word_t word;
    stem_t stem_1;
    stem_t stem_2;
    QString get_label() { return word + "=" + stem_1 + "=" + stem_2;}
};


#endif // TYPEDEFS_H
