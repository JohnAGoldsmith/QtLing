#include "Lexicon.h"

//#include "SignatureCollection.h"
//#include "StemCollection.h"
//#include "SuffixCollection.h"
//#include "WordCollection.h"
//#include "StringSurrogate.h"
#include "Word.h"
CLexicon::CLexicon() : m_Words(), m_Stems(), m_Suffixes(), m_Signatures()
{
    m_Protostems = QMap<CStringSurrogate, int>();
}


//linguistic methods
void CLexicon::FindProtostems()
{
}

void CLexicon::CreateStemAffixPairs()
{
}
void CLexicon::AssignSuffixesToStems()
{
}
























