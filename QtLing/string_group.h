#ifndef STRING_GROUP_H
#define STRING_GROUP_H
#include "Typedefs.h"

void right_string_diff(QPair<QString, QString> fraction, QString& common_string );
void  left_string_diff(QPair<QString, QString> fraction, QString& common_string );

enum string_type {regular, inverse};

class string_group
{
public:
    string_group();
};

class generalized_string{
    QList<QPair<QString, enum string_type>> m_string;

public:
    QString display();

};

class paradigm
{

public:
    QStringList get_paradigm();
private:
    QStringList paradigm;
};

#endif // STRING_GROUP_H
