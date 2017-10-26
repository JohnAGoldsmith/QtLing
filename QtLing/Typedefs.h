#ifndef TYPEDEFS_H
#define TYPEDEFS_H

//class CStem;
class CWord;
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


typedef  QString                                    sig_string;
typedef  QPair<CStem*,CSignature*>                  stem_sig_pair;
typedef  QPair<stem_sig_pair*,                      stem_sig_pair*> pair_of_stem_sig_pairs;
//typedef  QPair<sig_string, pair_of_stem_sig_pairs*> five_tuple_sig_diffs;

// Home-style type-specifications. "_t" stands for "type".
typedef QString                             word_t;
typedef QString                             morph_t;
typedef QString                             stem_t;
typedef QString                             sigstring_t;
typedef QString                             suffix_t;

typedef QList<stem_t>                       stem_list;
typedef QList<suffix_t>                     suffix_list;
typedef QList<QString>                      word_list;
typedef QList<CSignature*>                  SignatureList;
typedef QList<CSuffix*>                     CSuffixList;
typedef QList<CStem*>                       CStemList;
typedef QList<CSuffix*>                     CSuffix_ptr_list;
typedef QListIterator<suffix_t>             suffix_list_iterator;
typedef QListIterator<CSuffix*>             CSuffix_ptr_list_iterator;
typedef QListIterator<CStem*>               CStem_ptr_list_iterator;

typedef QSet<morph_t>                       morph_set;
typedef QSet<stem_t>                        stem_set;
typedef QSet<suffix_t>                      suffix_set;

typedef QMap<suffix_t, int>                 map_suffix_to_int;
typedef QMap<sigstring_t, morph_set*>       map_sig_to_morph_set;
typedef QMap<sigstring_t, stem_list*>       map_sig_to_stem_list;
typedef QMap<stem_t, CStem*>                map_string_to_stem;
typedef QMap<QString, CWord*>               map_string_to_word;
typedef QMap<QString, CSignature*>          map_string_to_sig;
typedef QMap<int, QString>                  IntToString;
typedef QMap< QString, CStem>               StringToStem;
typedef QMap<unsigned int, unsigned int>    PtrToPtr;
typedef QMap<QString, int>                  StringToInt;
typedef QMapIterator<QString,CSignature*>   map_sigstring_to_sigptr_iter;
typedef QMapIterator<QString, CWord*>       map_string_to_word_iter;
typedef QMapIterator<QString, CSuffix*>     map_string_to_suffix_iter;
typedef QMapIterator<QString, CStem*>       map_string_to_stem_iter;

#endif // TYPEDEFS_H
