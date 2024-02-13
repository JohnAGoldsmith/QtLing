
#include "morphemeparse.h"

MorphemeParse::MorphemeParse(QStringList these_morphs, int root) {
    morphs = these_morphs;
    root_index = root;
    first_index = -1 * root_index;
    last_index = these_morphs.count() - root_index;
}
MorphemeParse::MorphemeParse(QString word){
    morphs.append(word);
    root_index = 0;
    first_index = 0;
    last_index = 0;
}


QString MorphemeParse::get_morph(int n){
    if (n < first_index || n> last_index) {return QString();}
    return morphs[root_index + n];
}

