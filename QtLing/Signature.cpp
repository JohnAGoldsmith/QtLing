#include "Signature.h"

CSignature::CSignature(CStringSurrogate ssWord) : CParse(ssWord), m_Signature(ssWord.GetKey()), m_Affixes() {}

CSignature::CSignature(CSignature& signature) {
    m_Signature = signature.GetSignature();
    m_Affixes = signature.GetList();
}

