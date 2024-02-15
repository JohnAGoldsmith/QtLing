#include "spine.h"
#include "Lexicon.h"
#include "WordCollection.h"
#include "sigpaircollection.h"

// A spine is a list of pairs, which consist of a signature string and a string difference between stems.
// E.g., List: (e=ed=ing=s, e), (NULL=d=s, "")
Spine::Spine()
{
}
Spine::~Spine(){
}
void Spine::add(QString sigstring, QString diff){
    QPair<QString,QString> * new_pair = new QPair<QString,QString>(sigstring,diff);
    m_pairs.append(new_pair);
}

QString Spine::display(){
    QString result;
    for (int n = 0; n < m_pairs.length(); n++){
        result += m_pairs[n]->first + " " + m_pairs[n]->second + "; ";
    }
    return result;
}


