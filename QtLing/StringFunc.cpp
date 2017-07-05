#include "StringFunc.h"


#include <QString>
#include <QChar>
#include "StringSurrogate.h"

QChar* LxAlphabetizeString (const QChar* string, int length)
{
    int i;
    int*  Rank = new int [ length ];
    // Rank is an array that keeps track, for each letter occurrence, how many letters in the word are strictly
    // earlier than it alphabetically.
    for ( i =0; i < length; i++)
    {
        int p = 0;
        for (int j = 0; j < length; j++)
        {
            if ( string[j] < string[i] )
                p++;
        }
        Rank[i] = p;
    }

    QChar* NewString = new QChar [ length + 1];

    for ( i = 0; i<= length; i++)
        NewString[i] = '\0';

    int p = 0;
    for ( i = 0; i < length; i++)
    {
        p = Rank[  i  ];
        while ( NewString[p] != '\0' )
            p++;
        NewString[p] = string[i];
    }

    delete [] Rank;

    return NewString;

}

//--------------------------------------------------------------------
// CStringSurrogate/QChar/QString copies
//--------------------------------------------------------------------

void    LxStrCpy( const CStringSurrogate* From, QChar*& To, int Length, int Start )
{
    delete [] To;
    To = new QChar[Length];

    for( int i=0; i<Length; i++ )
    {
        To[i] = (*From)[i+Start];
    }
}


void    LxStrCpy( const CStringSurrogate* From, QString& To, int Length, int Start )
{
    To = "";
    for( int i=0; i<Length; i++ )
    {
        To += (*From)[i+Start];
    }
}


void    LxStrCpy( const QChar* From, QChar*& To, int Length, int Start )
{
    delete [] To;
    To = new QChar[Length];

    for ( int i=0; i<Length; i++ )
    {
        To[i] = From[i+Start];
    }
}


void    LxStrCpy( const QChar* From, QString& To, int Length, int Start )
{
    To = "";
    for ( int i=0; i<Length; i++ )
    {
        To += From[i+Start];
    }
}


void    LxStrCpy( const QString& From, QChar*& To, int Length, int Start )
{
    delete [] To;
    To = new QChar[Length];

    for ( int i=0; i<Length; i++ )
    {
        To[i] = From[i+Start];
    }
}


void    LxStrCpy( const QString& From, QString& To, int Length, int Start )
{
    To = From.mid( Start, Length );
}


void    LxStrCpy_R( const CStringSurrogate* From, QChar*& To, int Length, int Start )
{
    delete [] To;
    To = new QChar[Length];

    for ( int i=0; i<Length; i++ )
    {
        To[i] = (*From)[Length+Start-i-1];
    }
}


void    LxStrCpy_R( const CStringSurrogate* From, QString& To, int Length, int Start )
{
    for ( int i=0; i<Length; i++ )
    {
        To += (*From)[Length+Start-i-1];
    }
}


void    LxStrCpy_R( const QChar* From, QChar*& To, int Length, int Start )
{
    delete [] To;
    To = new QChar[Length];

    for ( int i=0; i<Length; i++ )
    {
        To[i] = From[Length+Start-i-1];
    }
}


void    LxStrCpy_R( const QChar* From, QString& To, int Length, int Start )
{
    for ( int i=0; i<Length; i++ )
    {
        To += From[Length+Start-i-1];
    }
}


void    LxStrCpy_R( const QString& From, QChar*& To, int Length, int Start )
{
    delete [] To;
    To = new QChar[Length];

    for ( int i=0; i<Length; i++ )
    {
        To[i] = From[Length+Start-i-1];
    }
}


void    LxStrCpy_R( const QString& From, QString& To, int Length, int Start )
{
    for ( int i=0; i<Length; i++ )
    {
        To[i] = From.at(Length+Start-i-1);
    }
}


void    LxStrFll( const CStringSurrogate* From, QChar*& To, int LengthFrom, int LengthTo, int Start )
{
    int Length = LengthFrom;
    if( Length > LengthTo ) Length = LengthTo;

    for ( int i=0; i<Length; i++ )
    {
        To[i] = (*From)[i+Start];
    }
}


void    LxStrFll( const CStringSurrogate* From, QString& To, int LengthFrom, int LengthTo, int Start )
{
    int Length = LengthFrom;
    if( Length > LengthTo ) Length = LengthTo;

    for ( int i=0; i<Length; i++ )
    {
        To[i] = (*From)[i+Start];
    }
}


void    LxStrFll( const QChar* From, QChar*& To, int LengthFrom, int LengthTo, int Start )
{
    int Length = LengthFrom;
    if( Length > LengthTo ) Length = LengthTo;

    for ( int i=0; i<Length; i++ )
    {
        To[i] = From[i+Start];
    }
}


void    LxStrFll( const QChar* From, QString& To, int LengthFrom, int LengthTo, int Start )
{
    int Length = LengthFrom;
    if( Length > LengthTo ) Length = LengthTo;

    for ( int i=0; i<Length; i++ )
    {
        To[i] = From[i+Start];
    }
}


void    LxStrFll( const QString& From, QChar*& To, int LengthFrom, int LengthTo, int Start )
{
    int Length = LengthFrom;
    if( Length > LengthTo ) Length = LengthTo;

    for ( int i=0; i<Length; i++ )
    {
        To[i] = From[i+Start];
    }
}


void    LxStrFll( const QString& From, QString& To, int LengthFrom, int LengthTo, int Start )
{
    int Length = LengthFrom;
    if( Length > LengthTo ) Length = LengthTo;

    for ( int i=0; i<Length; i++ )
    {
        To[i] = From[i+Start];
    }
}


//--------------------------------------------------------------------
// CStringSurrogate/QChar/QString comparisons
//--------------------------------------------------------------------


int      LxStrCmp( const CStringSurrogate* Str1, const CStringSurrogate* Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=0; i<length; i++ )
    {
        if ( (*Str1)[Start1+i] > (*Str2)[Start2+i] ) return 1;
        if ( (*Str1)[Start1+i] < (*Str2)[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}


int      LxStrCmp( const CStringSurrogate* Str1, const QChar* Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=0; i<length; i++ )
    {
        if ( (*Str1)[Start1+i] > Str2[Start2+i] ) return 1;
        if ( (*Str1)[Start1+i] < Str2[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}


int      LxStrCmp( const CStringSurrogate* Str1, const QString& Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=0; i<length; i++ )
    {
        if ( (*Str1)[Start1+i] > Str2[Start2+i] ) return 1;
        if ( (*Str1)[Start1+i] < Str2[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}


int      LxStrCmp( const QChar* Str1, const CStringSurrogate* Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=0; i<length; i++ )
    {
        if ( Str1[Start1+i] > (*Str2)[Start2+i] ) return 1;
        if ( Str1[Start1+i] < (*Str2)[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}


int      LxStrCmp( const QChar* Str1, const QChar* Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=0; i<length; i++ )
    {
        if ( Str1[Start1+i] > Str2[Start2+i] ) return 1;
        if ( Str1[Start1+i] < Str2[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}


int      LxStrCmp( const QChar* Str1, const QString& Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=0; i<length; i++ )
    {
        if ( Str1[Start1+i] > Str2[Start2+i] ) return 1;
        if ( Str1[Start1+i] < Str2[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}


int      LxStrCmp( const QString& Str1, const CStringSurrogate* Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=0; i<length; i++ )
    {
        if ( Str1[Start1+i] > (*Str2)[Start2+i] ) return 1;
        if ( Str1[Start1+i] < (*Str2)[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}


int      LxStrCmp( const QString& Str1, const QChar* Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=0; i<length; i++ )
    {
        if ( Str1[Start1+i] > Str2[Start2+i] ) return 1;
        if ( Str1[Start1+i] < Str2[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}


int      LxStrCmp( const QString& Str1, const QString& Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=0; i<length; i++ )
    {
        if ( Str1[Start1+i] > Str2[Start2+i] ) return 1;
        if ( Str1[Start1+i] < Str2[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}


int      LxStrCmp_R( const CStringSurrogate* Str1, const CStringSurrogate* Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=length-1; i>=0; i-- )
    {
        if ( (*Str1)[Start1+i] > (*Str2)[Start2+i] ) return 1;
        if ( (*Str1)[Start1+i] < (*Str2)[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}


int      LxStrCmp_R( const CStringSurrogate* Str1, const QChar* Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=length-1; i>=0; i-- )
    {
        if ( (*Str1)[Start1+i] > Str2[Start2+i] ) return 1;
        if ( (*Str1)[Start1+i] < Str2[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}


int      LxStrCmp_R( const CStringSurrogate* Str1, const QString& Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=length-1; i>=0; i-- )
    {
        if ( (*Str1)[Start1+i] > Str2[Start2+i] ) return 1;
        if ( (*Str1)[Start1+i] < Str2[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}


int      LxStrCmp_R( const QChar* Str1, const CStringSurrogate* Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=length-1; i>=0; i-- )
    {
        if ( Str1[Start1+i] > (*Str2)[Start2+i] ) return 1;
        if ( Str1[Start1+i] < (*Str2)[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}


int      LxStrCmp_R( const QChar* Str1, const QChar* Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=length-1; i>=0; i-- )
    {
        if ( Str1[Start1+i] > Str2[Start2+i] ) return 1;
        if ( Str1[Start1+i] < Str2[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}


int      LxStrCmp_R( const QChar* Str1, const QString& Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=length-1; i>=0; i-- )
    {
        if ( Str1[Start1+i] > Str2[Start2+i] ) return 1;
        if ( Str1[Start1+i] < Str2[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}


int      LxStrCmp_R( const QString& Str1, const CStringSurrogate* Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=length-1; i>=0; i-- )
    {
        if ( Str1[Start1+i] > (*Str2)[Start2+i] ) return 1;
        if ( Str1[Start1+i] < (*Str2)[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}


int      LxStrCmp_R( const QString& Str1, const QChar* Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=length-1; i>=0; i-- )
    {
        if ( Str1[Start1+i] > Str2[Start2+i] ) return 1;
        if ( Str1[Start1+i] < Str2[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}


int      LxStrCmp_R( const QString& Str1, const QString& Str2, int Length1, int Length2, int Start1, int Start2 )
{
    Q_ASSERT( Length1 >= 0 && Length2 >= 0 );
    int length = Length1;
    if( Length1 > Length2 ) length = Length2;

    for ( int i=length-1; i>=0; i-- )
    {
        if ( Str1[Start1+i] > Str2[Start2+i] ) return 1;
        if ( Str1[Start1+i] < Str2[Start2+i] ) return -1;
    }
    if ( Length1 > Length2 ) return 1;
    if ( Length1 < Length2 ) return -1;
    return 0;
}



//--------------------------------------------------------------------
// CStringSurrogate/QChar/QString concatenations
//--------------------------------------------------------------------

void    LxStrCat( QChar*& Front, const CStringSurrogate* Back, int LengthF, int LengthB, int StartF, int StartB )
{
    int length = LengthF + LengthB,
            i;
    QChar* NewQChar = new QChar[ length ];
    for(  i=0; i<LengthF; i++ )
    {
        NewQChar[i] = Front[StartF+i];
    }
    for( i=0; i<LengthB; i++ )
    {
        NewQChar[LengthF+i] = (*Back)[StartB+i];
    }
    delete [] Front;
    Front = NewQChar;
}


void    LxStrCat( QChar*& Front, const QChar* Back, int LengthF, int LengthB, int StartF, int StartB )
{
    int length = LengthF + LengthB,
            i;
    QChar* NewQChar = new QChar[ length ];
    for(  i=0; i<LengthF; i++ )
    {
        NewQChar[i] = Front[StartF+i];
    }
    for( i=0; i<LengthB; i++ )
    {
        NewQChar[LengthF+i] = Back[StartB+i];
    }
    delete [] Front;
    Front = NewQChar;
}

void    LxStrCat( QChar*& Front, const QString& Back, int LengthF, int LengthB, int StartF, int StartB )
{
    int length = LengthF + LengthB,
            i;
    QChar* NewQChar = new QChar[ length ];
    for( i=0; i<LengthF; i++ )
    {
        NewQChar[i] = Front[StartF+i];
    }
    for( i=0; i<LengthB; i++ )
    {
        NewQChar[LengthF+i] = Back[StartB+i];
    }
    delete [] Front;
    Front = NewQChar;
}


void    LxStrCat( QString& Front, const CStringSurrogate* Back, int LengthF, int LengthB, int StartF, int StartB )
{
    Front = Front.mid( StartF, LengthF );
    for( int i=0; i<LengthB; i++ )
    {
        Front += (*Back)[StartB+i];
    }
}


void    LxStrCat( QString& Front, const QChar* Back, int LengthF, int LengthB, int StartF, int StartB )
{
    Front = Front.mid( StartF, LengthF );
    for( int i=0; i<LengthB; i++ )
    {
        Front += Back[StartB+i];
    }
}

void    LxStrCat( QString& Front, const QString& Back, int LengthF, int LengthB, int StartF, int StartB )
{
    Front = Front.mid( StartF, LengthF ) + Back.mid( StartB, LengthB );
}




//--------------------------------------------------------------------
// QChar/QString mutators
//--------------------------------------------------------------------


void    LxStrLwr( QChar* str, int Length, int Start )
{
    for( int i=Start; i<Length; i++ )
    {
        str[i] = str[i].toLower();
    }
}

void    LxStrLwr( QString& str )
{
    str = str.toLower();
}

void    LxStrUpr( QChar* str, int Length, int Start )
{
    for( int i=Start; i<Length; i++ )
    {
        str[i] = str[i].toUpper();
    }
}

void    LxStrUpr( QString& str )
{
    str = str.toUpper();
}


/// For Rich Morphy Init Test
int		OverlapOfTwoAlphabetizedLists( QString str1, QString str2)
{
    int		Count = 0;
    int		i = 0, j= 0;
    QChar	str1c, str2c;

    while (i <= (int)str1.length() && j <= (int) str2.length()  )
    {
        str1c = str1[i];
        str2c = str2[j];
        if (str1c == str2c )
        {
            Count++;
            i++; j++; continue;
        }
        if (str1c < str2c )
        {
            i++;
            continue;
        }
        if (str1c > str2c )
        {
            j++;
            continue;
        }
    }
    return Count;


}
int		DifferencesOfTwoAlphabetizedLists( QString str1, QString str2)
{

    int Count = 0;
    int i = 0, j= 0;
    QChar str1c, str2c;

    while (i <= (int) str1.length() || j <= (int) str2.length()  )
    {
        if ( i >= static_cast <int> ( str1.length() ) )
        {
            if ( j >= static_cast <int> (str2.length()) ) break;

            Count++;
            j++;
            continue;
        }
        if ( j >= static_cast <int> (str2.length()) )
        {
            if ( i >= static_cast <int> (str1.length()) ) break;

            Count++;
            i++;
            continue;
        }

        str1c = str1[i];
        str2c = str2[j];

        if (str1c == str2c )
        {
            i++; j++; continue;
        }
        if (str1c < str2c )
        {
            i++;
            Count++;
            continue;
        }
        if (str1c > str2c )
        {
            j++;
            Count++;
            continue;
        }
    }
    return Count;

}

// conversions to string.
QString IntToStringWithCommas( int n )
{
    QString Number, comma = ",", WithCommas = "";
    Number = QString("%1").arg(n).trimmed();
    int commacount = (Number.length()-1)/3;
    for( int i=1; i<=commacount; i++ )
    {
        WithCommas = comma + Number.right( 3 ) + WithCommas;
        Number = Number.left( Number.length() - 3 );
    }
    return Number + WithCommas;
}

QString DblToStringWithCommas( double d )
{
    QString number, decimals, comma = ",", withCommas = "";
    number = QString("%1").arg( d, 0, 'f', 3 ).trimmed();

    int pointLoc = number.indexOf('.');

    decimals = number.mid( pointLoc );
    number = number.left( pointLoc );

    int commacount = (pointLoc-1)/3;
    for( int i=1; i<=commacount; i++ )
    {
        withCommas = comma + number.right( 3 ) + withCommas;
        number = number.left( number.length() - 3 );
    }
    return number + withCommas + decimals;
}
QString Filter( StringToString* filter, QString string )
{

//    // Find the length of longest filter string
//    int longest = 0;
//    StringToString::Iterator it;
//    for( it = filter->begin(); it != filter->end(); ++it )
//        if( longest < (int)it.key().length() ) longest = it.key().length();

//    // Filter from longest to shortest
//    QString filtered = string;
//    for( int i = longest; i >= 0; i-- )
//    {
//        for( it = filter->begin(); it != filter->end(); ++it )
//        {
//            if( (int)it.key().length() == i ) filtered.replace( it.key(), it->data() );
//        }
//    }
//    return filtered;
    int i = filter->count();
    i++;
    i--;
    return string;
}
QString DisplayInt ( int n )
{
    QString  Result;

    Result.arg( n );
    return Result;
}
QString DisplayFloat ( float f )
{
    QString Result;

    Result.arg( f, 0, 'f', 3 );

    return Result;

}
