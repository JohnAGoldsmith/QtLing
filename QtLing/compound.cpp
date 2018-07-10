#include "compound.h"

CompoundComponent::CompoundComponent(QString &word, CWord* p_cword):
    m_word(word), m_cword_ptr(p_cword)
{

}

CompoundComponent::add_connection(CompoundWord *p_compword, int position)
{
    m_connections[p_compword->get_word()] = QPair(position, p_compword);
}

CompoundWord::CompoundWord()
{

}
