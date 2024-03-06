#ifndef SIGNATURE_TREE_MODEL_H
#define SIGNATURE_TREE_MODEL_H
#include <QAbstractItemModel>
#include "SignatureCollection.h"


class signature_tree_model: public QAbstractItemModel
{
    CSignatureCollection *      m_signatures;
    CLexicon*                  m_lexicon;


public:
    explicit signature_tree_model(CSignatureCollection *, bool suffix_flag, QObject *parent = nullptr);




 };

#endif // SIGNATURE_TREE_MODEL_H
