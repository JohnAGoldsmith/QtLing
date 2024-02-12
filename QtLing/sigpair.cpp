#include "sigpair.h"

QString sig_pair::display(){
    QString response;
    response += label();
    return response;
}
