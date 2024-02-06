#include "spine.h"
#include "Lexicon.h"

// A spine is a list of pairs, which consist of a signature string and a string difference between stems.
// E.g., List: (NULL=ed=ing=s, e), (e=ed=ing=s, "")
Spine::Spine()
{
}
Spine::~Spine(){
}
void Spine::add(QString sigstring, QString diff){
    QPair<QString,QString> * new_pair = new QPair<QString,QString>(sigstring,diff);
    m_pairs.append(new_pair);
}


