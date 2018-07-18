#ifndef CSUFFIX_H
#define CSUFFIX_H

#include <QString>
#include <QChar>
#include <QList>
#include <QMap>
#include <QJsonObject>


class CSuffix
{
protected:
    QString m_key;
    int m_frequency;
    int m_count;
    int m_json_id;
public:
    CSuffix(QString ssWord);
    CSuffix(CSuffix&);
public:
    //Accessors
    QString get_key() const                 {return m_key;}
    QString GetSuffix() const               { return m_key;    }
    int     GetFrequency()                  { return m_frequency; }
    void    SetFrequency(int frequency)     { m_frequency = frequency; }
    int     get_count() const               { return m_count;}
    int     increment_count()               { m_count+= 1; return m_count;}

    void    to_json(QJsonObject& ref_json);
    void    set_json_id(int id) { m_json_id = id; }
};

class CPrefix
{
protected:
    QString m_key;
    int m_frequency;
    int m_count;
    int m_json_id;
public:
    CPrefix(QString ssWord);
    CPrefix(CPrefix&);
public:
    //Accessors
    QString get_key() const                 { return m_key;}
    QString GetPrefix() const               { return m_key;    }
    int     GetFrequency()                  { return m_frequency; }
    void    SetFrequency(int frequency)     { m_frequency = frequency; }
    int     get_count() const               { return m_count;}
    int     increment_count()               { m_count+= 1; return m_count;}

    void    to_json(QJsonObject& ref_json);
    void    set_json_id(int id) { m_json_id = id; }
};




#endif // CSUFFIX_H
