#ifndef CSUFFIX_H
#define CSUFFIX_H

#include <QString>
#include <QChar>
#include <QList>
#include <QMap>
#include <QJsonObject>
#include "generaldefinitions.h"
class Affix
{
protected:
    QString m_key;
    int     m_word_count;
    int     m_sig_count;
    int     m_json_id;
    bool    m_suffix_flag;
public:
    Affix(QString word);
    Affix(Affix &);
public:
    //Accessors
    QString get_key() const                 {return m_key;}
    QString get_affix() const               { return m_key;    }
    void    set_count(int frequency)        { m_word_count = frequency; }
    int     get_sig_count() const           { return m_sig_count;}
    int     increment_sig_count()           { m_sig_count+= 1; return m_sig_count;}
    int     increment_count()               { m_word_count++; return m_word_count;}
    int     count()                         { return m_word_count;}

    void    write_json(QJsonObject& ref_json, eJsonType json_type = INDEXED) const;
    void    read_json(const QJsonObject& ref_json);
    int     get_json_id() const { return m_json_id; }
    void    set_json_id(const int id) { m_json_id = id; }
};
class CSuffix
{
protected:
    QString m_key;      // json tag: "suffix"
    int     m_word_count;    // json tag: "frequency"
    int     m_sig_count;        // json tag: "count"
    int     m_json_id;      // json tag: "id"
public:
    CSuffix(QString ssWord);
    CSuffix(CSuffix&);
    ~CSuffix(){};
public:
    //Accessors
    QString get_key() const              { return m_key;}
    int     count()                      { return m_word_count; }
    void    set_count(int frequency)     { m_word_count = frequency; }
    int     get_sig_count() const        { return m_sig_count;}
    int     get_word_count()                { return m_word_count;}
    int     increment_sig_count()        { m_sig_count+= 1; return m_sig_count;}
    int     increment_count(int n =1 )   { m_word_count += n; return m_word_count;}
    int     increment_word_count(int n =1 )   { m_word_count += n; return m_word_count;}
    void    write_json(QJsonObject& ref_json, eJsonType json_type = INDEXED) const;
    void    read_json(const QJsonObject& ref_json);
    int     get_json_id() const { return m_json_id; }
    void    set_json_id(const int id) { m_json_id = id; }
};

class CPrefix
{
protected:
    QString m_key;      // json tag: "suffix"
    int m_frequency;    // json tag: "frequency"
    int m_sig_count;        // json tag: "count"
    int m_word_count;
    int m_json_id;      // json tag: "id"
public:
    CPrefix(QString ssWord);
    CPrefix(CPrefix&);
public:
    //Accessors
    QString get_key() const                 { return m_key;}
    QString GetPrefix() const               { return m_key;    }
    int     GetFrequency()                  { return m_frequency; }
    void    SetFrequency(int frequency)     { m_frequency = frequency; }
    int     get_sig_count() const               { return m_sig_count;}
    int     get_word_count()                    {return m_word_count;}
    int     increment_sig_count()               { m_sig_count+= 1; return m_sig_count;}
    int     increment_word_count(int n)     { m_word_count += n; return m_word_count;}
    int     get_count()                     { return m_frequency; }

    void    write_json(QJsonObject& ref_json, eJsonType json_type = INDEXED) const;
    void    read_json(const QJsonObject& ref_json);
    int     get_json_id() const { return m_json_id; }
    void    set_json_id(const int id) { m_json_id = id; }
};




#endif // CSUFFIX_H
