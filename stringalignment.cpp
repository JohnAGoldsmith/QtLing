#include "stringalignment.h"
#include <algorithm>
#include <QDebug>






int find_length_of_longest_common_right_substring(QString, QString);
void SortQStringListFromRight(QStringList& ThisStringList);


StringAlignment::StringAlignment(QStringList strings1, QStringList strings2, eDirectionality directionality)
{
    m_strings1 = strings1;
    m_strings2 = strings2;
    my_best_match = QVector<int>(strings1.length());
    other_best_match = QVector<int>(strings2.length());
    m_directionality = directionality;
};

QString StringAlignment::display(){
    QString out ;
    out =  m_strings1.join("=") + "  ";
    out += m_strings2.join("=") + "  ";
    for (int i = 0; i < m_strings1.count(); i++){
        out += " " + m_strings1[i] + "->";
        out += m_strings2[my_best_match[i]];
        out += " ; ";
    }
    out += " reverse order: ";
    for (int i = 0; i < m_strings2.count(); i++){
        out += " " + m_strings2[i] + "->";
        out += m_strings1[other_best_match[i]];
        out += "  ";
    }
    return out;
}

// helper for "calculate_distances"
int overlap(QString & string1, QString & string2){
    QString string2_copy = string2;
    int total = 0;
    if (string1 == "NULL"){
        if (string2 == "NULL"){
            total += 1;
            return total;
        }
        return total;
    }
    if (string2 == "NULL"){
        return total;
    }
    for (int i = 0; i < string1.length(); i++){
        int exist = string2_copy.indexOf( string1[i] );
        if ( exist >= 0 ){
            total++;
            string2_copy.remove(exist,1);
        }
    }
    return total;
}
// helper for "calculate_distances"
int unmatched(QString &  string1, QString & string2 ){
    int total = 0;
    if (string1 == "NULL"){
        return 0;
    }
    for (int i = 0; i < string1.length(); i++){
        if ( string2.indexOf(string1[i]) < 0 ){
             total++;
        }
    }
    return total;
}
// helper for "match_up"
void calculate_closeness(QString string1, QStringList strings2, QVector<float> * normalized_closeness ){
    /*!
     *  Compute a distribution describing asymmetric string similarity:
     *  Calculate how many letters in string1 are in string2; and how many are not in string2;
     *  subtract second from first. Can be positive or negative. Calculate a boltzmann style
     *  probability: take exp of value for each, then assign a closeness value to each
     *  as the exp(value) divided by the total of the exp(value)'s. This is normalized closeness.
     */
    float running_total(0.0);
    QVector<float> closeness(strings2.count());
    for (int i = 0; i < strings2.count(); i++){
        int match_count =  overlap(string1, strings2[i]);
        int unmatched_count = unmatched(string1, strings2[i]);
        float this_value = qExp (match_count - unmatched_count);
        closeness[i] = this_value ;
        running_total += this_value;
        //qDebug() << "  90" << "match count"<< match_count << "unmatched count"<< unmatched_count;
    }
    for (int i = 0; i < strings2.count(); i++){
        (*normalized_closeness)[i] = closeness[i]/running_total;
        //qDebug() << 94 << string1 <<  i << strings2[i] << closeness[i]/running_total;
    }
}
double find_closest_item(QVector<QVector<float>*> & v1, QVector<QVector<float>*>& v2, QVector<int>& best_match){
    float temp_best (0.0), closeness(0.0);
    double tightness(0.0);
    int temp_best_candidate = -1;
    int size = v1.length();
    for (int i = 0; i < size; i++){
        temp_best = 0.0;
        temp_best_candidate = -1;
        for (int j = 0; j< size; j++){
            closeness = v1[i]->at(j) * v2[j]->at(i);
            if (closeness > temp_best){
                temp_best = closeness;
                temp_best_candidate = j;
            }
        }
        best_match[i] = temp_best_candidate;
        tightness += temp_best;
    }
    return tightness;
}

void StringAlignment::match_up(){
    bool verboseFlag = true;
    tightness = 0.0;
    int size = m_strings1.length();
    if ( ! (size == m_strings2.length()) ) { return;}
    QVector<QVector<float>*> my_closeness_measures(m_strings1.count());
    QVector<QVector<float>*> other_closeness_measures(m_strings2.count());
    for (int i = 0; i < size; i++){
        QVector<float>* closenesses = new QVector<float>(m_strings2.count());
        my_closeness_measures[i] = closenesses;
        calculate_closeness(m_strings1[i], m_strings2, closenesses);
    }
    // Now calculate closenesses from the other signature affixes;
    // this is also a distribution.
    for (int j = 0; j < size; j++){
        QVector<float>* closenesses = new QVector<float>(m_strings2.count());
        other_closeness_measures[j] =  closenesses;
        calculate_closeness(m_strings2[j], m_strings1, closenesses);
    }
    float temp_best (0.0), closeness(0.0);
    int temp_best_candidate = -1;
    tightness  = find_closest_item(my_closeness_measures, other_closeness_measures, my_best_match);
    tightness += find_closest_item(other_closeness_measures, my_closeness_measures, other_best_match);
    if ( ! properly_aligned()) {
        if (verboseFlag) { qDebug() << "\n"<<  146 << "Alignment not accomplished on first pass"; }
        for (int i_1 = 0; i_1 < size; i_1++){
            int i_2 = my_best_match[i_1];
            if (other_best_match[i_2]==i_1){
                // this and other are in agreement; they select each other
                for (int j = 0; j < size; j++){
                    if ( ! (j == i_2)){
                        (*my_closeness_measures[i_1])[j] = 0.0;
                    }
                }
                for (int j = 0; j < size; j++){
                    if (! (j == i_1)){
                        (*other_closeness_measures[i_2])[j] = 0.0;
                    }
                }
            }
        }
        find_closest_item(my_closeness_measures, other_closeness_measures, my_best_match);
        find_closest_item(other_closeness_measures, my_closeness_measures, other_best_match);
    }
    if (verboseFlag) {qDebug() << 166 << "Aligned? " << properly_aligned() << "tightness: " <<  tightness  <<   display() ; }

}
bool StringAlignment::properly_aligned(){
    if ( ! (m_strings1.length() == m_strings2.length())) { return false;}
    int size = m_strings1.length();
    for (int i = 0; i < size; i++){
        int other = my_best_match[i];
        if (! (other_best_match[other] == i) ){
            return false;
        }
    }
    return true;
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
