        #ifndef CSUFFIX_H
    #define CSUFFIX_H

    #include <QString>
    #include <QChar>
    #include <QList>
    #include <QMap>
    //#include "generaldefinitions.h"

    class CSuffix
    {
    protected:
        QString m_key;
        int m_frequency;
        int m_count;
    public:
        CSuffix(QString ssWord);
        CSuffix(CSuffix&);
    public:
        //Accessors
        QString get_key() const {return m_key;}
        QString GetSuffix() const { return m_key;    }
        int     GetFrequency()    { return m_frequency; }
        void    SetFrequency(int frequency) { m_frequency = frequency; }
        int     get_count() const {return m_count;}
        int     increment_count() {m_count+= 1; return m_count;}
    };

    #endif // CSUFFIX_H
