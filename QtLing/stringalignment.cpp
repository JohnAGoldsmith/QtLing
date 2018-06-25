#include "stringalignment.h"
#include <algorithm>

int find_length_of_longest_common_right_substring(QString, QString);
void SortQStringListFromRight(QStringList& ThisStringList);


StringAlignment::StringAlignment(QStringList strings1, QStringList strings2, eDirectionality directionality)
{
    m_strings1 = strings1;
    m_strings2 = strings2;
    m_directionality = directionality;

    find_edge_strings();
};


/* This function finds the minimal peripheral string that distinguishes each string from its
 * predecessor and successor in a sorted list of strings.
 */
void StringAlignment::find_edge_strings()
{
    if (m_directionality == Dir_left_to_right) { // normal direction for suffixes -- so we scan right-to-left!
    end_sort();

    // first:
    int this_length =  find_length_of_longest_common_right_substring(m_strings1[0], m_strings1[1]);
    m_edge_strings1[0] = m_strings1[this_length];

    // middle:
    for (int i = 1; i < m_strings1.length()-1; i++){
        int upper = find_length_of_longest_common_right_substring(m_strings1[i-1], m_strings1[i]);
        int lower = find_length_of_longest_common_right_substring(m_strings1[i]  , m_strings1[i+1]);
        this_length = std::min(upper, lower);
        m_edge_strings1[i] = m_strings1[this_length];
    }

    // last:
    int k = m_strings2.length()-1;
    this_length =  find_length_of_longest_common_right_substring(m_strings2[k-1], m_strings2[k]);
    m_edge_strings2[k] = m_strings2[this_length];

    // first:
    this_length =  find_length_of_longest_common_right_substring(m_strings2[0], m_strings2[1]);
    m_edge_strings2[0] = m_strings2[this_length];

    // middle:
    for (int i = 1; i < m_strings2.length()-1; i++){
        int upper = find_length_of_longest_common_right_substring(m_strings2[i-1], m_strings2[i]);
        int lower = find_length_of_longest_common_right_substring(m_strings2[i]  , m_strings2[i+1]);
        this_length = std::min(upper, lower);
        m_edge_strings2[i] = m_strings2[this_length];
    }

    // last:
    int i = m_strings1.length()-1;
    this_length =  find_length_of_longest_common_right_substring(m_strings1[i-1], m_strings1[i]);
    m_edge_strings1[i] = m_strings1[this_length];


    } // end of Direction left to right
    else{

    }
}
void StringAlignment::beginning_sort()
{
    m_strings1.sort();
    m_strings2.sort();
}

void StringAlignment::end_sort()
{
    SortQStringListFromRight(m_strings1);
    SortQStringListFromRight(m_strings2);
 }
