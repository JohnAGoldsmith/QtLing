#ifndef STRFUNC_H
#define STRFUNC_H

#include <QString>
#include<QChar>

class CStringSurrogate;
template<class K, class V> class QMap;

QChar*    LxAlphabetizeString (const QChar*, int);

// CStringSurrogate/QChar/QString copies
void    LxStrCpy( const CStringSurrogate* From, QChar*& To, int Length, int Start = 0 );
void    LxStrCpy( const CStringSurrogate* From, QString& To, int Length, int Start = 0 );
void    LxStrCpy( const QChar* From, QChar*& To, int Length, int Start = 0 );
void    LxStrCpy( const QChar* From, QString& To, int Length, int Start = 0 );
void    LxStrCpy( const QString& From, QChar*& To, int Length, int Start = 0 );
void    LxStrCpy( const QString& From, QString& To, int Length, int Start = 0 );

void    LxStrCpy_R( const CStringSurrogate* From, QChar*& To, int Length, int Start = 0 );
void    LxStrCpy_R( const CStringSurrogate* From, QString& To, int Length, int Start = 0 );
void    LxStrCpy_R( const QChar* From, QChar*& To, int Length, int Start = 0 );
void    LxStrCpy_R( const QChar* From, QString& To, int Length, int Start = 0 );
void    LxStrCpy_R( const QString& From, QChar*& To, int Length, int Start = 0 );
void    LxStrCpy_R( const QString& From, QString& To, int Length, int Start = 0 );

void    LxStrFll( const CStringSurrogate* From, QChar*& To, int LengthFrom, int LengthTo, int Start = 0 );
void    LxStrFll( const CStringSurrogate* From, QString& To, int LengthFrom, int LengthTo, int Start = 0 );
void    LxStrFll( const QChar* From, QChar*& To, int LengthFrom, int LengthTo, int Start = 0 );
void    LxStrFll( const QChar* From, QString& To, int LengthFrom, int LengthTo, int Start = 0 );
void    LxStrFll( const QString& From, QChar*& To, int LengthFrom, int LengthTo, int Start = 0 );
void    LxStrFll( const QString& From, QString& To, int LengthFrom, int LengthTo, int Start = 0 );


// CStringSurrogate/QChar/QString comparisons
int		LxStrCmp( const CStringSurrogate* Str1, const CStringSurrogate* Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );
int		LxStrCmp( const CStringSurrogate* Str1, const QChar* Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );
int		LxStrCmp( const CStringSurrogate* Str1, const QString& Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );
int		LxStrCmp( const QChar* Str1, const CStringSurrogate* Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );
int		LxStrCmp( const QChar* Str1, const QChar* Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );
int		LxStrCmp( const QChar* Str1, const QString& Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );
int		LxStrCmp( const QString& Str1, const CStringSurrogate* Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );
int		LxStrCmp( const QString& Str1, const QChar* Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );
int		LxStrCmp( const QString& Str1, const QString& Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );

int		LxStrCmp_R( const CStringSurrogate* Str1, const CStringSurrogate* Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );
int		LxStrCmp_R( const CStringSurrogate* Str1, const QChar* Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );
int		LxStrCmp_R( const CStringSurrogate* Str1, const QString& Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );
int		LxStrCmp_R( const QChar* Str1, const CStringSurrogate* Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );
int		LxStrCmp_R( const QChar* Str1, const QChar* Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );
int		LxStrCmp_R( const QChar* Str1, const QString& Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );
int		LxStrCmp_R( const QString& Str1, const CStringSurrogate* Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );
int		LxStrCmp_R( const QString& Str1, const QChar* Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );
int		LxStrCmp_R( const QString& Str1, const QString& Str2, int Length1, int Length2, int Start1 = 0, int Start2 = 0 );


// CStringSurrogate/QChar/QString concatenations
void    LxStrCat( QChar*& Front, const CStringSurrogate* Back, int LengthF, int LengthB, int StartF = 0, int StartB = 0 );
void    LxStrCat( QChar*& Front, const QChar* Back, int LengthF, int LengthB, int StartF = 0, int StartB = 0 );
void    LxStrCat( QChar*& Front, const QString& Back, int LengthF, int LengthB, int StartF = 0, int StartB = 0 );
void    LxStrCat( QString& Front, const CStringSurrogate* Back, int LengthF, int LengthB, int StartF = 0, int StartB = 0 );
void    LxStrCat( QString& Front, const QChar* Back, int LengthF, int LengthB, int StartF = 0, int StartB = 0 );
void    LxStrCat( QString& Front, const QString& Back, int LengthF, int LengthB, int StartF = 0, int StartB = 0 );


// QChar/QString mutators
void    LxStrLwr( QChar*, int, int = 0 );
void    LxStrLwr( QString& );
void    LxStrUpr( QChar*, int, int = 0 );
void    LxStrUpr( QString& );

// For RichMorphy Init test
int		OverlapOfTwoAlphabetizedLists( QString, QString);
int		DifferencesOfTwoAlphabetizedLists( QString, QString);

// conversions to string.
QString IntToStringWithCommas(int n);
QString DblToStringWithCommas(double n);
QString Filter(QMap<QString, QString>* filter, QString s);
QString DisplayInt(int n);
QString DisplayFloat(float f);

// "NULL" string.
const QString TheStringNULL = "NULL";

#endif // STRFUNC_H
