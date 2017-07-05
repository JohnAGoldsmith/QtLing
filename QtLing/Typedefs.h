#ifndef TYPEDEFS_H
#define TYPEDEFS_H

// These typedefs make it easier to write types with long names.
// Linguistica is optimized for reading, not writing, though, so
// in the end these should be replaced with either their long
// names or names that better describe their specific use.

#include "generaldefinitions.h"
template<class K, class V> class QMap;
template<class V> class Q3PtrVector;
template<class V> class Q3PtrList;
template<class V> class Q3Dict;

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

#endif // TYPEDEFS_H
