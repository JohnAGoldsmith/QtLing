#include <QFile>
#include "Lexicon.h"
#include "SignatureCollection.h"
#include "StemCollection.h"
#include "SuffixCollection.h"
#include "WordCollection.h"
#include "Word.h"

void CLexicon::print_signature_transforms(){
    QFile transforms("sig_transforms.txt");
    transforms.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out_transforms(&transforms);

    //QStringList sig_transforms;
    if (m_suffix_flag){
        m_Signatures->compute_signature_transforms(m_suffix_flag, out_transforms);
    }

}
