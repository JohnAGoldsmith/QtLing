#ifndef COMPOUND_H
#define COMPOUND_H

#include <QString>
#include <QList>
#include <QMap>
#include <QPair>

class CWord;
class CompoundComponent;
class CompoundWord;

class CompoundComponent
{
    QString                                     m_word;
    QMap<QString, QPair<int, CompoundWord*>>    m_connections;
    CWord*                                      m_cword_ptr;

public:
    CompoundComponent();
    CompoundComponent(QString& word, CWord* p_cword);
    void add_connection(CompoundWord* compword, int position);
    void add_connection(CompoundWord* compword);

};

class CompoundWord
{
    QString                                     m_word;
    QList<CompoundComponent*>                   m_composition;
    CWord*                                      m_cword_ptr;

public:
    CompoundWord();
    CompoundComponent* add_component(QString& part);
    QString get_word() { return m_word; }

};

#endif // COMPOUND_H
