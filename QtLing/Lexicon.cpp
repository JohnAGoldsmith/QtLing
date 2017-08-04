#include "Lexicon.h"

//#include "SignatureCollection.h"
//#include "StemCollection.h"
//#include "SuffixCollection.h"
//#include "WordCollection.h"
//#include "StringSurrogate.h"
#include "Word.h"
CLexicon::CLexicon() : m_Words(), m_Stems(), m_Suffixes(), m_Signatures()
{
    m_Protostems = QMap<CStringSurrogate, int>();
}


//linguistic methods
void CLexicon::FindProtostems()
{
   CStringSurrogate word, previous_word;
//   CWordCollection Words = GetWords();
   QMapIterator<CStringSurrogate,CWord*> iter(m_Words.GetMap());
   bool StartFlag = true;
   bool DifferenceFoundFlag = false;
   CStringSurrogate stem;//watch
   while (iter.hasNext())
   {
     iter.next();
     word = iter.key();
     if (StartFlag)
     {
         StartFlag = false;
         previous_word = word;
         continue;
     }
     DifferenceFoundFlag = false;
     int end = qMin(word.Display().length(), previous_word.Display().length());

     for (int i = 0; i < end; i++)
     {
         if (previous_word[i] != word[i])
         {
             stem = previous_word.Left(i);
             DifferenceFoundFlag = true;
             if (!m_Protostems.contains(stem))
             {
                 m_Protostems[stem] = 1;
                 //textEdit->appendPlainText(stem);
             }
             break;
         }
     }
     if (DifferenceFoundFlag == true)
     {
         previous_word = word;
         continue;
     }
     else
     {
        if (previous_word.Display().length() < word.Display().length())
        {
            m_Protostems[previous_word] = 1;
            //textEdit->appendPlainText(previous_word + "  "+ word + " method 2");
        }
     }
     previous_word = word;
   }
   return;
}

void CLexicon::CreateStemAffixPairs()
{
    QMapIterator<CStringSurrogate,CWord*> iter(m_Words.GetMap());
    CStringSurrogate ssStem, ssSuffix;
    int suffix_length;
    while (iter.hasNext() )
    {
        iter.next();
        CStringSurrogate word = iter.key();
        int word_length = word.Display().length();
        for (int letterno = 1; letterno < word_length; letterno++)
        {
            ssStem = word.Left(letterno);
            if (m_Protostems.contains(ssStem))
            {

                suffix_length = word_length - letterno;
                ssSuffix = word.Right(suffix_length);

                CStem* stem = new CStem(ssStem);
                CSuffix* suffix = new CSuffix(ssSuffix);
                iter.value()->AddParse(stem, suffix, NULL);

                //textEdit->appendPlainText(stem + " " + suffix);
                if (m_Words.GetMap().contains(ssStem))
                {
                    iter.value()->AddParse(stem, NULL, NULL);
                    //textEdit->appendPlainText(stem + " NULL");
                }
            }
        }
    }
}
void CLexicon::AssignSuffixesToStems()
{
    QListIterator<CWord*> iter(m_Words.GetList());

    CWord* this_word;
    CStem* stem;
    CSuffix* suffix;
    CSignature* signature;

    //question use of this whole while loop
/*    while (iter.hasNext() )
    {
        this_word = iter.next();
        //get stem, suffix, signature from this_word.

        stem = this_pair.first;
        suffix = this_pair.second.first;
        suffix->SetFrequency(1);
//        QMap<QString,int>& suffix_map = Stems[stem];
//        suffix_map[suffix] = 1;
    }*/

    while (iter.hasNext())
    {
        this_word = iter.next();
    }


    QListIterator<CStem*> stem_iter(m_Stems.GetList());

    while(stem_iter.hasNext())
    {
        stem = stem_iter.next();
        QMap<QString,int>& suffixes = Stems[stem];
        signature_string = suffixes.keys().join("=");
        Signatures[signature_string].append(stem);
    }

    QMutableMapIterator<QString,QStringList> signatures_iter(Signatures);
    while(signatures_iter.hasNext()){
        if (signatures_iter.next().value().length() < 2)
        {
            signatures_iter.remove();
        }
        else
        {
            QString signature= signatures_iter.key();
            textEdit->appendPlainText("");
            textEdit->appendPlainText(signature);
            textEdit->appendPlainText(signatures_iter.value().join(" ") );
        }
    }
}
























