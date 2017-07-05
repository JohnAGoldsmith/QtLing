#include "Parse.h"

#include <algorithm>
#include <functional>
#include <QTextStream>
#include <QStringList>
#include <QList>
#include "CompareFunc.h"
#include "StringFunc.h"
#include "log2.h"
#include "Typedefs.h"

namespace {
    const std::size_t initial_piece_vector_len = 4;
}
double g_Lambda = base2log (26);

// construction/destruction.

/**
    Constructs an empty CParse. <kbd>DefaultPieceValue</kbd> is the default
    value to be used when populating the general use piece values array.
 */
CParse::CParse(double DefaultPieceValue)
    : m_Key(),
    m_AllocatedLength(0),
    m_KeyLength(0),

    m_Pieces(new int[initial_piece_vector_len]),	// initialized below
    m_PieceValues(new double[initial_piece_vector_len]),	// initialized below
    m_PieceCount(0),
    m_LengthOfPieceVector(initial_piece_vector_len),

    m_DefaultPieceValue(DefaultPieceValue),
    m_SortStyle(eNotSorted),
    m_Evaluation(0.0)
{
    m_Pieces[0] = 0;
    m_PieceValues[0] = m_DefaultPieceValue;
}

// Construct a CParse copy of another CParse
//
// Parameters:
//		- Parse - the Parse to be copied
//		- DefaultPieceValue - the default piece value
//
/**
    Constructs a copy of another CParse. <kbd>Parse</kbd> is the CParse to be copied.
    <kbd>DefaultPieceValue</kbd> is the default
    value to be used when populating the general use piece values array.
    <kbd>bAlphabetical</kbd> designates whether the pieces are kept in
    alphabetical order.
 */
CParse::CParse(const CParse& Parse, double DefaultPieceValue)
    : m_Key(),	// initialized below
    m_AllocatedLength(0),	// initialized below
    m_KeyLength(0),	// initialized below

    m_Pieces(),	// initialized below
    m_PieceValues(),	// initialized below
    m_PieceCount(0),	// initialized below
    m_LengthOfPieceVector(0),	// initialized below

    m_DefaultPieceValue(DefaultPieceValue),
    m_SortStyle(eNotSorted),
    m_Evaluation(0.0)
{
    // XXX. Better to copy key /without/ destroying parse structure.
    SetKey(Parse);
    m_PieceCount = Parse.Size();
    SetLengthOfPieceVector(m_PieceCount + 1);
    m_Pieces = new int[m_LengthOfPieceVector];
    m_PieceValues = new double[m_LengthOfPieceVector];

    std::copy(&Parse.m_Pieces[0],
        &Parse.m_Pieces[m_PieceCount + 1],
        &m_Pieces[0]);
    std::copy(&Parse.m_PieceValues[0],
        &Parse.m_PieceValues[m_PieceCount + 1],
        &m_PieceValues[0]);
}

// Construct a CParse using a single QChar
//
// Parameters:
//		- c - the QChar to be copied
//		- DefaultPieceValue - the default piece value
//
/**
    Constructs a CParse copy of a single QChar. <kbd>c</kbd> is the QChar to be copied.
    <kbd>DefaultPieceValue</kbd> is the default
    value to be used when populating the general use piece values array.
    <kbd>bAlphabetical</kbd> designates whether the pieces are kept in
    alphabetical order.
 */
CParse::CParse(const QChar& c, double DefaultPieceValue)
    : m_Key(new QChar[1]),	// filled below
    m_AllocatedLength(1),
    m_KeyLength(1),

    m_Pieces(new int[initial_piece_vector_len]),	// initialized below
    m_PieceValues(new double[initial_piece_vector_len]),	// initialized below
    m_PieceCount(1),
    m_LengthOfPieceVector(initial_piece_vector_len),

    m_DefaultPieceValue(DefaultPieceValue),
    m_SortStyle(eNotSorted),
    m_Evaluation(0.0)
{
    m_Key[0] = c;
    m_Pieces[0] = 0;
    m_Pieces[1] = 1;
    m_PieceValues[0] = m_DefaultPieceValue;
    m_PieceValues[1] = m_DefaultPieceValue;
}

// Construct a CParse using a string surrogate
//
// Parameters:
//		- c - the QChar to be copied
//		- DefaultPieceValue - the default piece value
//
/**
    Constructs a CParse copy of a string surrogate. <kbd>SS</kbd> is the surrogate to be copied.
    <kbd>DefaultPieceValue</kbd> is the default
    value to be used when populating the general use piece values array.
    <kbd>bAlphabetical</kbd> designates whether the pieces are kept in
    alphabetical order.
 */
CParse::CParse (const CStringSurrogate& SS, double DefaultPieceValue)
    : m_Key(),	// initialized below
    m_AllocatedLength(0),	// initialized below
    m_KeyLength(0),	// initialized below

    m_Pieces(),	// initialized below
    m_PieceValues(),	// initialized below
    m_PieceCount(0),	// initialized below
    m_LengthOfPieceVector(0),	// initialized below

    m_DefaultPieceValue(DefaultPieceValue),
    m_SortStyle(eNotSorted),
    m_Evaluation(0.0)
{
    // XXX. Better to copy key /without/ destroying parse structure.
    SetKey(SS);
    m_Pieces = new int[initial_piece_vector_len];
    {
        m_Pieces[0] = 0;
        m_Pieces[1] = SS.GetLength();
    }
    m_PieceValues = new double[initial_piece_vector_len];
    {
        m_PieceValues[0] = m_DefaultPieceValue;
        m_PieceValues[1] = m_DefaultPieceValue;
    }
    m_PieceCount = 1;
    m_LengthOfPieceVector = initial_piece_vector_len;
}

// Destroy this CParse
//
/**
    Destroys this CParse.
 */
CParse::~CParse() {
    delete[] m_Pieces;
    delete[] m_PieceValues;
    delete[] m_Key;
}

//--------------------------------------------------------------
// Copy utilities
//--------------------------------------------------------------


// Copy a string surrogate into m_Key
//
// Parameters:
//		- SS - the surrogate to be copied
//
/**
    Makes a CParse copy of a string surrogate. <kbd>SS</kbd> is the surrogate to be copied.
 */

void CParse::SetKey ( const CStringSurrogate SS )
{
  int Length = SS.GetLength();
//  QChar* key = new QChar[Length];
//  LxStrCpy( &SS, key, Length );
//  SetKey( key, ssLength ); // I copied the body of that function here:
  if (m_Key) { delete [] m_Key;}

  if ( Length > 0 )
  {

    m_AllocatedLength    = Length + 1;
    m_Key          = new QChar [m_AllocatedLength];
    m_KeyLength        = Length;
//  LxStrFll ( key, m_Key, m_KeyLength, m_AllocatedLength );
    LxStrFll ( &SS , m_Key, m_KeyLength, m_AllocatedLength );
  }
  else
  {
    m_Key = NULL;
    m_KeyLength = 0;
  }

  if(m_Pieces) SimplifyParseStructure();



}


// Copy a character into m_Key
//
// Parameters:
//		- ch - the character to copy
//
/**
    Makes a CParse copy of a QChar. <kbd>ch</kbd> is the character to be copied.
 */

void CParse::SetKey (const QChar& ch)
{
  SetKey( CStringSurrogate(&ch,0,1) );
}

// Copy a QChar array into m_Key
//
// Parameters:
//		- pQChar - location of the QChar array
//		- Length - length of the array or amount to be copied
//
/**
    Makes a CParse copy of a QChar array. <kbd>pQChar</kbd> is the array to be copied.
    <kbd>Length</kbd> is the length of substring of pQChar to be copied.
 */

void CParse::SetKey ( const QChar* pQChar, int Length )
{
  if (m_Key) { delete [] m_Key;}

  if ( Length > 0 )
  {

    m_AllocatedLength    = Length + 1;
    m_Key          = new QChar [m_AllocatedLength];
    m_KeyLength        = Length;
    LxStrFll ( pQChar, m_Key, m_KeyLength, m_AllocatedLength );
  }
  else
  {
    m_Key = NULL;
    m_KeyLength = 0;
  }

  if(m_Pieces) SimplifyParseStructure();
}


// Copy the key from another CParse into m_Key
//
// Parameters:
//		- Other - the CParse to copy from
//
/**
    Makes a CParse copy of another CParse. <kbd>Other</kbd> is the other CParse to be copied.
 */

void CParse::SetKey ( const CParse& Other )
{
  SetKey ( Other.GetKey() );
}



//--------------------------------------------------------------------
//  Overloaded operators
//--------------------------------------------------------------------

// Create a conjuct of this Parse and another
//
// Parameters:
//		- Parse2 - the second parse in the conjunct
//
// Returns:
//		- CParse - the conjunct CParse
//
/**
    Creates a conjunct of this CParse and another. Does not change this CParse.
    <kbd>Parse2</kbd> is a pointer to the other parse.
 */
CParse CParse::operator+ ( const CParse* Parse2 ) const
{
  CParse Sum = *this;
  Sum.Append( Parse2 );
  return Sum;
}


// Create a conjuct of this Parse and a string surrogate
//
// Parameters:
//		- SS - the string surrogate that will be the second part of the conjunct
//
// Returns:
//		- CParse - the conjunct CParse
//
/**
    Creates a conjunct of this CParse and a string surrogate. Does not change this CParse.
    <kbd>SS</kbd> is the string surrogate to be conjoined.
 */

CParse CParse::operator+ ( const CStringSurrogate SS ) const
{
  CParse Sum = *this;
  Sum.Append( SS );
  return Sum;
}



// Append another Parse and return the conjunct
//
// Parameters:
//		- Parse2 - the second parse in the conjunct
//
// Returns:
//		- CParse - a copy of this conjunct CParse
//
/**
    Appends another CParse to this one and also returns a copy.
    <kbd>Parse2</kbd> is a pointer to the CParse to be appended.
 */

CParse CParse::operator+= ( const CParse* Parse2 )
{
  Append ( Parse2 );
  return *this;
}


// Appemd a string surrogate and return the conjunct
//
// Parameters:
//		- Parse2 - the second parse in the conjunct
//
// Returns:
//		- CParse - a copy of this conjunct CParse
//
/**
    Appends a string surrogate and also returns a copy of the conjoined parse.
    <kbd>SS</kbd> is the string surrogate to be appended.
 */

CParse CParse::operator+= ( const CStringSurrogate SS )
{
  Append ( SS );
  return *this;
}


// Remove a piece matching a string surrogate
//
// Parameters:
//		- SS - a surrogate to remove
//
// Returns:
//		- CParse - a copy of this CParse with the piece removed
//
/**
    Removes a piece from the parse. <kbd>SS</kbd> is a string matching the
    piece to be removed.
 */
CParse  CParse::operator-= ( const CStringSurrogate SS )
{
  Remove( SS );
  return *this;
}


// Divide a one piece CParse into 2 at i
//
// Parameters:
//		- i - the index where the split will occur
//
/**
    Divides a one piece CParse into 2 pieces. <kbd>i</kbd> is the index
    before which the split will occur.
 */

void CParse::operator/= ( const int i )
{
  if( m_PieceCount == 1 || m_PieceCount == 0 )
  {
    if ( m_LengthOfPieceVector <= 2 )
    {
      SetLengthOfPieceVector(4);

      delete[] m_Pieces;
      m_Pieces = new int[m_LengthOfPieceVector];
      delete[] m_PieceValues;
      m_PieceValues = new double[m_LengthOfPieceVector];
    }
    m_PieceCount  = 2;
     m_Pieces [0]  = 0;
    m_Pieces [1]  = i;
    m_Pieces [2]  = m_KeyLength;
    m_PieceValues[0]= m_DefaultPieceValue;
    m_PieceValues[1]= m_DefaultPieceValue;
    m_PieceValues[2]= m_DefaultPieceValue;
  }
}


// Create a surrogate substring of this parse's key
//
// Parameters:
//		- n - the piece to make a substring of
//
// Returns:
//		- CStringSurrogate - the substring copy
//
/**
    Creates a string surrogate of one piece of the CParse. <kbd>n</kbd> is
    the piece which will be made into a surrogate.
 */
CStringSurrogate CParse::operator[] ( const int n ) const
{
  return GetPiece(n);
}


// Create a surrogate substring of this parse's key
//
// Parameters:
//		- From - the start index of the substring
//		- Length - the length of the substring
//
// Returns:
//		- CStringSurrogate - the substring copy
//
/**
    Create a string surrogate from substring. <kbd>From</kbd> is the
    starting index of the substring. <kbd>Length</kbd> the length of the substring.
 */

CStringSurrogate CParse::operator() ( const int From, const int Length ) const
{
  if ( From >= 0 && Length >= 0 && Length <= m_KeyLength - From +1 )  // JG Added +1 -- OK to take the last character of a string
    return CStringSurrogate( m_Key, From, Length );
  else return CStringSurrogate();
}


// Check if this Parse is equal to another
//
// Parameters:
//		- Other - the other parse to compare
//
// Returns:
//		- bool - true if this parse equals the Other by having the same string and same pieces, or by being the same object
//
/**
    Compare this CParse to another. <kbd>Other</kbd> is the other
    CParse to be compared. The comparison is performed on both the key and
    the piece array.
 */
bool CParse::operator== ( const CParse* Other ) const
{
  if ( this == Other ) { return true; }
  if ( Size() != Other->Size() ) { return false; }
  if ( GetKeyLength()  != Other->GetKeyLength() ) { return false; }

  for (int j = 0; j < GetKeyLength(); j++)
  {
    if ( m_Key[j] != Other->GetKey()[j] )
    {
      return false;
    }
  }

  for (int i = 1; i <= m_PieceCount; i++)
  {
    if (m_Pieces[i] != Other->GetPieces()[i] )
    {
      return false;
    }
  }
  return true;

}

/**
    Make a CParse copy of a string surrogate. <kbd>SS</kbd> is the
    string surrogate to be copied.
 */

CParse& CParse::operator=(const CStringSurrogate SS)
{ SetKey(SS); return *this; }

/**
    Append a string surrogate to this CParse. <kbd>SS</kbd> is the
    string surrogate to be copied.
 */
  void        CParse::operator<< ( const CStringSurrogate SS )
  { Append ( SS ); }

//-----------------------------------------------------------------
// Other methods
//-----------------------------------------------------------------


// Set a piece's value
//
// Parameters:
//		- n - the piece whose value will be set
//		- value - the new value
//
/**
    Set the value of a piece. Piece values are doubles and are
    for general use. It is often helpful to be able to give a qualitative
    or quantitative score to each piece. <kbd>n</kbd> is the index of the
    piece to be given the value. <kbd>value</kbd> is the new value.
 */
void CParse::SetPieceValue( int n, double value )
{
  if( n > 0 && n <= m_PieceCount )
    m_PieceValues[n] = value;
}


// Get a string surrogate of the entire key
//
// Returns:
//		- CStringSurrogate - a surrogate of m_Key
//
/**
    Get a surrogate of the entire CParse.
 */
CStringSurrogate CParse::GetKey() const
{
  if( m_KeyLength )
    return CStringSurrogate ( m_Key, 0, GetKeyLength() );
  else return CStringSurrogate ();
}


// Get a string surrogate of a piece of the parse
//
// Parameters:
//		- n - the piece number
//
// Returns:
//		- CStringSurrogate - a surrogate of piece n
//
/**
    Get a surrogate of one piece of the CParse. <kbd>n</kbd> is the
    index of the desired piece.
 */
CStringSurrogate CParse::GetPiece( int n ) const  // ::GetAt_QString
{
  if( m_Key && m_Pieces && n > 0 && n <= m_PieceCount )
    return CStringSurrogate ( m_Key, m_Pieces[n-1], ThisPieceLength(n) );
  else
    return CStringSurrogate();
}


// Get the value of a piece of the parse
//
// Parameters:
//		- n - the piece number
//
// Returns:
//		- double - the piece value
//
/**
    Get the value of a piece of the parse. <kbd>n</kbd> is the index of
    the desired piece.
 */
double CParse::GetPieceValue( int n ) const
{
  Q_ASSERT( n > 0 && n <= m_PieceCount );
  return m_PieceValues[n];
}


// Get the position of the first character of a piece
//
// Parameters:
//		- n - the piece number
//
// Returns:
//		- int - the index in m_Key of the first character of piece n, -1 if the index is invalid
//
/**
    Get the position of the first character of a piece. <kbd>n</kbd> is the piece
    in question.
 */
int  CParse::GetPositionOfFirstCharOfThisPiece( const int n ) const
{
  if( n > 0 && n <= m_PieceCount )
    return m_Pieces[n-1];
  else
    return -1;
}


// Change the break position between two pieces
//
// Parameters:
//		- Break - the index of the break to be updated in m_Pieces
//		- Val - the new value
//
/**
    Set the break position between two existing pieces. <kbd>Break</kbd> is
    the index of the second piece. <kbd>Val</kbd> is the string position
    where the new break will be located.
 */
void CParse::SetBreak ( const int Break, const int Val )  // ::SetAt
{
  if ( Break > 0 &&
    Break < m_PieceCount &&
    m_Pieces[Break-1] < Val &&
    m_Pieces[Break+1] &&
    Val < m_Pieces[Break+1] )
  {
    m_Pieces[Break] = Val;
  }
}


// Set this CParse equal to another
//
// Parameters:
//		- Other - the CParse to be copied
//
/**
    Set this CParse equal to another. <kbd>Other</kbd> is the
    CParse to be copied.
 */
void CParse::CopyParse ( const CParse& Other )
{
  if ( this == &Other )
  {
    return;
  }
  else if (Other.Size() == 0)
  {
    ClearParse();
    return;
  }

  SetKey (Other);

  m_PieceCount  = Other.Size();
  m_Evaluation  = Other.GetEvaluation();

  delete [] m_Pieces;
  delete [] m_PieceValues;

  SetLengthOfPieceVector( Other.GetLengthOfPieceVector() );
  m_Pieces        = new int [ m_LengthOfPieceVector ];
  m_PieceValues      = new double [ m_LengthOfPieceVector ];

  for (int i = 0; i <= m_PieceCount; i++ )
  {
    m_Pieces[i] = Other.m_Pieces[i];
    m_PieceValues[i] = Other.m_PieceValues[i];
  }
  m_SortStyle = Other.GetSortStyle();
}


// Verify there is room for appended pieces, makes
// room if necessary
//
// Parameters:
//		- NewSlots - the number of new piece slots
//		- LengthToAdd - the amount of new characters
//
/**
    Verifies that there is room in the structures for appending
    new pieces. Makes room if it is necessary. <kbd>NewSlots</kbd> is
    the number of new pieces. <kbd>LengthToAdd</kbd> is the number of
    new characters to be added to the key.
 */
void CParse::CheckSize( int NewSlots, int LengthToAdd)
{
  // Slots -------------------------------------------------------//

  if ( m_PieceCount + NewSlots >= m_LengthOfPieceVector )
  {
    AddSlots(NewSlots);
  }

  // m_Key ----------------------------------------------------------//

  int SpareRoom = m_AllocatedLength - m_KeyLength ; //just added that -1, 7-2001


  if ( SpareRoom < LengthToAdd )
  {
    if (LengthToAdd < m_KeyLength )
    {
      LengthToAdd = m_KeyLength + 1; //at least double the length; // just added that +1, 7-2001
    }

    AddKeyLength ( LengthToAdd );
  }

}


// Add slots for new pieces
//
// Parameters:
//		- N - number of new slots
//
/**
    Adds memory slots for new pieces. <kbd>N</kbd> is the number of
    slots to add.
 */
void CParse::AddSlots( int N )
{
  if ( N < m_LengthOfPieceVector)
  {
    N = m_LengthOfPieceVector;       // at least double the number of slots
  }
  if ( N < 4 ) { N = 4; }

  SetLengthOfPieceVector( m_LengthOfPieceVector + N + 1 );
  int* NewPieces      = new int [ m_LengthOfPieceVector ];
  NewPieces[0]      = 0;
  double* NewPieceValues  = new double [ m_LengthOfPieceVector ];
  NewPieceValues[0]    = m_DefaultPieceValue;

  if (m_Pieces)
  {
    for (int i = 1; i <= m_PieceCount; i++)
    {
      NewPieces [i] = m_Pieces[i];
      NewPieceValues[i] = m_PieceValues[i];
    }
  }

  delete [] m_Pieces;
  m_Pieces = NewPieces;
  delete [] m_PieceValues;      // WAS CRASHING HERE!  FIXED?  -cs- 01/19/04
  m_PieceValues = NewPieceValues;
}


// Add space to m_Key for new characters
//
// Parameters:
//		- Length - the number of new characters
//
/**
    Allocate space in the key array for new characters. <kbd>Length</kbd> the
    number of new characters.
 */
void CParse::AddKeyLength( int Length )
{
  if ( m_AllocatedLength > (int)Length)
  {
    Length = m_AllocatedLength; // the amount we wish to increase the m_Key Length: usually we'll double it
  }
  m_AllocatedLength  += Length;
  QChar*  NewKey    = new QChar [ m_AllocatedLength ];

  LxStrFll( m_Key, NewKey, m_KeyLength, m_AllocatedLength );

  delete [] m_Key;
  m_Key = NewKey;
}


// Append another CParse
//
// Parameters:
//		- Parse - the parse to be appended
//
/**
    Append another CParse. <kbd>Parse</kbd> is the CParse
    to be appended. If <kbd>ForceToEnd</kbd> is <i>TRUE</i>,
    the new parse will be appended to the end even when this
    parse is set to be in alphabetical order.
 */
void CParse::Append(const CParse& Parse)
{
    for (int i = 1; i <= Parse.Size(); ++i)
        Append(CStringSurrogate(Parse.GetPiece(i)));
}

// Append a string surrogate
//
// Parameters:
//		- SS - the surrogate to be appended
//
/**
    Append a string surrogate.
 */
void CParse::Append(const CStringSurrogate SS)
{
    if (SS.GetLength() == 0)
        return;

    CheckSize(1, SS.GetLength());

    LxStrCat(m_Key, &SS, m_KeyLength, SS.GetLength());
    m_KeyLength += SS.GetLength();
    ++m_PieceCount;

    m_Pieces[m_PieceCount] = m_KeyLength;
    m_PieceValues[m_PieceCount] = m_DefaultPieceValue;
}

// Append a QChar
//
// Parameters:
//		- ch - the character to be appended
//
/**
    Append a single QChar. <kbd>ch</kbd> is the QChar
    to be appended. If <kbd>ForceToEnd</kbd> is <i>TRUE</i>,
    the new parse will be appended to the end even when this
    parse is set to be in alphabetical order.
 */
void CParse::Append(const QChar& ch)
{
    CStringSurrogate ss(&ch, 0, 1);
    Append(ss);
    return;
}

// Append a double
//
// Parameters:
//		- d - the double to be appended
//
/**
    Append a double represented as a string.
 */
void CParse::Append(const double d)
{ Append(QString::number(d)); }

// Append an int
//
// Parameters:
//		- i - the int to be appended
//
/**
    Append an integer represented as a string with commas. <kbd>i</kbd> is the integer
    to be appended. If <kbd>ForceToEnd</kbd> is <i>TRUE</i>,
    the new parse will be appended to the end even when this
    parse is set to be in alphabetical order.
 */
void CParse::Append(const int i)
{
    const QString comma = ",";

    QString n = QString::number(i).trimmed();
    if (n.size() > 6) {
        n = n.left(n.size() - 6) + comma + n.right(6);
    }
    if (n.size() > 3) {
        n = n.left(n.size() - 3) + comma + n.right(3);
    }

    Append(n);
    Q_ASSERT(m_KeyLength < m_AllocatedLength);
}

// Append an int without commas
//
// Parameters:
//		- i - the int to be appended
//
/**
    Append an integer represented as a string without commas. <kbd>i</kbd> is the integer
    to be appended. If <kbd>ForceToEnd</kbd> is <i>TRUE</i>,
    the new parse will be appended to the end even when this
    parse is set to be in alphabetical order.
 */
void CParse::AppendNoCommas(const int i)
{ Append(QString::number(i)); }

// Append a hyphen
//
/**
    Append a hyphen. If <kbd>ForceToEnd</kbd> is <i>TRUE</i>,
    the new parse will be appended to the end even when this
    parse is set to be in alphabetical order.
 */
void CParse::AppendHyphen() { Append(QChar('-')); }

// Append a tab
//
/**
    Append a tab ("\t"). If <kbd>ForceToEnd</kbd> is <i>TRUE</i>,
    the new parse will be appended to the end even when this
    parse is set to be in alphabetical order.
 */
void CParse::AppendTab() { Append(QChar('\t')); }

// Append a carriage return
//
void CParse::AppendCR() { Append(QString("\r\n")); }

// Append a new line
//
void CParse::AppendNewLine() { Append(QChar('\n')); }

// Append a string on the next line
//
// Parameters:
//		- String - string to be appended
//
void CParse::AppendLine(const CStringSurrogate& SS)
{ AppendCR(); Append(SS); }

// Get the last piece
//
// Returns:
//		- CStringSurrogate - a surrogate of the last piece
//
CStringSurrogate CParse::GetLastPiece() const
{
  return GetPiece ( m_PieceCount );
}


// Check if the parse contains a string
//
// Parameters:
//		- SS - a surrogate of the string in question
//
// Returns:
//		- bool - true if there is a piece matching SS
//
bool CParse::Contains ( const CStringSurrogate& SS ) const
{
  if ( m_SortStyle == eNotSorted || m_SortStyle == eAlphabetizedFromEnd)
  {
    for (int i = 1; i <= m_PieceCount; i++)
    {

      if ( GetPiece(i) == SS ) { return true; }
    }
    return false;
  }
  else if (m_SortStyle == eAlphabetized)
  {
    for (int i = 1; i <= m_PieceCount && (GetPiece(i) < SS || GetPiece(i) == SS); i++)
    {

      if ( GetPiece(i) == SS ) { return true; }
    }
    return false;
  }
  else return false;
}

/*
// Check if the parse contains a string
//
// Parameters:
//		- String - a the string in question
//
// Returns:
//		- bool - true if there is a piece matching String
//
bool CParse::Contains ( const QString String ) const
{
  if ( m_bAlphabetical == false )
  {
    for (int i = 1; i <= m_PieceCount; i++)
    {
      if ( GetPiece(i) == String ) { return true; }
    }
    return false;
  }
  else  // this is alphabetical
  {
    for (int i = 1; i <= m_PieceCount && (GetPiece(i) < String || GetPiece(i) == String); i++)
    {
      if ( GetPiece(i) == String ) { return true; }
    }
    return false;
  }
}
*/


// Find the piece index of a substring
//
// Parameters:
//		- SS - a substring surrogate
//
// Returns:
//		- int - the piece index of the substring
//

int  CParse::Find ( const CStringSurrogate& SS ) const // ::ReturnIndex
{
  for (int i = 1; i <= m_PieceCount; i++)
  {
    if ( GetPiece(i) == SS )
    {
      return i;
    }
  }
  return 0;
}


// Clear the piece and key arrays
//

void CParse::ClearParse ()
{
  delete[] m_Pieces;
  delete[] m_PieceValues;
  SetLengthOfPieceVector(4);
  m_Pieces = new int[ m_LengthOfPieceVector ];
  m_Pieces[0] = 0;
  m_PieceValues = new double[ m_LengthOfPieceVector ];
  m_PieceValues[0] = m_DefaultPieceValue;
  m_PieceCount = 0;

  if (m_Key)
  {
    delete [] m_Key;
  }
  m_AllocatedLength  = 4;
  m_Key        = new QChar[ m_AllocatedLength ];
  m_KeyLength      = 0;
  m_Evaluation    = 0;
}


// Clear just the piece and piece value arrays
//
// This should be used only when we don't expect
// the Key to be linguistically real anymore --
// otherwise we probably should use
// SimplifyParseStructure.
//

void CParse::ClearParseStructure()
{
  m_Pieces[0] = 0;
  m_Pieces[1] = m_KeyLength;
  m_PieceValues[0] = m_DefaultPieceValue;
  m_PieceCount = 1;
}


// Simplify parse to one piece
//
void CParse::SimplifyParseStructure()
{
  if ( m_KeyLength == 0 )
  {
    m_PieceCount = 0;
    return;
  }

  m_PieceCount  = 1;
  m_Pieces[0]    = 0;
  m_Pieces[1]    = m_KeyLength;

  return;
}


// Set the piece count
//
// Parameters:
//		- Count - the new count, must be less than m_LengthOfPieceVector
//
void CParse::SetPieceCount( int Count )
{
    if( Count < m_LengthOfPieceVector )
    m_PieceCount = Count;
}


// Get a QString copy of the parse
// with the pieces delimited by some
// QChar
//
// Parameters:
//		- c - the piece delimiter
//
// Returns:
//		- QString - the delimited parse
//
QString CParse::Display( const QChar c, StringToString* filter ) const
{
  QString string;

  if( m_Key && m_PieceCount > 1 )
  {
    for (int i =  1; i < m_PieceCount; i++)
    {
      string += GetPiece(i).Display();
      if ( !c.isNull() )
      {
        string += c ;
      }
    }
    string += GetPiece(m_PieceCount).Display();
  }
  else if( m_Key ) string = GetKey().Display();

  if( filter )
  {
    string = Filter( filter, string );
  }

  return string;
}
QString CParse::Display(StringToString* filter ) const
{
  QString string;

  if( m_Key && m_PieceCount > 1 )
  {
    for (int i =  1; i < m_PieceCount; i++)
    {
      string += GetPiece(i).Display();
    }
    string += GetPiece(m_PieceCount).Display();
  }
  else if( m_Key ) string = GetKey().Display();

  if( filter )
  {
    string = Filter( filter, string );
  }

  return string;
}





// Move some characters from a piece into its
// preceding piece
//
// Parameters:
//		- PieceFrom - the piece that will be shortened
//		- LengthShortened - the number of characters to move
//
void CParse::MoveThisManyLettersLeftwardFromArg1
    (int PieceFrom, int LengthShortened)
{
  int PieceLength = ThisPieceLength(PieceFrom);
  if( PieceFrom > 1
    && PieceFrom <= m_PieceCount
    && LengthShortened > 0
    && LengthShortened <= PieceLength)
  {
    if( LengthShortened < PieceLength )
    {
      m_Pieces[ PieceFrom-1 ] += LengthShortened;
    }
    else if( LengthShortened == PieceLength )
    {
      MergePieces(PieceFrom-1);
    }
    if (ThisPieceLength(1) == GetKeyLength() )
    {
      ClearParseStructure();
    }
  }
}


// Move some characters from a piece into its
// following piece
//
// Parameters:
//		- PieceFrom - the piece that will be shortened
//		- LengthOfShift - the number of characters to move
//
void  CParse::MoveThisManyLettersRightwardFromArg1 (int  PieceFrom, int LengthOfShift)
{
  int PieceLength = ThisPieceLength(PieceFrom);
  if( PieceFrom > 0
    && PieceFrom < m_PieceCount
    && LengthOfShift > 0
    && LengthOfShift <= PieceLength)
  {
    if( LengthOfShift < PieceLength )
    {
      m_Pieces[ PieceFrom  ] -= LengthOfShift;
    }
    else if( LengthOfShift == PieceLength )
    {
      MergePieces(PieceFrom);
    }
    if (ThisPieceLength(1) == GetKeyLength() )
    {
      ClearParseStructure();
    }
  }
}


// Get the length of on piece of the parse
//
// Parameters:
//		- PieceNo - the index of the piece
//
// Returns:
//		- int - the length of m_Pieces[PieceNo]
//
int CParse::ThisPieceLength ( int PieceNo ) const
{
  if( m_Pieces && PieceNo > 0 && PieceNo <= m_PieceCount )
    return m_Pieces[PieceNo] - m_Pieces[PieceNo-1];
  else
    return -1;
}


// Get the left n characters of the parse
//
// Parameters:
//		- n - the number of desired characters
//
// Returns:
//		- CStringSurrogate - a surrogate of the left n characters
//
CStringSurrogate CParse::Left( int n ) const
{
  if( n > 0)
  {
    if( n > m_KeyLength ) n = m_KeyLength;
    return CStringSurrogate ( m_Key, 0, n );
  }
  else
    return CStringSurrogate ();
}


// Get the right n characters of the parse
//
// Parameters:
//		- n - the number of desired characters
//
// Returns:
//		- CStringSurrogate - a surrogate of the right n characters
//
CStringSurrogate CParse::Right( int n ) const
{
  if ( n >= 0 )
  {
    if ( n > m_KeyLength ) n = m_KeyLength;
    return CStringSurrogate ( m_Key, m_KeyLength - n, n );
  }
  else
  {
    return CStringSurrogate ();
  }
}


// Get a substring of all pieces preceding
// piece n
//
// Parameters:
//		- n - the index following the desired substring
//
// Returns:
//		- CStringSurrogate - all pieces up to but not including piece n
//
CStringSurrogate CParse::WhatPrecedesPiece(int n) const
{
  if( n > 0 )
  {
    if( n > Size()) n = Size();
    return CStringSurrogate ( m_Key, 0, m_Pieces[n-1] );
  }
  else
    return CStringSurrogate();
}


// Get a substring that includes piece n
// and everything after n
//
// Parameters:
//		- n - the index of the desired piece
//
// Returns:
//		- CStringSurrogate - the substring from m_Pieces[n] to m_Pieces[Size()]
//
CStringSurrogate CParse::ThisPieceAndWhatFollows( int n ) const
{
  if( n <= Size() )
  {
    if ( n < 1 ) n = 1;
    return CStringSurrogate ( m_Key, m_Pieces[n-1], m_KeyLength-m_Pieces[n-1] );
  }
  else
    return CStringSurrogate();
}


// Not a member function, checks if any two parses
// overlap
//
// Parameters:
//		- Parse1 - the first CParse
//		- Parse2 - the second CParse
//
// Returns:
//		- bool - true if any piece of Parse1 matches any piece of Parse2
//
bool AnyOverlap (const CParse& Parse1, const CParse& Parse2)
{
  Q_ASSERT ( Parse1.GetSortStyle()==eAlphabetized && Parse2.GetSortStyle() == eAlphabetized );

  for (int i = 1; i <= Parse1.Size(); i++)
  {
    for (int j = 1; j <= Parse2.Size(); j++ )
    {
      if ( Parse1[i] > Parse2[j] )
      {
        continue;
      }
      if ( Parse1[i] == Parse2[j] )
      {
        return true;
      }
      if ( Parse1[i] < Parse2[j] )
      {
        break;
      }
    }
  }
  return false;
}


// Find out if this contains all pieces
// of another parse
//
// Parameters:
//		- Parse - the pieces we want to look for
//
// Returns:
//		- bool - true if every piece in Parse is contained in this
//
bool CParse::Contains (const CParse& Parse) const
{
  bool    Success = false;

  for (int i = 1; i<= Parse.Size(); i++)
  {
    for (int j = 1; j <= m_PieceCount; j++)
    {
      if ( GetPiece(j) == Parse.GetPiece(i) )
      {
        Success = true;
        break;
      }
    }
    if (Success == false)
    {
      return false;
    }
    Success = false;
  }
  return true;
}


// Find out if any piece of this equals
// any piece of another parse
//
// Parameters:
//		- Parse - the other parse
//
// Returns:
//		- bool - true if any piece of this equals any piece of Parse
//
bool CParse::Overlap (const CParse& Parse) const
{
   if (m_SortStyle == eAlphabetized && Parse.GetSortStyle() == eAlphabetized)
   {
      for (int i=1, j=1; i <= m_PieceCount && j <= Parse.Size();)
      {
        CStringSurrogate ours = GetPiece(i);
        CStringSurrogate theirs = Parse.GetPiece(j);
        if ( ours == theirs )
        {
          return 1;
        }
        else if( ours < theirs )
        {
          i++;
          continue;
        }
        else // ours > theirs
        {
          j++;
          continue;
        }
      }
      return 0;
   }
   return 0; // double check this.
   // @@@@ put it another part, in case either this or Parse are not sorted JG Oct 2008

}


// Merge the pieces of another parse
// and this into a new parse
//
// Parameters:
//		- Other - the other parse
//		- Output - the two parses merged into one
//
/*
CParse CParse::MergeParse (const CParse& Other, CParse& Output) const
{
//  Alphabetize();
//  Other.Alphabetize();

  int   i = 1,
      j=  1,
      Winner;
  QString ThisPiece,
      ThatPiece;
  CStringSurrogate    ssThis, ssThat;

  Output.ClearParse();
  Output.Alphabetize();
  while ( i <= (int) m_PieceCount || j<= Other.Size() )
  {
    if ( j > Other.Size() )
    {
      Winner = 1;
      //ThisPiece = GetAt(i);
      ssThis = GetPiece(i);

    } else
    {
      if ( i > Size() )
      {
        Winner = 2;
        //ThatPiece = Other.GetAt(j);
        ssThat = Other.GetPiece(j);

      } else
      {
        //ThisPiece = GetAt(i);
        //ThatPiece = Other.GetAt(j);

        ssThis = GetPiece(i);
        ssThat = Other.GetPiece(j);

        //if (ThisPiece < ThatPiece)
        if (ssThis < ssThat)
        {
          Winner = 1;
        } else
        {
          //if ( ThisPiece > ThatPiece )
          if ( ssThis > ssThat )
          {
            Winner = 2;
          } else
          {
            Winner =  3;
          }
        }
      }
    }

    switch (Winner) {
      case 3:{    //the same piece

        Output.Append(ssThis);
        i++;
        j++;
        break;
      }
      case 1: {

        Output.Append(ssThis);
        i++;
        break;
      }
      case 2:  {

        Output.Append(ssThat);
        j++;
        break;
      }
    }
  }
  Q_ASSERT (m_PieceCount < 2 || m_Pieces[1] > 0 );
  return Output;
}
*/

// Merge the pieces of another parse
// into this parse
//
// Parameters:
//		- Other - the other parse
//

void CParse::MergeAndAlphabetizeParse ( const CParse& Other )
{
  CParse  Merged;
  if( m_SortStyle != eAlphabetized ) Alphabetize();
//  MergeParse (Other, Merged);
//  this->ClearParse();
//  *this = Merged;

    Append (Other);
    Alphabetize();


  return;
}


// Make a new cut in the length - n position
//
// Parameters:
//		- n - the position from the right in which the cut will be made
//
void CParse::CutNFromTheRight (int n)
{
  if( n > 0 && n < GetKeyLength() )
    CutRightBeforeHere ( GetKeyLength() - n );
}




// Make a new cut in the n position
//
// Parameters:
//		- n - the position from the left in which the cut will be made
//
void CParse::CutNFromTheLeft (int n)
{

//for( int d = 0; d <= Size(); d++ )
//{
//	qDebug( QString( "%1" ).arg( m_Pieces[d] ) );
//}

  if( n > 0 && n < GetKeyLength() )
    CutRightBeforeHere ( n );


//for( int d = 0; d <= Size(); d++ )
//{
//	qDebug( QString( "%1" ).arg( m_Pieces[d] ) );
//}

}


// Make a new cut
//
// Parameters:
//		- loc - the index following the cut location
//
void CParse::CutRightBeforeHere (int loc) //loc is zero-based, of course.
{
  if( loc >= 0 && loc <= GetKeyLength() )
  {
    int i = 0;
    int RightSpot;

    // Adding one slot, nothing to the key
    CheckSize(1,0);

    for ( i = 0; m_Pieces[i] < loc && i <= m_PieceCount; i++ )
    {
    // blank on purpose
    }

    if (m_Pieces[i] == loc )
    {
      // there was already a cut there
      Q_ASSERT (m_PieceCount < 2 || m_Pieces[1] > 0 );
      return;
    }
    if (m_PieceCount == 0)
    {
      m_PieceCount = 1;
      m_Pieces[1] = GetKeyLength();
      m_PieceValues[1] = m_DefaultPieceValue;
    }

    m_PieceCount++;
    RightSpot = i;

    for ( i=m_PieceCount; i > RightSpot   ; i--)
    {
      m_Pieces[i] = m_Pieces[i-1];
      m_PieceValues[i] = m_PieceValues[i-1];
    }

    m_Pieces[RightSpot] = loc;
    m_PieceValues[RightSpot] = m_DefaultPieceValue;
    Q_ASSERT (m_PieceCount < 2 || m_Pieces[1] > 0 );
  }
}


// Merge two adjacent pieces of this
// into one piece
//
// Parameters:
//		- i - the index of the first piece
//
void CParse::MergePieces (int i)
{
// merge piece #i and piece# i+1;
  if ( i >= m_PieceCount || i < 1 ) {return;}

  for (int n=i ; n < m_PieceCount; n++)
  {
    m_Pieces[n] = m_Pieces[n+1];
    m_PieceValues[n] = m_PieceValues[n+1];
  }
  m_PieceCount--;
   Q_ASSERT (m_PieceCount < 2 || m_Pieces[1] > 0 );
}

/*
// Separate a delimited string into pieces
// and write the pieces over this parse
//
// Parameters:
//		- String - the delimited string
//		- Divider - the QChar that delimits the pieces
//
void CParse::Collapse( const QString String, const QChar& Divider )
{
  Collapse( String.unicode(), String.length(), Divider );
}
*/


// Separate a delimited string into pieces
// and write the pieces over this parse
//
// Parameters:
//		- String - the delimited string
//		- Length - the length of String
//		- Divider - the QChar that delimits the pieces
//
void CParse::Collapse( const CStringSurrogate& SS, const QChar& Divider)
{
  int Length = SS.GetLength(),
    i;
  ClearParse();

  int DividerCount = 0;
  for ( i = 0; i < Length ; i++)
  {
    if (SS[i] == Divider) DividerCount++;
  }

  SetLengthOfPieceVector( DividerCount + 2 );
  delete[] m_Pieces;
  delete[] m_PieceValues;
  m_Pieces = new int[ m_LengthOfPieceVector ];
  m_PieceValues = new double[m_LengthOfPieceVector];
  m_AllocatedLength = Length+1;
  m_KeyLength = Length - DividerCount;
  m_Key = new QChar [ m_AllocatedLength ];
  m_PieceCount = 0; // change March 2003
  m_Pieces[0] = 0;
  m_PieceValues[0] = m_DefaultPieceValue;
  int j = 0;
  int n = 1;
  for ( i = 0; i < Length ; i++)
  {
    if ( SS[i] == Divider )
    {
      if ( m_Pieces[n-1] < j )  // i.e., not two dividers in a row ? -1 added March 2003
      {
        m_Pieces[n++] = j;
        m_PieceCount++;
      }
    }
    else
    {
      *(m_Key + j++) = SS[i];
    }
  }
  if ( m_Pieces[n-1] < j )  // i.e., not two dividers in a row // ?? maybe change to be like the following function...
  {
    m_Pieces[n++] = j;
    m_PieceCount++;
  } // march 2003
  for( i=0;i<m_PieceCount;i++) m_PieceValues[i] = m_DefaultPieceValue;
  m_SortStyle = eNotSorted; // It is no longer guaranteed to be in alphabetical order. -cs- 01/20/2004
}


/*
// Absorb a string delineated into pieces
// by white space
//
// Parameters:
//		- String - the string to be absorbed
//
void CParse::CollapseSpaceTab( const QString String )
{
  QStringList List;

  QRegExp     regexp ("[ \t]");
  List = QStringList::split ( regexp, String );

  m_PieceCount = 0;

  AbsorbQStringList (List);
}
*/


/*
// Absorb a string and make each line
// a different piece
//
// Parameters:
//		- String - the string to be absorbed
//
void CParse::CollapseLine( QString String ) // divider is \r\n
// similar to Collapse, but uses \r\n as the divider (which is Windows' way of separating lines)
{

  QStringList List;
  QRegExp     regexp ("\n\r");
  List = QStringList::split ( regexp , String  );

  m_PieceCount = 0;

  AbsorbQStringList (List);
}
*/


// If the last character of every piece is
// the same, get that character
//
// Returns:
//		- QChar - the common last letter from all pieces if there is one, NULL otherwise.
//
QChar  CParse::FindCommonLastLetterFromAll() const
{
  QChar Letter;

  if ( GetPiece(1).IsNULL() )
  {
    return QChar(0);
  }
  if (m_PieceCount > 0)
  {
    Letter = GetPiece(1).LastChar();
  }
  for (int i = 2; i <= m_PieceCount; i++)
  {
    if (GetPiece(i).IsNULL() ) return QChar(0);

    if ( Letter != GetPiece(i).LastChar() )
    {
      return QChar(0);
    }
  }
  return Letter;
}


// If the first character of every piece is
// the same, get that character
//
// Returns:
//		- QChar - the common first letter from all pieces if there is one, NULL otherwise.
//
QChar  CParse::FindCommonFirstLetterFromAll() const
{
  QChar Letter;

  if ( GetPiece(1).IsNULL() )
  {
    return QChar(0);
  }
  if (m_PieceCount > 0)
  {
    Letter = GetPiece(1)[0];
  }
  for (int i = 2; i <= m_PieceCount; i++)
  {

    if (GetPiece(i).IsNULL() ) return QChar(0);

    if ( Letter != GetPiece(i)[0] )
    {
      return QChar(0);
    }
  }

  return Letter;
}

void CParse::RemovePiece(int n)
{
    using std::copy;
    using std::transform;
    using std::minus;
    using std::bind2nd;

    Q_ASSERT(n >= 1 && n <= m_PieceCount);

    const int length = ThisPieceLength(n);
    Q_ASSERT(m_Pieces[n-1] + length == m_Pieces[n]);
    Q_ASSERT(m_Pieces[n] <= GetKeyLength());

    // move pieces left
    copy(&m_Key[m_Pieces[n]], &m_Key[GetKeyLength()],
        &m_Key[m_Pieces[n-1]]);

    // move piece offsets left
    transform(&m_Pieces[n], &m_Pieces[m_PieceCount + 1],
        &m_Pieces[n - 1], bind2nd(minus<int>(), length));

    // move piece values left
    copy(&m_PieceValues[n], &m_PieceValues[m_PieceCount + 1],
        &m_PieceValues[n - 1]);

    --m_PieceCount;

    // Make sure leftmost piece is nonempty
    // (how could it not be?).
    Q_ASSERT(m_PieceCount < 2 || m_Pieces[1] > 0 );
}

void CParse::Remove ( const CStringSurrogate& ssPiece )
{
    const int i = Find(ssPiece);

    if (i != 0) {
        // found!
        RemovePiece(i);
    } else {
        // not found: nothing to remove.
    }
}

/*
// Append a new piece in alphabetical
// order in relation to the other pieces
//
// Parameters:
//		- String - the string to be appended
//

  void CParse::AppendInAlphabeticalOrder ( const QString String )
{
  Q_ASSERT ( Alphabetical() && IsAlphabetical() );
  AppendInAlphabeticalOrder ( CStringSurrogate( String ) );
}
*/



// Append a the pieces of another parse
// in alphabetical order in relation to
// the other pieces
//
// Parameters:
//		- pParse - the parse (pieces) to be appended
//
/*
void CParse::AppendInAlphabeticalOrder ( const CParse* pParse )
{
  for (int i = 1; i <= (int) pParse->Size(); i++)
  {
    AppendInAlphabeticalOrder( pParse->GetPiece(i) );
  }
}
*/

// Append a new piece in alphabetical
// order in relation to the other pieces
//
// Parameters:
//		- SS - the surrogate to be appended
//
/*
void CParse::AppendInAlphabeticalOrder ( const CStringSurrogate& SS, bool IsTemplateOperator )
{
  CStringSurrogate  tempSS;
  int*        Sizes;
  int          pos=0, j;

  Q_ASSERT ( Alphabetical() );

  CheckSize( 1, SS.GetLength() ); // increases number of slots if necessary.

  //  ----------------------------------------------
  //  1. Easiest case, if SS just goes on the end:
  //  ----------------------------------------------

  if ( m_PieceCount == 0 || SS > GetPiece(m_PieceCount) )
  {
    Append ( SS, 1 ); // Force append to end of parse
    return;
  }
  //  ----------------------------------------------


  Sizes = new int[ m_PieceCount+1 ];
  Sizes[0] = 0;

  for (int n = 1; n <= m_PieceCount; n++)
  {
    Sizes[n] = ThisPieceLength(n);
  }

  for ( pos = 1; pos<= m_PieceCount; pos++)
  {
    // Removed to allow duplicates

      if ( IsTemplateOperator )
      {
        if ( GetPiece (pos) == SS )
        {
          return;
        }
      }

//    if ( GetPiece (i) == SS )
//    {
//      return;          // this is new -- I hope it's OK.
//    }

    if ( GetPiece(pos) > SS )
    {
      break;
    }
  }

    // ab cde wxyz  +  mno
    // 0  2   5   9
    // i is now 3;
    // it should become:
    // ab cde mno wxyz
    // 0  2   5   8    12

  //  ----------------------------------------------
  //  Make the new m_Key
  //  ----------------------------------------------

//  Not necessary with CheckSize() called earlier
//  m_AllocatedLength  += SS.GetLength();

  QChar* NewKey    = new QChar [ m_AllocatedLength ];
  tempSS        = UpToAndIncludingThisPiece(pos-1);

  for( j=0; j<(int)tempSS.GetLength(); j++ )
    NewKey[j] = tempSS[j];
  int CurrentLength  = tempSS.GetLength();

  for( j=0; j<(int)SS.GetLength(); j++ )
    NewKey[j+CurrentLength] = SS[j];
  CurrentLength += SS.GetLength();

  tempSS        = ThisPieceAndWhatFollows(pos);
  for ( j=0; j<(int)tempSS.GetLength(); j++ )
    NewKey[j+CurrentLength] = tempSS[j];
  m_KeyLength    += SS.GetLength();

   if (m_Key)
  {
    delete [] m_Key;
  }
  m_Key = NewKey;

  //  ----------------------------------------------
  //  ----------------------------------------------


  m_PieceCount++;
  Q_ASSERT (pos > 0);

  m_Pieces[pos] = m_Pieces[pos-1] + SS.GetLength();

  for( int i=m_PieceCount; i>pos; i-- )
  {
    m_PieceValues[i] = m_PieceValues[i-1];
  }
  m_PieceValues[pos] = m_DefaultPieceValue;

  for (pos++ ; pos<=m_PieceCount; pos++)
  {
    m_Pieces[pos] = m_Pieces[pos-1] + Sizes[pos-1];
  }

  delete [] Sizes;
}
*/

// Check if this is alphabetical, does not
// appeal to the member m_bAlphabetical
//
// See Alphabetical().
//
// Returns:
//		- bool - true if the pieces of this are in alphabetical order
//
bool CParse::IsAlphabetical() const
{
  CStringSurrogate former, latter;

  for (int i = 1; i < m_PieceCount; i++)
  {
    former = GetPiece(i);
    latter = GetPiece(i+1);
    if ( former > latter )
    {
      return false;
    }
  }
  return true;
}

//----------------------------------------------//
// Alphabetize the pieces of the parse
//----------------------------------------------//
void CParse::Alphabetize()
{
  if (  m_SortStyle != eAlphabetized && m_PieceCount > 1 )
  {
    QStringList List;
    CreateQStringList (List);
    List.sort();
    AbsorbQStringList (List);
    m_SortStyle = eAlphabetized;
  }
  return;
}



//------------------------------------------------------------------//
// Reverse Alphabetize the pieces of the parse (from the right side)
//------------------------------------------------------------------//
void CParse::ReverseAlphabetize ()
{

    QStringList         List;
    CreateQStringList   (List);
    SortQStringListFromRight(List);
    AbsorbQStringList   (List);

}





//------------------------------------------------------------------//

// Replace the parse structure with
// carriage returns between the pieces
//
void CParse::InsertCRs()
{
  CParse Temp;

  for (int i = 1; i <= m_PieceCount; i++)
  {
    Temp.AppendCR();
    Temp.Append( GetPiece(i) );
  }
  Temp.AppendCR();
  ClearParse();
  CopyParse (Temp);
}



//////////////////////////////////////////////
//        October 3 1999
//////////////////////////////////////////////

// Absorb the leftmost n characters from
// another parse as key
//
// Parameters:
//		- Parse - the source parse
//		- n - the number of characters
//
void CParse::TakeLeft (const CParse& Parse, int n)
{
  CStringSurrogate buffer = Parse.GetKey();
  SetKey( buffer.Left(n) );
}


// Absorb the rightmost n characters from
// another parse as key
//
// Parameters:
//		- Parse - the source parse
//		- n - the number of characters
//
void CParse::TakeRight (const CParse& Parse, int n)
{
  CStringSurrogate buffer = Parse.GetKey();
  ClearParse();
  Append ( buffer.Right(n) );
}


// Get a CParse of each piece that begins with a specific
// prefix, store any pieces that don't start with that
// prefix to pRemains
//
// Parameters:
//		- Prefix - the prefix in question
//		- pRemains - all pieces that don't contain Prefix
//
// Returns:
//		- CParse - all pieces that do contain Prefix
//
CParse CParse::RipOut( const QChar Prefix, CParse* pRemains ) const
{
  CParse  Output;
  QString strNull = "NULL";
  CStringSurrogate Suffix,
      Null( strNull.unicode(), 0, strNull.length() );
  if (pRemains) {
    pRemains -> ClearParse();
  }
        // feb 4 2000 changed subscripts to zero based //????
  for (int i = 1; i <= Size(); i++) {
    Suffix = GetPiece(i);
    if ( Suffix.GetLength() == 1 && Suffix[0] == Prefix ) {
//      Output.AppendInAlphabeticalOrder ( Null );
        Output.Append(Null);
        continue;
    }
    if ( Suffix[0] == Prefix ) {
//      Output.AppendInAlphabeticalOrder ( Suffix.Mid(1) );
//      Output.Append ( Suffix.Mid(1) );
        continue;
    }
    if (pRemains)
    {
      pRemains->Append(Suffix);
    }
  }
Q_ASSERT (m_PieceCount < 2 || m_Pieces[1] > 0 );
  return Output;
}


// Create a different parse with each piece shortened
// by the same amount
//
// Parameters:
//		- NumLetters - the number of letters to shorten each suffix by
//		- ShortenedAffixParse - the CParse to store the shortened suffixes in.
//
int CParse::DeleteLastCharsOfAllPieces (
        int        NumLetters,
        CParse&      ShortenedAffixParse ) const
{
  QString    strTailingString,
        strAffix,
        strNull = "NULL",
        strPreSuffix;
  CStringSurrogate Null( strNull.unicode(), 0, strNull.length() );

  if ( ContainsNULL() ) return 0;

  ShortenedAffixParse.ClearParse();

  QStringList  List;
  CreateQStringList (List);

  strTailingString = List.first().right(NumLetters);

  QStringList::Iterator it;
  for ( it = List.begin(); it != List.end(); ++it)
  {
    if ( (*it).right(NumLetters) != strTailingString )
    {
      return 0;
    }

    if ( static_cast <int> ( (*it).length() ) == (int) NumLetters )
    {
//    ShortenedAffixParse.AppendInAlphabeticalOrder( Null );
      ShortenedAffixParse.Append( Null );
    }
    else
    {
      strPreSuffix = (*it).left ( (*it).length() - NumLetters );
//      ShortenedAffixParse.AppendInAlphabeticalOrder( CStringSurrogate( strPreSuffix.unicode(),
//                                       0,
//                                       strPreSuffix.length() ) );
       ShortenedAffixParse.Append ( CStringSurrogate( strPreSuffix.unicode(),
                                       0,
                                       strPreSuffix.length() ) );

    }
  }
  return 1;
}


// Create a different parse with each affix shortened
// by the same amount
//
// Parameters:
//		- NumLetters - the number of letters to shorten each suffix by
//		- ShortenedAffixParse - the CParse to store the shortened suffixes in.
//
int CParse:: DeleteLeadingCharsOfAllPieces (
        int        NumLetters,
        CParse&      ShortenedAffixParse ) const
{
  QString    strLeadingString,
        strSuffix,
        strNull = "NULL",
        strPostPrefix;
  CStringSurrogate Null( strNull.unicode(), 0, strNull.length() );

  if ( ContainsNULL() ) return 0;

  ShortenedAffixParse.ClearParse();

  QStringList  List;
  CreateQStringList (List);

  strLeadingString = List.first().left(NumLetters);

  for ( QStringList::Iterator it = List.begin(); it != List.end(); it++)
  {
    if ( (*it).left(NumLetters) != strLeadingString )
    {
      return 0;
    }

    if ( static_cast <int> ( (*it).length() ) == (int) NumLetters )
    {
//      ShortenedAffixParse.AppendInAlphabeticalOrder( Null );
      ShortenedAffixParse.Append ( Null );
    }
    else
    {
      strPostPrefix = (*it).left ( (*it).length() - NumLetters );
//      ShortenedAffixParse.AppendInAlphabeticalOrder( CStringSurrogate( strPostPrefix.unicode(),
//                                       0,
//                                       strPostPrefix.length() ) );
      ShortenedAffixParse.Append ( CStringSurrogate( strPostPrefix.unicode(),
                                       0,
                                       strPostPrefix.length() ) );
    }
  }
  return 1;
}


// Find out if all the piece lengths are 1
//
// Returns:
//		- bool - true if all pieces are of length 1
//
bool CParse::AreAllPiecesSingletons () const
{
  for (int i = 1; i <= m_PieceCount; i++) {
    if ( ThisPieceLength(i) != 1 )
    {
      return false;
    }
  }
  return true;
}


// Add a prefix to every piece in this parse
//
// Parameters:
//		- SS - the prefix to add
//
void CParse::PrefixToAllPieces2 ( const CStringSurrogate& SS )
{
    CParse						TempParse;
    CStringSurrogate			OnePiece, NewPiece;
    QString						TempString;


    for ( int i=1; i<= Size(); i++)
    {
        OnePiece = GetPiece(i);

        if ( OnePiece == CStringSurrogate(QString("NULL").unicode(), 0, 4))
        {
            NewPiece = SS;
        }
        else
        {
            TempString = SS.Display() + OnePiece.Display();
            NewPiece =  CStringSurrogate(TempString);
        }

        TempParse.Append(NewPiece);

    }

    this ->ClearParse();

    (*this) = TempParse;

    return;

}


// Add a prefix to every piece in this parse
//
// Parameters:
//		- SS - the prefix to add
//
// Note: this does not work correctly when one of the pieces is NULL;
//
void CParse::PrefixToAllPieces ( const CStringSurrogate& SS )
{
  int piece_end, piece_start,
    size_to_add = SS.GetLength()*Size(),
    pos, i, j;

  CheckSize( 0, size_to_add );

  m_KeyLength += size_to_add;
  pos = m_KeyLength - 1;

  for( i=Size(); i>=1; i-- )
  {
    piece_end = m_Pieces[i] - 1;
    piece_start = m_Pieces[i-1];

    Q_ASSERT(pos >= 0);

    for( j=piece_end; j >= piece_start; j-- )
    {
      m_Key[pos--] = m_Key[j];
    }
    for( j=SS.GetLength()+SS.GetStart()-1; j>=(int)SS.GetStart(); j-- )
    {
      m_Key[pos--] = SS[j];
    }
  }
  for( i=1; i<=Size(); i++ )
  {
    m_Pieces[i] += i*SS.GetLength();
  }
}


// Add a prefix to every piece in this parse
//
// Parameters:
//		- ch - the prefix to add
//
void CParse::PrefixToAllPieces (const QChar ch)
{
  PrefixToAllPieces( CStringSurrogate(&ch,0,1) );
}


// Add a suffix to every piece in this parse
//
// Parameters:
//		- SS - the suffix to add
//
void CParse::SuffixToAllPieces2 ( const CStringSurrogate& SS )
{
    CParse						TempParse;
    CStringSurrogate			OnePiece, NewPiece;
    QString						TempString;


    for ( int i=1; i<= Size(); i++)
    {
        OnePiece = GetPiece(i);

        if ( OnePiece == CStringSurrogate(QString("NULL").unicode(), 0, 4))
        {
            NewPiece = SS;
        }
        else
        {
            TempString =OnePiece.Display() +  SS.Display() ;
            NewPiece =  CStringSurrogate(TempString);
        }

        TempParse.Append(NewPiece);

    }

    this ->ClearParse();

    (*this) = TempParse;

    return;

}


// Add a suffix to every piece in this parse
//
// Parameters:
//		- SS - the suffix to add
//
// Note: This does not work correctly when one of the pieces is NULL.
//
void CParse::SuffixToAllPieces ( const CStringSurrogate& SS )
{
  int piece_end, piece_start,
    size_to_add = SS.GetLength()*Size(),
    pos, i, j;

  CheckSize( 0, size_to_add );

  m_KeyLength += size_to_add;
  pos = m_KeyLength - 1;

  for( i=Size(); i>=1; i-- )
  {
    piece_end = m_Pieces[i] - 1;
    piece_start = m_Pieces[i-1];

    Q_ASSERT(pos >= 0);

    for( j=SS.GetLength()+SS.GetStart()-1; j>=(int)SS.GetStart(); j-- )
    {
      m_Key[pos--] = SS[j];
    }
    for( j=piece_end; j >= piece_start; j-- )
    {
      m_Key[pos--] = m_Key[j];
    }
  }
  for( i=1; i<=Size(); i++ )
  {
    m_Pieces[i] += i*SS.GetLength();
  }
  m_SortStyle = eNotSorted; // It is no longer guaranteed to be in alphabetical order. -cs- 01/20/2004
}


// Add a suffix to every piece in this parse
//
// Parameters:
//		- SS - the suffix to add
//
void CParse::SuffixToAllPieces (const QChar ch)
{
  SuffixToAllPieces( CStringSurrogate(&ch,0,1) );
}


// Remove a common prefix from all pieces
// assuming that all pieces have the prefix
//
// Parameters:
//		- SS - the prefix to be removed
//
void CParse::RemovePrefixFromAllPieces( const CStringSurrogate& SS )
{
  CParse        Temp;
  CStringSurrogate  ssPiece, ssPiece2;

  for (int i = 1; i <= m_PieceCount; i++)
  {
    ssPiece = GetPiece(i);
    Q_ASSERT ( ssPiece.Left(SS.GetLength()) == SS );
    ssPiece2 = ssPiece.Mid(
            SS.GetLength(),
            ssPiece.GetLength() - SS.GetLength()
                );

//    Temp.AppendInAlphabeticalOrder( ssPiece2 );
      Temp.Append( ssPiece2 );
  }
  Temp.Alphabetize();
  *this = Temp;
}


// Remove a common suffix from all pieces
// assuming that all pieces have the suffix
//
// Parameters:
//		- SS - the suffix to be removed
//
void CParse::RemoveSuffixFromAllPieces( const CStringSurrogate& SS )
{
  CParse        Temp;
  CStringSurrogate  ssPiece, ssPiece2;
  if(m_SortStyle == eAlphabetized ) Temp.Alphabetize();

  for (int i = 1; i <= m_PieceCount; i++)
  {
    ssPiece = GetPiece(i);
    Q_ASSERT ( ssPiece.Right(SS.GetLength()) == SS );
    ssPiece2 = ssPiece.Left(
            ssPiece.GetLength() - SS.GetLength()
                );

    Temp.Append( ssPiece2 );
  }
  *this = Temp;
}


// Find out if this parse is equal
// to a char plus another signature
//
// Parameters:
//		- Parse - the signature
//		- c - the character
//
// Returns:
//		- bool - true if this is equal to c + Parse
//
bool CParse::TestIfCharPlusSigEqualsThis( const CParse& Parse, const QChar c) const
{

  CParse Other = Parse;
  Other.Append(c);
  if( this->Contains(Other) && Other.Contains(this) ) return true;
  else return false;
/*
  CParse  PNewPiece;
  CStringSurrogate    ssPiece, ssNewPiece;

  PNewPiece.Append(c);
  for (int i = 1; i <= Parse.Size(); i++)
  {
    ssPiece = Parse.GetPiece(i);
    if (ssPiece.IsNULL() ) {ssPiece.MakeNull() ;}
    PNewPiece <<  ssPiece;
    if (Contains (PNewPiece) == 0 ) { return false;}
  }
  return true;
*/
}

/*
void CParse::SignatureDifference(CParse* Other, CParse& Results)
{
  QString      Suffix;
  CParse      MyCopy,
          ThatCopy = *Other;
  int        Index;
  char      cCommonLetter;
  char      MyExtraChar, ThatExtraChar, ExtraChar;
  CParse*      pSuffixSig,
    *      pNULLContainingSig;


  if ( Size() != Other->Size() ) { return; }

  Results.ClearParse();


  // MyCopy will contain only the suffixes in This which are
  // NOT part of Other. Suffixes that are in both This and
  // Other will be removed from ThatCopy.
  for (int i = 1; i <= m_PieceCount; i++)
  {
    Suffix = GetAt(i);
    Index = ThatCopy.Find(Suffix);
    if (Index)
    {
      ThatCopy.RemovePiece(Index);
    }
    else
    {
//      MyCopy.AppendInAlphabeticalOrder(Suffix);
      MyCopy.Append(Suffix);

    }


  }
  //We now have two parses with nothing in common: MyCopy and ThatCopy.
  MyExtraChar   =   MyCopy.FindCommonFirstLetterFromAll();
  ThatExtraChar = ThatCopy.FindCommonFirstLetterFromAll();
  if (MyExtraChar && ThatExtraChar ) {return; }
  if (MyExtraChar && !ThatExtraChar)
  { MyIsLonger = true;  ThatIsLonger = false; ExtraChar = MyExtraChar; } else
  { MyIsLonger = false; ThatIsLonger = true;  ExtraChar = ThatExtraChar; }

  // 1 possibility: we augment the small suffixes to "bulk" them up, then
  // delete stem final copies (like in the case of English "ed"
  // 2nd possibility: we delete a suffix vowel in certain cases













  //some old but useful code to recreate:
    We now have two parses with nothing in common.
    We want to see if one has a NULL and the other
    has a single letter that all of its sisters start
    with. If so, check if all the affixes differ by
    just that letter. If so, return that letter.


    if ( MyCopy.ContainsNULL() )
    {
      pSuffixSig = &ThatCopy;
      pNULLContainingSig = &MyCopy;
    }
    else {
      if (ThatCopy.ContainsNULL() )
      {
        pSuffixSig = &MyCopy;
        pNULLContainingSig = &ThatCopy;
      }
      else
      {
        return;
      }
    }

    if (pSuffixSig->ContainsNULL() ) {
      return;
    }
    cCommonLetter = pSuffixSig->FindCommonFirstLetterFromAll();
    if (! cCommonLetter) {
      return;
    }
    *pSuffixSig = pSuffixSig->RipOut(cCommonLetter);

    pSuffixSig->Alphabetize();
    if ( * pSuffixSig == * pNULLContainingSig )
    {
      Results << (char) cCommonLetter;
    }

    return;

}
*/


// Find out if a piece of the parse
// equals "NULL"
//
// Returns:
//		- bool - true if any piece of this equals "NULL"
//
bool CParse::ContainsNULL() const
{
//  QString Null = "NULL";
  if ( Find ( CStringSurrogate( TheStringNULL.unicode(), 0, TheStringNULL.length() ) ) > 0 )
  {
    return true;
  } else{
    return false;
  }
}


/*
// Find the piece index of a substring
//
// Parameters:
//		- String - a substring
//
// Returns:
//		- int - the piece index of the substring
//
int CParse::Find( const QString String ) const
{
  return Find ( CStringSurrogate( String ) );
}
*/


// Get the intersection of this parse
// with another
//
// Parameters:
//		- Parse - the other parse to intersect
//
// Returns:
//		- CParse - a parse intersection of the pieces of this and parse
//
CParse CParse::Intersection (const CParse& Parse) const
{
  Q_ASSERT( GetSortStyle() == eAlphabetized && Parse.GetSortStyle() == eAlphabetized );

  int i = 1, j = 1;
  CParse Intersection;
  CStringSurrogate Piece;
  CStringSurrogate Other;

  for (i = 1; i <= Size(); i++)
  {
    if ( j > Parse.Size()  ) { return Intersection; }
    Piece = GetPiece(i);

    while ( j <= Parse.Size()   )
    {
      Other = Parse.GetPiece(j);
      if ( Other > Piece )
      {
        break;
      }
      if ( Other == Piece )
      {
        Intersection.Append (Piece);
        j++;
        break;
      }
      if ( Other < Piece )
      {
        j++;
        continue;
      }
    }
  }
  return Intersection;
}


//  TODO: Ask John.  Same as CParse::Find?  e.g. CParse::Find("c")
//
//char* CParse::FindLeftmostOccurrence(char c, int* i) const
//{
//  return strchr (m_Key + *i, (int) c);
//}


// Get the char at position n
//
// Parameters:
//		- n - index of desired character
//
// Returns:
//		- QChar - the character at index n
//
QChar CParse::GetChar(int n) const
{
  if (n >= 0 || n < GetKeyLength() ) return m_Key[n];
  else return QChar();
}


// Get a reversed copy of this parse
//
// Returns:
//		- CParse - a reversed copy of this parse
//
CParse  CParse::GetReverse() const
// ignores Parse structure, just reverses the string
{
  QChar* reverse;
  LxStrCpy_R( GetKeyPointer(), reverse, GetKeyLength() );
  return CParse( CStringSurrogate( reverse, 0, GetKeyLength() ) );
}

// Reverse this parse, including structure
//
void  CParse::ReverseMe()
{
    int 	tempInt, i;
    double	tempDbl;
    QChar	tempChr;

    // Reverse the key
    for( i = 0; i < m_KeyLength/2; i++ )
    {
        tempChr = m_Key[ m_KeyLength - i - 1 ];
        m_Key[ m_KeyLength - i - 1 ] = m_Key[i];
        m_Key[i] = tempChr;
    }


//for( int d = 0; d <= Size(); d++ )
//{
//	qDebug( QString( "%1" ).arg( m_Pieces[d] ) );
//}

    // Reverse the pieces
    for( i = 0; i < (m_PieceCount+1)/2; i++ )
    {
        tempInt = m_Pieces[ m_PieceCount - i ];
        m_Pieces[ m_PieceCount - i ] = m_Pieces[i];
        m_Pieces[i] = tempInt;
    }


//for( int d = 0; d <= Size(); d++ )
//{
//	qDebug( QString( "%1" ).arg( m_Pieces[d] ) );
//}

    for( i = 0; i <= m_PieceCount; i++ ) m_Pieces[i] = m_KeyLength - m_Pieces[i];


//for( int d = 0; d <= Size(); d++ )
//{
//	qDebug( QString( "%1" ).arg( m_Pieces[d] ) );
//}

    // Reverse the piece values
    for( i = 0; i < m_PieceCount/2; i++ )
    {
        tempDbl = m_PieceValues[ m_PieceCount - i ];
        m_PieceValues[ m_PieceCount - i ] = m_PieceValues[ i + 1 ];
        m_PieceValues[ i + 1 ] = tempDbl;
    }
}




// Copy a signature over this parse making
// cuts according to how the signature is
// delimited
//
// Parameters:
//		- String - a signature (QString type) with its pieces delimited by periods
//
void CParse::IngestSignature ( const QString String )
{
  Collapse ( String, QChar('.') );
}



// Tokenize the parse into pieces, the parse
// will probably be one piece at this point
// with desired pieces delimited with '-'
// and '/', a series '--' breaks into an empty
// piece
//
void CParse::Tokenize()
{
  for (int i = 0; i < GetKeyLength()-1; i++)
  {
    if ( m_Key[i] == '-' && m_Key[i+1] == '-')
    {
      CutRightBeforeHere(i);
      CutRightBeforeHere(i+2);
      i++;
    }
    else if ( m_Key[i] == '-' || m_Key[i] == '/' )
    {
      CutRightBeforeHere(i);
      CutRightBeforeHere(i+1);
    }
  }
}


// Find out if the parse is a proper
// name
//
// Returns:
//		- bool - true if the first char is upper case and the rest aren't
//
bool CParse::IsProperName() const
{
  if ( m_Key[0].category() == QChar::Letter_Uppercase )
  {
    if ( m_Key[1].category() == QChar::Letter_Lowercase )
      return true;
  }
  return false;
}


// Fill some of the key with character ch
//
// Parameters:
//		- ch - the character to fill with
//		- HowMany - the number of spaces to fill
//
void CParse::Fill ( const QChar& ch, int HowMany )
{
  if( GetKeyLength() >= HowMany )
    for( int i=0; i<HowMany; i++ )
      m_Key[i] = ch;
}


// Get a substring of all pieces before n
// and including n
//
// Parameters:
//		- n - the index of the piece desired
//
// Returns:
//		- CStringSurrogate - the substring of m_Pieces[0] through m_Pieces[n]
//
CStringSurrogate  CParse::UpToAndIncludingThisPiece( int n ) const
{
  if( n > 0 )
  {
    if( n > Size()) n = Size();
    return CStringSurrogate ( m_Key, 0, m_Pieces[n] );
  }
  else
    return CStringSurrogate();
}


// Get a surrogate substring
//
// Parameters:
//		- From - the start index
//		- Length - the length of substring
//
// Returns:
//		- CStringSurrogate - a surrogate of the substring
//
CStringSurrogate CParse::Mid(int From, int Length) const
{
    Q_ASSERT(Length >= -1);
    if (From < 0 || From > m_KeyLength)
        return CStringSurrogate();
    if (Length == -1 || Length > m_KeyLength - From)
        return CStringSurrogate(m_Key, From, m_KeyLength - From);

    return CStringSurrogate(m_Key, From, Length);
}

// Remove any pieces which don't begin
// with a given string
//
// Parameters:
//		- ssPiece - the piece to match with
//
void  CParse::RemovePiecesThatDoNotBegin ( const CStringSurrogate& ssPiece )
{
  CParse  NewParse;

  for (int i = 1; i <= (int) Size(); i++)
  {
    if ( GetPiece(i).Left( ssPiece.GetLength() ) == ssPiece )
    {
      NewParse.Append ( GetPiece(i) );
    }
  }
  CopyParse ( NewParse );
}


// Remove any pieces which don't end
// with a given string
//
// Parameters:
//		- ssPiece - the piece to match with
//
void  CParse::RemovePiecesThatDoNotEnd ( const CStringSurrogate& ssPiece )
{

  CParse  NewParse;

  for (int i = 1; i <= (int) Size(); i++)
  {
    if ( GetPiece(i).Right( ssPiece.GetLength() ) == ssPiece )
    {
      NewParse.Append ( GetPiece(i) );
    }
  }
  CopyParse ( NewParse );
}


// Remove any pieces which begin
// with a given string
//
// Parameters:
//		- ssPiece - the piece to match with
//
void CParse::RemovePiecesThatBegin ( const CStringSurrogate& ssPiece )
{

  CParse NewParse;

  for (int i = 1; i <= (int) Size(); i++)
  {
    if ( GetPiece(i).Left( ssPiece.GetLength() ) == ssPiece )
    {
      continue;
    }
    else
    {
      NewParse.Append ( GetPiece(i) );
    }
  }
  CopyParse ( NewParse );

}


// Remove any pieces which end
// with a given string
//
// Parameters:
//		- ssPiece - the piece to match with
//
void CParse::RemovePiecesThatEnd ( const CStringSurrogate& ssPiece )
{

  CParse NewParse;

  for (int i = 1; i <= (int) Size(); i++)
  {
    if ( GetPiece(i).Right( ssPiece.GetLength() ) == ssPiece )
    {
      continue;
    }
    else
    {
      NewParse.Append ( GetPiece(i) );
    }
  }
  CopyParse ( NewParse );

}


/* TODO: uncomment when CAlignment and CStringEditGrid are available

// Get the string edit distance between piece 1 of this
// and piece 1 of another CParse
//
// Parameters:
//		- Other - the other CParse
//		- Alignment - the best alignment
//
// Returns:
//		- float - the edit distance
//
float CParse::FindStringEditDistance (const CParse& Other, CAlignment& Alignment ) const
{
  QString Str1 = "#" + GetKey().Display();
  QString Str2 = "#" + Other.GetKey().Display();
  CStringEditGrid Grid (  CStringSurrogate(Str1.unicode(),0,Str1.length()  ),
              CStringSurrogate(Str2.unicode(),0,Str2.length()  ));

  return Grid.FindBestAlignment ( Alignment );
}

*/


// Count how many pieces of this match
// any piece of another parse
//
// Parameters:
//		- Parse - an alphabetized parse
//
// Returns:
//		- int - the amount of matching pieces
//
int  CParse::CountOverlapAlphabetized(CParse& Parse)
{
  Q_ASSERT( m_SortStyle == eAlphabetized && Parse.GetSortStyle() == eAlphabetized );

  int Count = 0;
  int i = 1, j= 1;
  while (i <= (int) Size() && j <= (int) Parse.Size()  )
  {
    if (GetPiece(i) == Parse.GetPiece(j) )
    {
      Count++;
      i++; j++; continue;
    }
    if (GetPiece(i) < Parse.GetPiece(j) )
    {
      i++;
      continue;
    }
    if (GetPiece(i) > Parse.GetPiece(j) )
    {
      j++;
      continue;
    }
  }
  return Count;
}

// Compute the complexity of this parse
//
// Returns:
//		- double - the complexity of this parse
//
double CParse::ComputeComplexity() const
{
  double Complexity = base2log ( Size() );
  for (int i = 1; i <= (int) Size(); i++)
  {
    double length = ThisPieceLength( i );
    Complexity += g_Lambda * length +
        length*(length+1)/2.0;

    Q_ASSERT (Complexity < 15000);
    Q_ASSERT (Complexity > 0 );
  }
  return Complexity;
}




// TODO: This is not the best way, this loses piece value
// information in some of its uses, may be best to phase it
// out

// Copy over the parse structure absorbing
// the items in a QStringList as separate
// pieces
//
// Parameters:
//		- List - the list of QStrings to absorb
//
void CParse::AbsorbQStringList( QStringList& List )
{
  int      Length = 0;

  ClearParse();
  SetLengthOfPieceVector( List.count() + 1 );
  if( m_LengthOfPieceVector < 4 ) SetLengthOfPieceVector(4);
  m_PieceCount = List.count();
  delete[] m_Pieces;
  delete[] m_PieceValues;
  m_Pieces = new int [ m_LengthOfPieceVector ];
  m_Pieces[0] = 0;
  m_PieceValues = new double [ m_LengthOfPieceVector ]; // TODO: This is bad!  Loss of piece value info
  m_PieceValues[0] = m_DefaultPieceValue;

  int k= 1;
  QStringList::Iterator it;
  for ( it = List.begin(); it != List.end(); ++it )
  {
    Length += (*it).length();
    m_Pieces[k] = Length;
    m_PieceValues[k] = m_DefaultPieceValue;
    k++;
  }

  m_KeyLength = Length;
  m_AllocatedLength = Length;
  if ( m_Key ) { delete [] m_Key; }

  m_Key = new QChar [ m_KeyLength ];

  int locator = 0;

  for ( it = List.begin(); it != List.end(); ++it )
  {
    for (int i=0; i < (int)(*it).length(); i++)
    {
      m_Key[locator + i] = (*it)[i];
    }
    locator += (*it).length();
  }
  m_PieceCount = List.count();
}


// Create a QStringList copy of this
// parse
//
// Parameters:
//		- List - the structure to copy into
//
// Returns:
//		- QStringList& - reference to copied list
//
QStringList&  CParse::CreateQStringList ( QStringList& List ) const
{
  List.clear();

  for (int i = 1; i <= Size(); i++)
  {
    List.append ( GetPiece(i).Display() );
  }

  return List;
}

// Replace a piece of a parse
//
//	Parameters:
//		- PieceNumber
//		- String surrogate of what the piece will become
//
void CParse::Replace(int PieceNumber, CStringSurrogate& NewKey)
{
    QStringList TempList;
    CreateQStringList ( TempList );
//    QStringList::Iterator it;
//    int count = 1;
//    for (it = TempList.begin(); it != TempList.end(); it++)
//    {
//        if (count == PieceNumber)
//        {
//            it = TempList.removeAt(it);
//            TempList.insert(it, NewKey.Display() );
//        }
//        count++;
//    }

    int i;
    int count = 1;
    int len = TempList.count();
    for (i = 0; i < len; i++) {
        if (count == PieceNumber) {
            TempList.removeAt(i);
            TempList.insert(i, NewKey.Display());
        }
        count++;
    }
    AbsorbQStringList (TempList);
}

// Make this Parse equal to an alphabetized
// list of trigrams of a string
//
// Parameters:
//		- string - the string to use
//		- length - the length of the string
//
void CParse::FindAlphabetizedTrigrams (QChar* string, int length)
{
  QChar* NewString = NULL;
  QStringList strlist;
  QString temp;

  // Put a # at the beginning and end:

  QChar* pound  = new QChar [1];
  pound[0]    = '#';
  NewString    = new QChar [1];
  NewString[0]  = pound[0];
  LxStrCat(NewString, string, 1, length);
  LxStrCat(NewString, pound, length+1, 1);
  string = NewString;
  int i;

   // =========================================

  // Get all the trigrams and sort them
  for ( i = 0 ; i < length; i++)
  {
    temp += string[i];
    temp += string[i+1];
    temp += string[i+2];
    strlist << temp;
    temp = "";
  }
  strlist.sort();


  // Make this parse equal to the trigram list
  ClearParse();

  m_AllocatedLength  = 3 * length + 1;
  if (m_Key) { delete [] m_Key; }
  m_Key = new QChar[ m_AllocatedLength ];

  SetLengthOfPieceVector( length + 1 );
  m_PieceCount = length;
  delete[] m_Pieces;
  m_Pieces = new int [ m_LengthOfPieceVector ];
  m_Pieces[0] = 0;
  delete[] m_PieceValues;
  m_PieceValues = new double [ m_LengthOfPieceVector ];
  m_PieceValues[0] = m_DefaultPieceValue;

  int pos = 0;
  for ( QStringList::Iterator it = strlist.begin(); it != strlist.end(); ++it )
  {
    m_Key[pos++] = (*it).unicode()[0];
    m_Key[pos++] = (*it).unicode()[1];
    m_Key[pos++] = (*it).unicode()[2];
    }

  for ( i = 0; i < length; i++)
  {
    m_Pieces[i+1] = (i+1) * 3;
    m_PieceValues[i+1] = m_DefaultPieceValue;
  }
  m_PieceCount = length;

  if (NewString) delete NewString;

}


QTextStream& operator<< (QTextStream& Stream, CParse& Parse)
{
  Stream << Parse.m_PieceCount;
  for (int i = 1; i<= Parse.m_PieceCount ; i++)
  {
    Stream << Parse.GetPiece(i).Display();
  }
  return Stream;
}


void CParse::CopyParseStructure (const CParse& other)
{
    if( Display() != other.Display() )
    {
        return;
    }


    m_PieceCount = other.Size();

    delete[] m_Pieces;
    m_Pieces = new int[ m_PieceCount+1 ];

    for( int i=0; i <= m_PieceCount; i++ )
    {
        m_Pieces[i] = other.GetPieces()[i];
    }

    Q_ASSERT( Display('.') == other.Display('.') );
}



void CParse::CopyParseStructureWithoutBoundaries (const CParse& other)
{
    int i;

    if( GetKey() != other.Mid( 1, other.GetKeyLength() - 2 ) )
    {
        return;
    }

    m_PieceCount = other.Size();

    delete[] m_Pieces;
    m_Pieces = new int[ m_PieceCount+1 ];

    m_Pieces[0] = 0;
    for( i=1; i <= m_PieceCount; i++ )
    {
        m_Pieces[i] = other.GetPieces()[i] - 1;
    }
    m_Pieces[i] = m_Pieces[i]-1;
}



// Yuhu added for SED
void CParse::yuhuMergeParse(CParse* Other)
{
    int			i;
    int			start, end;
//	char		buffer[20];

    for (i =1; i<= Other ->Size(); i++)
    {
        start = Other ->m_Pieces [ i - 1 ];
        end =  Other ->m_Pieces [ i ];
        //gcvt(end, 2, buffer);
        //AfxMessageBox(buffer);
        CutRightBeforeHere(end);
    }

    return;
}


// Compute the description length of this parse
//

double CParse::ComputeDL( int char_count ) const
{
    if( Display() == "NULL" ) return 0.0;
    return ((double)m_KeyLength) * base2log( (double) char_count );
}

//------------------------------------------------------------------------//
//  Finds the location of a piece of the parse, and
//  increments its value by an amount.
//
// Parameters:
//		- ssPiece - the piece to match with
//      - increment -- the amount by which incremented
void CParse::IncrementPieceValue(CStringSurrogate ssPiece, double increment)
{
    int loc = Find(ssPiece);
    SetPieceValue (loc, GetPieceValue(loc) + increment);
}

//------------------------------------------------------------------------//
//  Sorts the parse by the PieceValues array.
//
//
// No parameters:
//
void  CParse::SortByValue() ///< Sort the pieces of the parse by the PieceValues.
{
    CParse      myCopy;
    int         size = Size();

    myCopy  =  *this;

    int*        PiecePointers   = new int[ size ];
    double*     Values          = new double [size];
    for (int j = 0; j < size; j++) { Values[j] = m_PieceValues[j+1]; }


    SortVector (PiecePointers, Values, size );
    m_SortStyle = eNotSorted;
    ClearParse();


    for (int i = 0; i < size; i++)
    {
        Append ( myCopy.GetPiece( PiecePointers[i] + 1 ) );
    }

    //------------//
    // Note that the Values are not in the right order for this new sort -- fix this. JG August 2008.
    //------------//

}
//------------------------------------------------------------------------//
//  Sorts the parse by Piece length.
//
//
// No parameters:
//
void  CParse::SortByPieceLength() ///< Sort the pieces of the parse by the length of the pieces.
{
    CParse      myCopy;
    int         size = Size();

    myCopy  =  *this;

    int*        PiecePointers   = new int[ size ];
    double*     Values          = new double [size];
    for (int j = 0; j < size; j++) { Values[j] = -1 * ThisPieceLength(j+1); }


    SortVector (PiecePointers, Values, size );
    m_SortStyle = eNotSorted;
    ClearParse();


    for (int i = 0; i < size; i++)
    {
        Append ( myCopy.GetPiece( PiecePointers[i] + 1 ) );
    }


}
void CParse::MergeParse (CParse& Other, CParse& Output)
{
    int	 	i = 1,
            j=  1,
            Winner;
    QString ThisPiece,
            ThatPiece;
    CSS		ssThis, ssThat;

    Output.ClearParse();
    while ( i <= Size() || j<= Other.Size() )
    {
        if ( j > Other.Size() )
        {
            Winner = 1;
            //ThisPiece = GetAt(i);
            ssThis = GetPiece(i);

        } else
        {
            if ( i > Size() )
            {
                Winner = 2;
                //ThatPiece = Other.GetAt(j);
                ssThat = Other.GetPiece(j);

            } else
            {
                //ThisPiece = GetAt(i);
                //ThatPiece = Other.GetAt(j);

                ssThis = GetPiece(i);
                ssThat = Other.GetPiece(j);

                //if (ThisPiece < ThatPiece)
                if (ssThis < ssThat)
                {
                    Winner = 1;
                } else
                {
                    //if ( ThisPiece > ThatPiece )
                    if ( ssThis > ssThat )
                    {
                        Winner = 2;
                    } else
                    {
                        Winner =  3;
                    }
                }
            }
        }

        switch (Winner) {
            case 3:{		//the same piece
                //Output->Append(ThisPiece);
                Output.Append(ssThis);
                i++;
                j++;
                break;
            }
            case 1: {
                //Output.Append(ThisPiece);
                Output.Append(ssThis);
                i++;
                break;
            }
            case 2:  {
                //Output.Append(ThatPiece);
                Output.Append(ssThat);
                j++;
                break;
            }
        }
    }
    Q_ASSERT (Size() < 2 || ThisPieceLength(1)  > 0 );
}
/*
void CParse::MergeParse (CParse* Other)
{
    CParse Merged;
    MergeParse (Other, &Merged);
    this->ClearParse();
    this->Equals(Merged);
    return;
}
*/
