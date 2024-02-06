#ifndef SPINE_H
#define SPINE_H
#include <QList>
#include <QPair>


class Spine
{
    QList<QPair<QString,QString>*> m_pairs; // signature string and string difference

public:
    Spine();
    ~Spine();
    void add(QString  , QString  );
    QPair<QString,QString>* at(int n) {return m_pairs[n];}
    int length() {return m_pairs.length();}
};

#endif // SPINE_H
