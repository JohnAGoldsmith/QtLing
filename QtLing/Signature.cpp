#include "Signature.h"

CSignature::CSignature(CStringSurrogate ssWord) : CParse(ssWord), m_Word(ssWord.GetKey()), m_Signatures() {}

CSignature::CSignature(CSignature& signature) {
    m_Signature = signature.GetSignature();
    m_Affixes(signature.GetList());
}

