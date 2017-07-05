#include "CompareFunc.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <QStringList>
#include <QString>
#include <QChar>


int GreaterThan (const void *pA, const void *pB)
{
  struct Pair
  {
    int    Index;
    double  Value;
  };

    Pair F1 = *static_cast<const Pair*>(pA);
    Pair F2 = *static_cast<const Pair*>(pB);

  if ( F1.Value == F2.Value ) { return 0; }
  if ( F1.Value >  F2.Value )  { return -1; }
  return 1;

};

int FollowsAlphabetically(const void *pA, const void *pB)
{
  struct Pair
  {
    int      Index;
    QString  Value;
  };

    Pair F1 = *static_cast<const Pair*>(pA);
    Pair F2 = *static_cast<const Pair*>(pB);

  if ( F1.Value == F2.Value ) { return 0; }
  if ( F1.Value >  F2.Value )  { return -1; }
  return 1;

};

namespace {
    int compare_int(int a, int b)
    {
        // cannot just return a - b because overflow is possible
        return a > b ? 1 : a < b ? -1 : 0;
    }

    int compare_qchar(QChar a, QChar b)
    { return compare_int(a.unicode(), b.unicode()); }

    int compare_reversed_string(const QString& a, const QString& b)
    {
        if (a.size() > b.size())
            return -compare_reversed_string(b, a);

        typedef std::reverse_iterator<QString::const_iterator>
            riterator;
        const riterator rbegin_a = riterator(a.constEnd());
        const riterator rend_a = riterator(a.constBegin());
        const riterator rbegin_b = riterator(b.constEnd());

        const std::pair<riterator, riterator> x = std::mismatch(
            rbegin_a, rend_a, rbegin_b);
        if (x.first == rend_a)
            // tails match
            return compare_int(a.size(), b.size());
        return compare_qchar(*x.first, *x.second);
    }
}

/// sort strings from their right ends, rather than their left ends.
int ReversedFollowsAlphabetically(const void *pA, const void *pB)
{
    typedef std::pair<int, QString> item;
    return compare_reversed_string(
        static_cast<const item*>(pA)->second,
        static_cast<const item*>(pB)->second);
}


//----------------------------------------------------------------------------------------------//
void SortVector(int* Output, const double* Input, int Length)
{
  int i;
  struct Pair
  {
    int    Index;
    double  Value;
  };
  Pair* Pairs = new Pair[ Length ];
  for ( i = 0; i < Length; i++)
  {
    Pairs[i].Index = i;
    Pairs[i].Value = Input[i];
  }
  qsort( Pairs, Length, sizeof( Pair ), GreaterThan );
  for ( i = 0; i < Length; i++)
  {
    Output[i] = Pairs[i].Index;
  }

  delete [] Pairs;
  return;
}
//----------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------//
void SortReversedStrings (int* Output, const QStringList& Input, int Length)
{
  int i;
  typedef std::pair<int, QString> Pair;
  Pair* Pairs = new Pair[ Length ];
  for ( i = 0; i < Length; i++)
  {
    Pairs[i].first = i;
    Pairs[i].second = Input[i];
  }
  qsort ( Pairs, Length, sizeof( Pair ), ReversedFollowsAlphabetically );
  for ( i = 0; i < Length; i++)
  {
    Output[i] = Pairs[i].first;
  }

  delete [] Pairs;
  return;
}
//----------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------//
void SortQStringListFromRight(QStringList& ThisStringList)
{
    int             Length      = ThisStringList.count();
    QStringList     CopyOfMe    = ThisStringList;
    int*            IndexList   = new int[ Length ];

    SortReversedStrings (IndexList, ThisStringList, Length);
    ThisStringList.clear();
    for (int i = 0; i < Length; i++)
    {
       ThisStringList.append(CopyOfMe[ IndexList[i] ]);
    }
}
//----------------------------------------------------------------------------------------------//
