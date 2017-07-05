#include "StringSurrogate.h"

#include "Parse.h"
//#include "Trie.h"       LEAVING THIS COMMENTED OUT FOR NOW; NEEDS TO BE REMOVED
#include "StringFunc.h"

//--------------------------------------------------------------------
// Construction/Destruction
//--------------------------------------------------------------------


/**
    Constructs an empty string surrogate.
 */

CStringSurrogate::CStringSurrogate()
{
    m_Key = NULL;
    m_Start = 0;
    m_Length = 0;
    m_Backwards = false;
}


/**
    Constructs a string surrogate from a QChar array. <kbd>Key</kbd> points to the
    parent string. <kbd>StartIndex</kbd> is the index of the first character in the
    surrogate substring. <kbd>Length</kbd> is the length of the substring. Set <kbd>Backwards</kbd>
    equal to <i>TRUE</i> if all string operations should be performed in reverse.

    Examples:
        QString hello_world = "Hello, world.";
        CStringSurrogate s(hello_world, 3, 8, false);	// "lo, worl"
        CStringSurrogate s2(hello_world, 0, 9, true);	// "ow ,olleH"

        CStringSurrogate empty1(0, 0, 0, false);	// empty
        CStringSurrogate empty2(0, 0, 0, true);	// empty
        CStringSurrogate empty3(new QChar[0], 0, 0, false);	// empty, with memory leak
        CStringSurrogate empty4(new QChar[15], 7, 0, true);	// empty, with memory leak
 */

CStringSurrogate::CStringSurrogate( const QChar* Key, const int StartIndex, const int Length, bool Backwards ):
        m_Key( Key ),
        m_Start( StartIndex ),
        m_Length( Length ),
        m_Backwards( Backwards )
{}

CStringSurrogate::CStringSurrogate( const QChar* s, int len )
    : m_Key(s),
      m_Start(0),
      m_Length(len),
      m_Backwards(false)
{ Q_ASSERT(s != 0 && len >= 0); }

/**
    Constructs a string surrogate from a CParse. <kbd>Parse</kbd> is a reference to the
    parent string. <kbd>StartIndex</kbd> is the index of the first character in the
    surrogate substring. <kbd>Length</kbd> is the length of the substring. If <kbd>Length</kbd> = -1,
    the substring will continue to the end of the parent string. Set <kbd>Backwards</kbd>
    equal to <i>TRUE</i> if all string operations should be performed in reverse.
 */

CStringSurrogate::CStringSurrogate( const CParse& Parse, const int StartIndex, const int Length, bool Backwards )
{
    m_Key = Parse.GetKeyPointer();
    m_Start = StartIndex;
    if( Length < 0 ) m_Length = Parse.GetKeyLength() - StartIndex;
    else m_Length = Length;
    m_Backwards = Backwards;
}


/**
    Constructs a string surrogate from a pointer to a CParse. <kbd>pParse</kbd> is a pointer to the
    parent string. <kbd>StartIndex</kbd> is the index of the first character in the
    surrogate substring. <kbd>Length</kbd> is the length of the substring. If <kbd>Length</kbd> = -1,
    the substring will continue to the end of the parent string. Set <kbd>Backwards</kbd>
    equal to <i>TRUE</i> if all string operations should be performed in reverse.
 */

CStringSurrogate::CStringSurrogate( const CParse* pParse, const int StartIndex, const int Length, bool Backwards  )
{
    m_Key = pParse->GetKeyPointer();
    m_Start = StartIndex;
    if( Length < 0 ) m_Length = pParse->GetKeyLength() - StartIndex;
    else m_Length = Length;
    m_Backwards = Backwards;
}

//TAPAN; NODE REF: Commented out till we know how to replace nodes better
/**
    Constructs a string surrogate from a pointer to a trie node. <kbd>pNode</kbd> is a pointer to node.
    Set <kbd>Backwards</kbd> equal to <i>TRUE</i> if all string operations should be performed in reverse.
 */
//CStringSurrogate::CStringSurrogate( CNode* pNode, bool Backwards )
//{
//    m_Key = pNode->GetKeyPointer();
//    if( m_Key )
//    {
//        m_Length = pNode->GetKeyLength();
//        m_Start = 0;
//    }
//    else
//    {
//        m_Length = 0;
//        m_Start = 0;
//    }
//    m_Backwards = Backwards;
//}


/**
    Constructs a string surrogate from a QString. <kbd>String</kbd> is a pointer to the
    parent string. <kbd>Start</kbd> is the index of the first character in the
    surrogate substring. <kbd>Length</kbd> is the length of the substring. If <kbd>Length</kbd> = -1,
    the substring will continue to the end of the parent string. Set <kbd>Backwards</kbd>
    equal to <i>TRUE</i> if all string operations should be performed in reverse.

    Note: Use carefully! If the QString is changed, the QChar* of the string may be in a new location
    and this surrogate will not point to the right memory location anymore.
 */


CStringSurrogate::CStringSurrogate( const QString String, const int Start, const int Length, bool Backwards )
{
    m_Key = String.unicode();
    m_Start = Start;
    if( Length < 0 ) m_Length = String.length();
    else m_Length = Length;
    m_Backwards = Backwards;
}


/**
    Copies another string surrogate. <kbd>other</kbd> is the string surrogate to be copied.
 */

CStringSurrogate::CStringSurrogate( const CStringSurrogate& other )
{
    m_Key = other.GetKey();
    m_Start = other.GetStart();
    m_Length = other.GetLength();
    m_Backwards = other.IsBackwards();
}


/**
    Destroys this string surrogate. Because surrogates do not own any string
    data, this destructor does nothing.
 */

CStringSurrogate::~CStringSurrogate()
{}


//--------------------------------------------------------------------
// Overloaded Operators
//--------------------------------------------------------------------

// Gets the nth QChar from the string surrogate.
//
// Paramaters:
//		- n - the desired index
//
// Returns:
//		- QChar - the nth QChar in m_Key after m_Start, NULL if n is too large or too small
//
/**
    Returns the the nth QChar from the string surrogate. <kbd>n</kbd> is the index
    of the desired character in the surrogate.
 */

QChar CStringSurrogate::operator[] (const int n ) const
{
    return GetChar(n);
}


// Sets the string surrogate equal to the key in a CParse.
//
// Parameters:
//		- Parse - contains a QChar array called 'm_Key'
//
/**
    Makes a string surrogate from a CParse. <kbd>Parse</kbd> is the parent CParse.
 */
void CStringSurrogate::operator= ( const CParse& Parse )
{
    m_Key = Parse.GetKeyPointer();
    m_Start = 0;
    m_Length = Parse.GetKeyLength();
    m_Backwards = false;
}


// Copies another string surrogate.
//
// Parameters:
//		- const CStringSurrogate - to be copied
//
/**
    Makes a copy of another string surrogate. <kbd>other</kbd> is the surrogate
    that will be copied.
 */

void CStringSurrogate::operator= ( const CStringSurrogate& other )
{
    m_Key = other.GetKey();
    m_Start = other.GetStart();
    m_Length = other.GetLength();
    m_Backwards = other.IsBackwards();
}


// Sets the string surrogate equal to the unicode QChar array
// of a QString.
//
// Note: Use carefully! If the QString is changed the
// QChar* of the string may be in a new location and this
// surrogate will not point to the right memory location
// anymore.
//
// Parameters:
//		- String - contains a QChar array called 'unicode'
//
/**
    Makes a surrogate of a QString. <kbd>String</kbd> is the parent
    QString.

    Note: Use carefully! If the QString is changed the QChar* of the
    string may be in a new location and this surrogate will not
    represent the same string.
 */

void CStringSurrogate::operator= ( const QString String )
{
    m_Key = String.unicode();
    m_Start = 0;
    m_Length = String.length();
    m_Backwards = false;
}


// Sets the string surrogate equal to the key in a CParse*
//
// Parameters:
//		- Parse - contains a QChar array called 'm_Key'
//
/**
    Makes a string surrogate of a pointer to a CParse. <kbd>pParse</kbd>
    pointer to the parent CParse.
 */
void CStringSurrogate::operator= ( const CParse* pParse )
{ operator= ( *pParse ); }


// Checks if this string surrogate does not have the same characters
// as another string surrogate.
//
// Parameters:
//		- SS - the other string surrogate
//
// Returns:
//		- bool - true if any QChar in SS does not match the corresponding QChar in the string surrogate from m_Start to m_Start + m_Length
//
/**
    Checks if this string surrogate does not have the same characters
    as another string surrogate. <kbd>SS</kbd> is the other surrogate
    to compare.
 */

bool CStringSurrogate::operator!= ( const CStringSurrogate& SS ) const
{ return !operator== (SS); }


// Checks if this string surrogate does not have the same characters
// as a CParse.
//
// Parameters:
//		- Parse - CParse has a member m_Key which is an array of type QChar
//
// Returns:
//		- bool - true if any QChar in Parse.m_Key does not match the corresponding QChar in the string surrogate from m_Start to m_Start + m_Length
//
/**
    Checks if this string surrogate does not have the same characters
    as a CParse. <kbd>Parse</kbd> is the CParse to compare.
 */

bool CStringSurrogate::operator!= ( const CParse& Parse ) const
{ return !operator== (Parse); }


// Combines this string surrogate and another into a CParse.
// No change is made to this surrogate or the other.
//
// Parameters:
//		- ssRight - the surrogate that will be appended
//
// Returns:
//		- CParse - a copy of this surrogate plus ssRight
//

/**
    Combines this string surrogate and another into
    a CParse. No change is made to either string surrogate.
    <kbd>ssRight</kbd> is the surrogate to the right of the operator.
 */

CParse  CStringSurrogate::operator+ ( const CStringSurrogate& ssRight ) const
{
    CParse parse ( *this );
    parse.Append (ssRight);
    return parse;
}


// Combines this string surrogate and a QChar into a CParse,
// no change is made to this surrogate.
//
// Parameters:
//		- c - the QChar that will be appended
//
// Returns:
//		- CParse - a copy of this surrogate plus c
//
/**
    Combines this string surrogate and a single QChar into a CParse. No
    change is made to this surrogate. <kbd>c</kbd> is the character to the
    right of the operator.
 */

CParse  CStringSurrogate::operator+ ( const QChar c ) const
{
    CParse parse (*this);
    parse.Append (c);
    return parse;
}


// Checks if this string surrogate has the same characters
// as another string surrogate.
//
// Parameters:
//		- SS - the other string surrogate
//
// Returns:
//		- bool - true if every QChar in SS matches every QChar in the string surrogate from m_Start to m_Start + m_Length
//
/**
    Compare the characters of this string surrogate with those of
    another surrogate. <kbd>SS</kbd> is the other surrogate (to the
    right of the operator).
 */

bool CStringSurrogate::operator== ( const CStringSurrogate& SS ) const
{
    if ( SS.GetLength() != m_Length ) return false;

    for ( int i=0; i<m_Length; i++ )
    {
        if ( GetChar(i) != SS[i] )
        {
            return false;
        }
    }
    return true;
}


// Checks if this string surrogate has the same characters
// as a CParse.
//
// Parameters:
//		- Parse - CParse has a member m_Key which is an array of type QChar
//
// Returns:
//		- bool - true if every QChar in Parse.m_Key matches every QChar in the string surrogate from m_Start to m_Start + m_Length
//

/**
    Compares the characters of this surrogate with the characters
    of a CParse. <kbd>Parse</kbd> is the CParse to compare with.
 */

bool CStringSurrogate::operator== ( const CParse& Parse ) const
{
    CStringSurrogate SS = Parse.GetKey();
    return (*this) == SS;
}

// Checks if this string surrogate is alphabetically
// greater than another string surrogate.
//
// Parameters:
//		- SS - the other string surrogate
//
// Returns:
//		- bool - true if this surrogate is alphabetically greater than SS, this includes when they are equal from m_Start through m_Start + m_Length and this surrogate is shorter than SS
//

/**
    Compares the alphabetical value of this string surrogate with
    another, returns TRUE if this is greater. <kbd>SS</kbd> is the
    other string surrogate (on the right side of the operator).
 */

bool CStringSurrogate::operator> ( const CStringSurrogate& SS ) const
{
    int length = m_Length;
    if( length > SS.GetLength() ) length = SS.GetLength();

    for ( int i=0; i<length; i++ )
    {
        if ( GetChar(i) < SS[i] ) return false;
        else if( GetChar(i) > SS[i] ) return true;
    }
    if( m_Length <= SS.GetLength() ) return false;
    else return true;
}


// Checks if this string surrogate is alphabetically
// less than another string surrogate.
//
// Parameters:
//		- SS - the other string surrogate
//
// Returns:
//		- bool - true if this surrogate is alphabetically less than SS, this includes when they are equal from m_Start through m_Start + m_Length and this surrogate is longer than SS
//

/**
    Compares the alphabetical value of this string surrogate with
    another, returns TRUE if this is the lesser. <kbd>SS</kbd> is the
    other string surrogate (on the right side of the operator).
 */

bool CStringSurrogate::operator<  ( const CStringSurrogate& SS ) const
{
    int length = m_Length;
    if( length > SS.GetLength() ) length = SS.GetLength();

    for ( int i=0; i<length; i++ )
    {
        if ( GetChar(i) > SS[i] ) return false;
        else if( GetChar(i) < SS[i] ) return true;
    }
    if( m_Length < SS.GetLength() ) return true;
    else return false;
}


//-----------------------------------------------------------------
// Other Methods
//-----------------------------------------------------------------


// Returns the parent string key.
//
/**
    Returns a pointer to the parent's QChar string.
 */
const QChar* CStringSurrogate::GetKey() const
{ return m_Key; }

// Returns the length of the substring.
//
/**
    Returns the length of the surrogate.
 */
int CStringSurrogate::GetLength() const
{ return m_Length; }

// Returns the start position of the substring.
//
/**
    Returns the start position of the surrogate relative
    to the beginning of the parent string.
 */
int CStringSurrogate::GetStart() const
{ return m_Start; }

// Finds out if the substring is backwards.
//
/**
    Returns the state of the Backwards flag.
 */
bool CStringSurrogate::IsBackwards() const
{ return m_Backwards; }

 // Designates the surrogate as a backwards substring.
 //
/**
    Sets the <i>Backwards</i> state of the surrogate.
 */
void CStringSurrogate::SetBackwards( bool b )
{ m_Backwards = b; }

// Gets the nth QChar from the string surrogate.
//
// Paramaters:
//		- n - the desired index
//
// Returns:
//		- QChar - the nth QChar in m_Key after m_Start, NULL if n is too large or too small
//
/**
    Gets the nth QChar from the string surrogate. <kbd>n</kbd> is the
    index of the desired character. Returns <i>NULL</i> if n is out of
    range.
 */

QChar CStringSurrogate::GetChar( const int n ) const
{
    if ( n >= 0 && n < (int)m_Length )
    {
        if ( m_Backwards )
            return m_Key[ m_Start + m_Length - n - 1 ];
        else
            return m_Key[ m_Start + n ];
    } else
        return QChar(0);
}

// Finds out if this string surrogate is a single punctuation mark.
//
// Returns:
//		- bool - true if string surrogate is one character and that character is a punctuation
//
/**
    Checks if this surrogate is a single punctuation mark.
 */

bool CStringSurrogate::IsPunctuation( QString punctuation ) const
{
    if( punctuation == "EMPTY" ) punctuation = "";

    if ( m_Length == 1)
    {
        QString punc = ".";
        if (punctuation.indexOf(punc) > -1) {
            return true;
        }
        //        return ( punctuation.find( m_Key[ m_Start ] ) > -1 || m_Key[m_Start].isPunct() );
    }
    return false;
}


// Makes another surrogate of the left n QChars in this surrogate.
//
// Parameters:
//		- n - number of desired QChars
//
// Returns:
//		- CStringSurrogate - a different surrogate with just the left n QChars; the new surrogate points to the same QChar array but has different m_Start and m_Length values
//
/**
    Makes another surrogate of the left n QChars in this surrogate. <kbd>n</kbd>
    is the length of the new surrogate.
 */

CStringSurrogate CStringSurrogate::Left (int n) const
{
  if( n >= 0 )
  {
    if( n <= (int)m_Length )
    {
      if( m_Backwards )
        return CStringSurrogate( m_Key, m_Start+m_Length-n, n, m_Backwards );
      else
        return CStringSurrogate( m_Key, m_Start, n, m_Backwards );
    }
    else
      return CStringSurrogate( m_Key, m_Start, m_Length, m_Backwards );
  }
  else
    return CStringSurrogate();
}


// Makes another surrogate of the right n QChars in this
// surrogate.
//
// Parameters:
//		- n - number of desired QChars
//
// Returns:
//		- CStringSurrogate - a different surrogate with just the right n QChars; the new surrogate points to the same QChar array but has different m_Start and m_Length values
//
/**
    Makes another surrogate of the right n QChars in this surrogate. <kbd>n</kbd>
    is the length of the new surrogate.
 */

CStringSurrogate CStringSurrogate::Right (int n) const
{
    if( n >= 0 )
    {
        if( n <= (int)m_Length )
        {
            if( m_Backwards )
                return CStringSurrogate( m_Key, m_Start, n, m_Backwards );
            else
                return CStringSurrogate( m_Key, m_Start+m_Length-n, n, m_Backwards );
        }
        else
            return CStringSurrogate( m_Key, m_Start, m_Length, m_Backwards );
    }
    else
        return CStringSurrogate();
}

// Makes another surrogate of the middle SpanLength QChars
// in this surrogate, starting at character SkipHowMany.
//
// Parameters:
//		- SkipHowMany - starting index, also number of QChars skipped SpanLength - the number of characters to include, if negative or too large, the remainder of the string following SkipHowMany will be returned, default = -1
//
// Returns:
//		- CStringSurrogate - a different surrogate made from the same QChar array with m_Start at SkipHowMany plus this.m_Start and m_Length = SpanLength
//
/**
    Makes another surrogate of QChars from the middle of this surrogate. <kbd>SkipHowMany</kbd> is the
    starting position of the new sub-string. <kbd>SpanLength</kbd> is the length of the sub-string.
 */

CStringSurrogate CStringSurrogate::Mid ( int SkipHowMany, int SpanLength ) const
{
    if ( SpanLength < 0  || SkipHowMany + SpanLength > (int)m_Length )
    {
        // Readjust SpanLength to be the remainder
        // of this surrogate
        SpanLength = m_Length - SkipHowMany;
    }

    if ( SkipHowMany >= 0 && SkipHowMany < (int)m_Length )
        if( m_Backwards ) return CStringSurrogate( m_Key, m_Start+m_Length-SkipHowMany-SpanLength, SpanLength, m_Backwards );
        else return CStringSurrogate ( m_Key, m_Start+SkipHowMany, SpanLength, m_Backwards );

    else return CStringSurrogate();
}

// Returns a copy of the first character in the surrogate.
//
// Returns:
//		- QChar - the character at m_Key[m_Start]
//
/**
    Returns a copy of the first character in the surrogate.
 */

QChar CStringSurrogate::FirstChar() const
{
    return GetChar(m_Start);
}


// Returns a copy of the last character in the surrogate.
//
// Returns:
//		- QChar - the character at m_Key[m_Start+m_Length-1]
//
/**
    Returns a copy of the last character in the surrogate.
 */

QChar CStringSurrogate::LastChar() const
{
    return GetChar(m_Start + m_Length-1);
}


// Returns a QString copy of the surrogate.
//
// Returns:
//		- QString - a real string copy of the surrogate
//
/**
    Makes a QString copy of the surrogate for displaying and
    printing. <kbd>filter</kbd> is a map of individual characters
    to strings of multiple characters that will replace them
    in the output.
 */

QString CStringSurrogate::Display( StringToString* filter ) const
{
    QString string = "";
    for ( int i=0; i<m_Length; i++ )
    {
        string += GetChar(i);
    }

    if( filter ) return Filter( filter, string );
    else return string;
}


// Makes the string surrogate empty (NULL).
//
/**
    Makes the string surrogate empty (NULL).
 */

void CStringSurrogate::MakeNull() {
    m_Key = NULL;
    m_Start = 0;
    m_Length = 0;
    m_Backwards = false;
}


// Converts a surrogate that has only the characters "NULL"
// into an empty (NULL) string surrogate.
//
/**
    Converts this surrogate into into an empty surrogate if it
    only has the characters "NULL".
 */

void CStringSurrogate::ConvertNULL()
{
    if ( IsNULL() )  MakeNull();
}


// Finds out if the surrogate has only the characters "NULL".
//
// Returns:
//		- bool - true if the surrogate's length is 4 and it is equal to "NULL"
//

/**
    Finds out if this surrogate only has the characters "NULL".
 */

bool CStringSurrogate::IsNULL() const
{
    QString Null = "NULL";
    if ( m_Length == 4 && operator== ( CStringSurrogate(Null) ) )
    {
        return true;
    }
    return false;
}
