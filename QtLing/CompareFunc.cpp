#include "CompareFunc.h"

#include <algorithm>
#include <iterator>
#include <utility>
#include <QStringList>
#include <QString>
#include <QChar>
#include <QDebug>

QString QStringList2QString(QStringList string_list){
    QString string;
    if (string_list.length() == 0) return string;
    for (int i =0; i< string_list.length()-1; i++){
        string += string_list[i] + "=";
    }
    string += string_list.last();
    return string;
}


bool signature_1_contains_signature_2(QString sig1, QString sig2)
{
    QStringList list_1 = sig1.split("=");
    QStringList list_2 = sig2.split("=");
    foreach (QString affix, list_2 ){
        if (! list_1.contains(affix)){
            return false;
        }
    }
    return true;
}

bool contains_affix_string(QString sig1, QString affix)
{
    QStringList affixes = sig1.split("=");
    if (affixes.contains(affix) ){
        return true;
    }
    return false;
}


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
        // the following has been in here for years...but it's wrong. July 2018.
//        if (a.size() > b.size())
//            return -compare_reversed_string(b, a);

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
int find_length_of_longest_common_right_substring(QString string1, QString string2){
    int i = 0;
    int len1 = string1.length();
    int len2 = string2.length();
    while ( i < len1 && i < len2 ){
        if (string1[len1 - i] != string2[len2 - i]){
            break;
        }
        i--;
    }
    return i;
}
//----------------------------------------------------------------------------------------------//
int find_length_of_longest_common_lef_substring(QString string1, QString string2){
    int i = 0;
    int len1 = string1.length();
    int len2 = string2.length();
    while ( i < len1 && i < len2 ){
        if (string1[i] != string2[i]){
            break;
        }
        i++;
    }
    return i;
}
//----------------------------------------------------------------------------------------------//
void invert(QStringList list){    // this is not currently used.
    QStringList this_string_list;
    QStringList these_letters;
    foreach (QString this_string,list){
        these_letters = this_string.split("");
        std::reverse(these_letters.begin(),these_letters.end());
        this_string = these_letters.join("");
    }
}
