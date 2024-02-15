#ifndef SPINECOLLECTION_H
#define SPINECOLLECTION_H

#include "spine.h"

class SpineCollection
{
    QMap<QString, Spine*> * m_;


public:
    SpineCollection();
};

#endif // SPINECOLLECTION_H
