#include "string_group.h"
#include "Typedefs.h"

enum string_type;

QString generalized_string::display(){
    QString result;
    QPair<QString, string_type> this_pair;
    foreach (this_pair, m_string){
        if (this_pair.second == regular){
            result += this_pair.first;
        } else{
            result += "( " + this_pair.first + " )";
        }
    }
}

//  This puts the result in the input StringPair.
void right_string_diff(QPair<QString, QString> fraction, QString& common_string )
{
    int i = 0;
    QString instring1 ( fraction.first );
    QString instring2 = fraction.second;
    QString numerator, denominator;
    while (i <  instring1.length() and i <  instring2.length() ){
            if  ( instring1[i]==instring2[i]){ i++; }
            else{ break; }
    }
    if (i < instring1.length() ){ numerator = instring1.mid(i,-1); }
    if (i < instring2.length()){ denominator = instring2.mid(i,-1); }
    fraction.first= numerator;
    fraction.second = denominator;
    common_string = instring1.left(i);
    return;
}
void left_string_diff(QPair<QString,QString> fraction, QString& common_string )
{
    QString instring1 = fraction.first;
    QString instring2 = fraction.second;
    QString numerator, denominator;
    int length1 = instring1.length();
    int length2 = instring2.length();
    int i1 = length1;
    int i2 = length2;
    while (i1 >= 0 and i2 >= 0) {
            if  ( instring1[i1]==instring2[i2]){ i1--; i2--; }
            else{ break; }
    }
    if (i1 < length1 ){ numerator = instring1.left(i1+1); }
    if (i2 < length2) { denominator = instring2.left(i2+1); }
    fraction.first= numerator;
    fraction.second = denominator;
    common_string = instring1.mid(i1+1,-1);
    return;
}
