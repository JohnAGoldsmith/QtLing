#ifndef TYPEDEFS_H
#define TYPEDEFS_H

class CStem;
class CWord;

// These typedefs make it easier to write types with long names.
// Linguistica is optimized for reading, not writing, though, so
// in the end these should be replaced with either their long
// names or names that better describe their specific use.

#include "generaldefinitions.h"
template<class K, class V> class QMap;
template<class V> class Q3PtrVector;
template<class V> class Q3PtrList;
template<class V> class Q3Dict;


// Home-style type-specifications. "_t" stands for "type".
typedef QString word_t;
typedef QString morph_t;
typedef QString stem_t;
typedef QString sigstring_t;

typedef QMap<class QString, class CStem>    StringToStem;
typedef QMap<unsigned int, enum eLxaClass>  PtrToClass;
typedef QMap<unsigned int, unsigned int>    PtrToPtr;
typedef QList<class CSignature*>            SignatureList;
typedef QList<class CSuffix*>               SuffixList;
typedef Q3Dict<class CSignature>            SignatureMap;
typedef Q3PtrVector<class CStem>            StemVector;
typedef QMap<class QString, int>            StringToInt;
typedef QMap<int, QString>                  IntToString;
typedef class CStringSurrogate              CSS;


typedef QSet<morph_t>                       morph_set;
typedef QSet<stem_t>                        stem_set;
typedef QMap<sigstring_t, morph_set*>       map_sig_to_morph_set;
typedef QMap<QString, CWord*>               map_string_to_word;
typedef QMap<QString, CSignature*>          map_string_to_sig;
typedef QMapIterator<QString, CWord*>       map_string_to_word_iter;
typedef QList<QString>                      word_list;




#endif // TYPEDEFS_H
