#ifndef CSTRINGSURROGATE_H
#define CSTRINGSURROGATE_H

#include <QString>
#include <QMap>

class CParse;
class CNode;

typedef QMap<QString,QString>  StringToString;

/// A surrogate for passing substrings.
/**
    In Linguistica, substrings are often passed as parameters that will never be edited.
    To minimize	unnecessary string copying, we use the string surrogate to represent strings.
    String surrogates are the pair (<i>s</i>,<i>l</i>), where <i>s</i> is a pointer to the
    beginning of the string and <i>l</i> is the length of the string.

    In certain cases, the reverse string is desired. String surrogates have a <i>backwards</i>
    flag. When the flag is set to <i>true</i>, all operations on the string surrogate will
    iterate from the end of the surrogate to the beginning.
*/

class CStringSurrogate
{
protected:
    const QChar* m_Key;			///< pointer to parent string
    int          m_Start;		///< start position of substring in parent string
    int          m_Length;		///< length of substring
    bool         m_Backwards;	///< true if string operations should iterate in reverse


public:	// construction/destruction.
    CStringSurrogate();                                          /// requires: len >= 0
    CStringSurrogate(const QChar* s, int len);                   /// requires: len >= 0
    CStringSurrogate(const QChar* s, int pos, int len,
            bool reversed = false);
    CStringSurrogate(const CParse& s, int pos = 0, int len = -1,
            bool reversed = false);
    CStringSurrogate(QString s, int pos = 0, int len = -1,       /// deprecated
            bool reversed = false);
    CStringSurrogate(const CParse* s, int pos = 0, int len = -1, /// deprecated
            bool reversed = false);
    CStringSurrogate(CNode* pNode, bool Backwards = false);
    virtual ~CStringSurrogate();

    // Copy.
    CStringSurrogate(const CStringSurrogate& other);
    void   operator=  ( const CParse& );
    void   operator=  ( const CParse* pParse );
    void   operator=  ( const QString );
    void   operator=  ( const CStringSurrogate& );

    // Operators
    QChar  operator[] ( const int ) const;
    CParse operator+  ( const CStringSurrogate& ) const;
    CParse operator+  ( const QChar ) const;
    bool   operator== ( const CStringSurrogate& ) const;
    bool   operator== ( const CParse& ) const;
    bool   operator!= ( const CStringSurrogate& SS ) const;
    bool   operator!= ( const CParse& Parse ) const;
    bool   operator>  ( const CStringSurrogate& ) const;
    bool   operator<  ( const CStringSurrogate& ) const;

    // Accessors
    QString          Display( StringToString* = NULL ) const;
    QChar            FirstChar() const;
    const QChar*     GetKey() const;
    int              GetLength() const;
    int              GetStart() const;
    bool             IsBackwards() const;
    bool             IsNULL() const;
    bool             IsPunctuation( QString ) const;
    QChar            LastChar() const;
    CStringSurrogate Left( int ) const;
    CStringSurrogate Mid( int SkipHowMany, int SpanLength = -1 ) const;
    CStringSurrogate Right( int ) const;

    // Mutators
    void ConvertNULL();
    void MakeNull();
    void SetBackwards( bool b = true );

private:
    // Accessors
    QChar GetChar( const int ) const;

};
#endif // CSTRINGSURROGATE_H
