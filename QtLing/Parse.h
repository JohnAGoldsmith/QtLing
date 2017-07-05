#ifndef CPARSE_H
#define CPARSE_H


class CParse;

#include <QChar>
#include "StringSurrogate.h"
#include "generaldefinitions.h"
class QString;
template<class K, class V> class QMap;

/// A parse is a string (i.e., vector of QChar) partitioned into continguous
/// segments.  In Linguistica, such objects are used in a few different ways:
///  * as a general purpose string value type (i.e., as the underlying storage
///    for CStringSurrogates)
///  * as a general purpose list-of-strings type
///  * to represent a word parsed into morphemes.
///
/// Segment indices are 1-based to allow considering the beginning of
/// the string as the “end of segment 0”.
//  m_Pieces's length must always be at least 1 greater than PieceCount;
//  though m_Pieces[0] is always 0.
//	xx xxx xx:	is represented:
//	0 2 5 7
//  so Piece[1] goes from Key[m_Pieces[0]] to
//						  Key[m_Pieces[1] - 1];
//  Piece[2] goes from  Key[m_Pieces[1]] to
//						Key[m_Pieces[2]-1]
//	abcd
//  m_Pieces array:
//  0  4; so Piece[1] goes from Key[m_Pieces[0]] to
//							to Key[m_Pieces[1] - 1];
//	Key's length will alway be m_Pieces[ PieceCount ];
//  And: this + m_Pieces[ k ] points to the beginning of Piece # k
class CParse {
protected:
    /// beginning of managed string
    class QChar* m_Key;
    /// length allocated for string use
    int m_AllocatedLength;
    /// length of string --- i.e., length of the user-visible portion
    /// (so m_KeyLength <= m_AllocatedLength)
    int m_KeyLength;

    /// offsets for ends of segments.
    /// (so m_Pieces[0] == 0)
    int* m_Pieces;
    /// values associated to segments (for general use)
    /// m_PieceValues[0] is unused
    double* m_PieceValues;
    /// number of segments.
    /// (the last segment has index m_PieceCount)
    int m_PieceCount;
    /// length allocated for m_Pieces, m_PieceValues use
    /// (m_LengthOfPieceVector >= m_PieceCount + 1)
    int m_LengthOfPieceVector;

    /// default value for new pieces
    double m_DefaultPieceValue;

    /// alphabetize inserted strings?
    enum eParseSortStyle m_SortStyle;

    /// extra tidbit for general use!
    float m_Evaluation;
public:
    // construction/destruction.

    CParse(double default_piece_value = 0.0);
    CParse(const CParse&, double = 0.0);
    CParse(const CStringSurrogate&, double = 0.0);
    CParse(const QChar&, double = 0.0);
    virtual ~CParse();

    // copy.

    CParse& operator=(const CParse& Parse)
    { CopyParse(Parse); return *this; }
    CParse& operator=(const CParse* pParse)
    { CopyParse(*pParse); return *this; }
    CParse& operator=(const QChar& ch) { SetKey(ch); return *this;}
    CParse& operator=(const CStringSurrogate SS);
    void CopyParseStructure(const CParse&);
    void CopyParseStructureWithoutBoundaries(const CParse&);
    void AbsorbQStringList(class QStringList&);
    void CopyParse(const CParse&);
    void Collapse(const CStringSurrogate&, const QChar& = ' ');
    void IngestSignature(const QString);
    void SetKey(const CStringSurrogate);
    void SetKey(const QChar*, int);
    void SetKey(const QChar&);
    void SetKey(const CParse&);
    void SetKey(const CParse* Parse) { SetKey(*Parse); }

    // move.

    /// XXX. deletes key twice!
    void TakeLeft(const CParse&, int);
    void TakeLeft(const CParse* pParse, int n) { TakeLeft(*pParse, n); }
    void TakeRight(const CParse&, int);
    void TakeRight(const CParse* pParse, int n) { TakeRight(*pParse, n); }

    // clear.

    void ClearParse();

    /// assign alphabetized list of the trigrams in str
    void FindAlphabetizedTrigrams(QChar* str, int len);
    /// assign underlying string
    void ClearParseStructure();
    /// assign underlying string, but remember 0th piece value.
    void SimplifyParseStructure();

    // append/insert.

    void operator<<(double n) { Append(n); }
    void operator<<(int n) { Append(n); }
    void operator<<(const CParse* Parse) { Append(Parse); }
    void operator<<(const CStringSurrogate SS);
    CParse operator+=(const CParse*);
    CParse operator+=(const CStringSurrogate);
    void Append(const CStringSurrogate str);
    void Append(const QChar& ch);
    void Append(double piece);	///< Append(QString::number(piece))
    void Append(int piece);	///< Append(StringToIntWithCommas(piece))
    void Append(const CParse& str);
    void Append(const QString& str) { Append(CStringSurrogate(str)); }
    /// deprecated
    void Append(const CParse* str) { Append(*str); }
    void AppendCR();
    void AppendHyphen();
    void AppendLine(const CStringSurrogate& line);
    void AppendNewLine();
    void AppendNoCommas(int piece);
    void AppendTab();
    void AppendInAlphabeticalOrder(const CStringSurrogate&,
        bool IsTemplateOperator = false);
    void AppendInAlphabeticalOrder(const CParse*);

    // concatenate.

    CParse operator+(const CParse*) const;
    CParse operator+(const CStringSurrogate) const;

    // remove.

    CParse operator-=(const CStringSurrogate);
    /// Remove the first segment that compares equal to piece.
    /// If no such segment exists, do nothing.
    void Remove(const CStringSurrogate& piece);
    /// Requires: 1 <= i <= m_PieceCount
    /// erase i'th piece
    void RemovePiece(int i);

    // split segment.

    void operator/=(int i);
    void CutNFromTheLeft(int);
    void CutNFromTheRight(int);
    void CutRightBeforeHere(int);

    // merge/combine segments.

    void MergePieces(int);

    // move inter-segment cut.

    void MoveThisManyLettersLeftwardFromArg1(int, int);
    void MoveThisManyLettersRightwardFromArg1(int, int);
    void SetBreak(int, int);

    // number of segments.

    int Size() const { return m_PieceCount; }

    // segment access.

    CStringSurrogate operator[](int) const;
    CStringSurrogate GetLastPiece() const;
    CStringSurrogate GetPiece(int) const;
    int* GetPieces() const { return m_Pieces; }
    int GetPositionOfFirstCharOfThisPiece(const int) const;
    CStringSurrogate ThisPieceAndWhatFollows(int) const;
    int ThisPieceLength(int) const;
    CStringSurrogate UpToAndIncludingThisPiece(int) const;
    CStringSurrogate WhatPrecedesPiece(int) const;
    int GetPieceLoc(int piececount)
    {
        Q_ASSERT(piececount > 0);
        return m_Pieces[piececount];
    }

    // general-purpose segment data

    double GetPieceValue(int) const;
    double* GetPieceValues() const { return m_PieceValues; }
    /// Find a particular "piece" of the parse, and
    /// increment its value-count.
    void IncrementPieceValue(CStringSurrogate, double);
    void SetPieceValue(int, double);

    // entire string

    CStringSurrogate GetKey() const;
    int GetKeyLength() const { return m_KeyLength; }

    // substring.

    CStringSurrogate operator()(int, int) const; // in underlying string
    CStringSurrogate Mid(int, int) const;
    CStringSurrogate Left(int) const;
    CStringSurrogate Right(int) const;

    // character access.
    QChar GetChar(int) const;

    // comparison.

    bool operator==(const CParse*) const;
    bool operator!=(const CParse* Parse) const
    { return !(*this == Parse); }

    // XXX. poor man’s run-time type information
    virtual eLxaClass GetType() const { return PARSE_CLASS; }

    // maintaining sort.

    enum eParseSortStyle GetSortStyle() const { return m_SortStyle; }
    void SetSortStyle(enum eParseSortStyle e) { m_SortStyle = e; }
    void NotAlphabetized() { m_SortStyle = eNotSorted; }

    // sort.

    void Alphabetize();
    void ReverseAlphabetize();
    /// Sort the pieces of the parse by piece lengths,
    /// in decreasing size.
    void SortByPieceLength();
    /// Sort the pieces of the parse by the PieceValues.
    void SortByValue();

    // queries.

    bool AreAllPiecesSingletons() const;
    int CountOverlapAlphabetized(CParse&);
    /// Index for the first segment that compares equal to piece.
    /// If no such segment exists, result is 0.
    int Find(const CStringSurrogate& piece) const;
    bool IsAlphabetical() const;
    bool IsProperName() const;

    // split/parse.

    void Tokenize();

    // conversion

    class QStringList& CreateQStringList(class QStringList&) const;
    QString Display(const QChar, QMap<QString, QString>* filter = 0) const;
    QString Display(QMap<QString, QString>* filter = 0) const;

    // prefix/suffix extraction helpers.

    int DeleteLastCharsOfAllPieces(int, CParse&) const;
    int DeleteLeadingCharsOfAllPieces(int, CParse&) const;
    QChar FindCommonFirstLetterFromAll() const;
    QChar FindCommonLastLetterFromAll() const;
    CParse RipOut(QChar, CParse* = 0) const;
    bool TestIfCharPlusSigEqualsThis(const CParse&, const QChar) const;
    void PrefixToAllPieces(const CStringSurrogate&);
    void PrefixToAllPieces2(const CStringSurrogate&);
    void PrefixToAllPieces(const QChar ch);
    void RemovePiecesThatDoNotBegin(const CStringSurrogate&);
    void RemovePiecesThatDoNotEnd(const CStringSurrogate&);
    void RemovePiecesThatBegin(const CStringSurrogate&);
    void RemovePiecesThatEnd(const CStringSurrogate&);
    void RemovePrefixFromAllPieces(const CStringSurrogate&);
    void RemoveSuffixFromAllPieces(const CStringSurrogate&);
    void SuffixToAllPieces(const CStringSurrogate&);
    void SuffixToAllPieces2(const CStringSurrogate&);
    void SuffixToAllPieces(const QChar ch);

    // extra general-purpose value.

    float GetEvaluation() const { return m_Evaluation; }
    void SetEvaluation(float e) { m_Evaluation = e; }

    // data, capacity.

    int GetAllocatedLength() const { return m_AllocatedLength; }
    QChar* GetKeyPointer() const { return m_Key; }
    int GetLengthOfPieceVector() const { return m_LengthOfPieceVector; }

    // set operations.

    CParse Intersection(const CParse&) const;
    bool Overlap(const CParse&) const;
    void MergeParse(const CParse&);
    void MergeParse(CParse& OtherInput, CParse& Output); // allomorphy. Change?
    void yuhuMergeParse(CParse*);
    void MergeAndAlphabetizeParse(const CParse&);
    bool Contains(const CParse&) const;
    bool Contains(const CParse* pParse) const
    { return Contains(*pParse); }
    bool Contains(const CStringSurrogate&) const;
    bool ContainsNULL() const;

    // reverse.

    CParse GetReverse() const;
    void ReverseMe();

    // rewrite a segment.

    void Replace(int, CStringSurrogate&);

    // partially overwrite.

    void Fill(const QChar&, int);

    /// add a "\r\n" segment before each segment
    void InsertCRs();

    // description length.

    double ComputeComplexity() const;
    virtual double ComputeDL(int char_count = 26) const;
protected:
    virtual void SetLengthOfPieceVector(int i)
    { m_LengthOfPieceVector = i; }
private:
    void AddKeyLength(int);
    void AddSlots(int);

    void CheckSize(int, int);
    void SetPieceCount(int);

    friend class QTextStream& operator<<(
        class QTextStream& Stream, CParse& Parse);
};

bool AnyOverlap(const CParse&, const CParse&);


#endif // CPARSE_H
