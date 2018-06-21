#include <QDomDocument>
#include <QDomElement>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include "Lexicon.h"
#include "Word.h"
#include "WordCollection.h"
#include "mainwindow.h"
//#include "CorpusWord.h"
#include "goldstandard.h"
//#include "CorpusWordCollection.h"
//#include "TemplateCollection.h"
//#include "Typedefs.h"

/* from old lxa 4.7 code */
//typedef QMap<CParse*,int> ParseToInt;
//typedef QMap<QString,CParse*> StringToParse;
//typedef QMap<QString,ParseToInt*> StringToParseToInt;
//typedef QMap<QString,StringToInt*> StringToStringToInt;
// typedef QList<CStem*> CStemList;
// typedef QMap<QString,CStemList> StringToCStemList;

/* Things modified in other source code files:
 * - in QtLing.pro, added "xml" to the list after QT +=
 * - Modified Lexicon.h
 * - Modified mainwindow.h
 */

GoldStandard::GoldStandard()
{
    m_GS = new GoldStandard::GSMap();
}

GoldStandard::GoldStandard(QString& XML_file_name):
    m_true_positive_parses(nullptr),
    m_retrieved_parses(nullptr),
    m_XML_file_name(XML_file_name)
{
    m_GS = new GoldStandard::GSMap();
}

GoldStandard::GoldStandard(const GoldStandard &gs):
    m_total_recall(gs.m_total_recall),
    m_total_precision(gs.m_total_precision),
    m_XML_directory(gs.m_XML_directory),
    m_XML_file_name(gs.m_XML_file_name),
    m_GS(gs.clone_GSMap(gs.m_GS)) {}

GoldStandard& GoldStandard::operator=(const GoldStandard & gs)
{
    if (&gs != this) {
        delete_GSMap(m_GS);
        if (gs.m_GS)
            m_GS = gs.clone_GSMap(gs.m_GS);
        else
            m_GS = nullptr;
        m_total_recall = gs.m_total_recall;
        m_total_precision = gs.m_total_precision;
        m_XML_directory = gs.m_XML_directory;
        m_XML_file_name = gs.m_XML_file_name;
    }
    return *this;
}


GoldStandard::~GoldStandard()
{
    delete_GSMap(m_GS);
    delete_GSMap(m_true_positive_parses);
    delete_GSMap(m_retrieved_parses);
}

GoldStandard::GSMap* GoldStandard::clone_GSMap(GoldStandard::GSMap* map) const
{
    GSMap* newmap = new GSMap();
    for (GSMap::iterator gsIter = map->begin();
         gsIter != map->end(); gsIter++) {
        Parse_triple_map* newptm = new Parse_triple_map();
        Parse_triple_map currptm = *(gsIter.value());
        for (Parse_triple_map::iterator ptmIter = currptm.begin();
             ptmIter != currptm.end(); ptmIter++) {
            Parse_triple* newpt = new Parse_triple(ptmIter.key(), ptmIter.value()->p_suffix, QString());
            newptm->insert(ptmIter.key(), newpt);
        }
        newmap->insert(gsIter.key(),newptm);
    }
    return newmap;
}

void GoldStandard::delete_GSMap(GoldStandard::GSMap* map)
{
    if (map) {
        for (GSMap::iterator gsIter = map->begin();
             gsIter != map->end(); gsIter++) {
            Parse_triple_map ptm = *(gsIter.value());
            for (Parse_triple_map::iterator ptmIter = ptm.begin();
                 ptmIter != ptm.end(); ptmIter++) {
                delete ptmIter.value();
            }
            delete gsIter.value();
        }
        delete map;
    }
}

void GoldStandard::add_parse_triple(GoldStandard::GSMap *map, const QString &word, const QString &stem, const QString &affix)
{
    GSMap::iterator wordIter = map->find(word);
    if (wordIter == map->end()) {
        Parse_triple_map* this_ptm = new Parse_triple_map();
        Parse_triple* this_triple = new Parse_triple(stem, affix, QString());
        this_ptm->insert(stem, this_triple);
        map->insert(word, this_ptm);
    } else {
        Parse_triple* this_triple = new Parse_triple(stem, affix, QString());
        wordIter.value()->insert(stem, this_triple);
    }
}

bool GoldStandard::evaluate(CWordCollection *p_word_collection)
{
    if (p_word_collection->get_count() == 0) {
        QString errorMsg = "Did not find analysis Lexicon given by Linguistica.\n Please load file and analyse using Ctrl+S.";
        QMessageBox::information(nullptr, "Gold Standard: Error", errorMsg, QMessageBox::Ok);
        return false;
    }
    int true_positives = 0; // the total number of correct parses found by lxa
    int total_retrieved = 0; // the total number of parses found by lxa
    int total_correct = 0; // the total number of parses in gold standard
    int temp_overlap_word_count = 0;
    int temp_gs_word_count = 0;

    m_true_positive_parses = new GSMap();
    m_retrieved_parses = new GSMap();

    GSMap::const_iterator gs_iter;
    Parse_triple_map::const_iterator gs_ptm_iter, lxa_ptm_iter;
    const Parse_triple_map *p_lxa_ptm, *p_gs_ptm;
    CWord* p_word;

    for (gs_iter = m_GS->constBegin();
         gs_iter != m_GS->constEnd();
         gs_iter++) {
        temp_gs_word_count++;
        const QString& str_word = gs_iter.key();
        p_word = p_word_collection->get_word(str_word);

        if (p_word == NULL) {
            continue;
        }
        // only consider words that appear both in gold standard and lxa output result
        // ptm stands for parse_triple_map
        temp_overlap_word_count++;
        p_lxa_ptm = p_word->get_parse_triple_map();
        p_gs_ptm = gs_iter.value();

        // increment correct retrieved and total correct by iterating through ptm of gs
        for (gs_ptm_iter = p_gs_ptm->constBegin();
             gs_ptm_iter != p_gs_ptm->constEnd();
             gs_ptm_iter++) {
            total_correct++;
            const QString& stem = gs_ptm_iter.key();
            if (p_lxa_ptm->contains(stem)) {
                true_positives++;
                const QString& suffix = gs_ptm_iter.value()->p_suffix;
                // store parse into collection of true positive parses
                add_parse_triple(m_true_positive_parses, str_word, stem, suffix);
                // qDebug() << "[TRUE POSITIVE] " << str_word << ": " << stem << "-" << suffix;
            }
        }

        // increment total_retrived
        for (lxa_ptm_iter = p_lxa_ptm->constBegin();
             lxa_ptm_iter != p_lxa_ptm->constEnd();
             lxa_ptm_iter++) {
            const QString& stem = lxa_ptm_iter.key();
            if (stem != str_word) {// only count parses with non-null suffixes
                total_retrieved++;
                const QString& suffix = lxa_ptm_iter.value()->p_suffix;
                // store parse into collection of retrieved parses
                add_parse_triple(m_retrieved_parses, str_word, stem, suffix);
                // qDebug() << "[RETRIEVED] " << str_word << ": " << stem << "-" << suffix;
            }
        }
    }

    if (total_correct == 0) {
        QString errorMsg = "Did not find any parses in gold standard!";
        QMessageBox::information(nullptr, "Gold Standard: Error", errorMsg, QMessageBox::Ok);
        return false;
    }

    if (total_retrieved == 0) {
        QString errorMsg = "Did not find any parses in Linguistica's analysis\n"
                           "that are identical to a parse given by the Gold Standard";
        QMessageBox::information(nullptr, "Gold Standard: Error", errorMsg, QMessageBox::Ok);
        return false;
    }

    m_total_recall = (double) true_positives / (double) total_correct;
    m_total_precision = (double) true_positives / (double) total_retrieved;
    m_true_positive_count = true_positives;
    m_retrieved_count = total_retrieved;
    m_correct_count = total_correct;
    m_gs_word_count = temp_gs_word_count;
    m_overlap_word_count = temp_overlap_word_count;

    //qDebug() << "True positives: " << true_positives;
    //qDebug() << "Total # of correct parses in GS: " << total_correct;
    //qDebug() << "Total # retrieved: " << total_retrieved;
    return true;
}

void GoldStandard::read_XML()
{
// XML file must be first opend to use this function
/* old code
    QString	goldStdFileName = Q3FileDialog::getOpenFileName(m_projectDirectory,
                                                            "XML Files (*.xml)",
                                                            this,
                                                            "open file dialog",
                                                            "Choose a gold standard file to open" );
    --THIS FUNCTIONALITY IS MOVED INTO openXML() in mainwindow.cpp
*/
    // The below is the new version

    if( !m_XML_file_name.isEmpty() )
    {
        delete_GSMap(m_GS);
        m_GS = new GSMap();
        QFile goldStdFile( m_XML_file_name );
        if( goldStdFile.open( QIODevice::ReadOnly ) )
        {

            QDomDocument doc( "Alchemist" ) /* author_data, document_data */;

            QString errorMsg;
            int errorLine, errorColumn;
            if( !doc.setContent( &goldStdFile, &errorMsg, &errorLine, &errorColumn ) )
            {
//Maybe we should put this back in.
//				QMessageBox::warning( this, "Gold Standard : XML Error",
//				  QString( errorMsg + "\nLine: %1" + "Col: %2" ).arg( errorLine ).arg( errorColumn ), QMessageBox::Ok, NULL, NULL );

                return;
            }
            QDomElement alchemist_doc, morph, piece, string, wordnode
                        /* gloss, element, notes, morpheme, allomorph,
                        lmnt, feature, name, feature_id, instance_id */;
            //
            QDomNode node1, node2, node3, node4;
            /*
            QString feature_name;
            QDomText text;
            QDomNodeList nodes;
            CStem* pStem;
            */

            QString word, stem, affix;
            GSMap::iterator wordIter;

            bool skipWord = false;
            bool newWord = false;
            int lastEnd, start, length, pieceLength;

            alchemist_doc = doc.documentElement();

            if( alchemist_doc.tagName() != "alchemist-doc" ) {
                errorMsg = "The XML document \"" + alchemist_doc.tagName() + "\" is not an alchemist document.";
                /* old version
                QMessageBox::information( NULL, "Gold Standard : XML Error", errorMsg, "OK" );
                */
                QMessageBox::information(nullptr, "Gold Standard : XML Error", errorMsg, QMessageBox::Ok);
                return;
            }

            // Author data (optional)
            node1 = alchemist_doc.firstChild();
            if ( !node1.isNull() && node1.isElement() && node1.nodeName() == "author-data" ) {
                // Skip...
                node1 = node1.nextSibling();
            }
            // Document data (optional)
            if ( !node1.isNull() && node1.isElement() && node1.nodeName() == "document-data" ) {
                // Skip...
                node1 = node1.nextSibling();
            }
            // Feature list first of morphology description
            if ( node1.isNull() || !node1.isElement() || node1.nodeName() != "feature-list" ) {
                // TODO: add to error string
            } else {
                // Skip...
                node1 = node1.nextSibling();
            }
            // Morpheme list second
            if( node1.isNull() || !node1.isElement() || node1.nodeName() != "morpheme-list" ) {
                // TODO: add to error string
            } else {
                // Skip...
                node1 = node1.nextSibling();
            }

            // Word list last.. this is what we need!
            if( node1.isNull() || !node1.isElement() || node1.nodeName() != "word-list" ) {
                // TODO: add to error string
            } else {
                //qDebug() << 288 << "Reached word list";
                node2 = node1.firstChild();

                while( !node2.isNull() &&
                       node2.isElement() &&
                       node2.nodeName() == "word" )
                {
                    wordnode = node2.toElement();
                    node2 = node2.nextSibling();

                    // score attribute
                    if( !wordnode.hasAttribute( "score" ) ) {
                        // TODO: add to error string
                        continue;
                    } else {
                        if( wordnode.attribute( "score" ) == "Not Scored" ) continue;
                        else if( wordnode.attribute( "score" ) == "Certain" ); // we want to look at these words
                        else if( wordnode.attribute( "score" ) == "Somewhat Certain" ) continue;
                        else if( wordnode.attribute( "score" ) == "Uncertain" ) continue;
                    }
                    // string element
                    node3 = wordnode.firstChild();
                    if( !node3.isElement() || node3.nodeName() != "string" ) {
                        // TODO: add to error message
                        continue;
                    }
                    string = node3.toElement();

                    // Make new gold standard word
                    word = string.text();
                    length = word.length();
                    // pStem = new CStem( strStem ); //-- from old code
                    newWord = false;
                    wordIter = m_GS->find(word);

                    if (wordIter == m_GS->end()) {
                        wordIter = m_GS->insert(word, new Parse_triple_map());
                        newWord = true;
                    }

                    // gloss element
                    node3 = string.nextSibling();
                    if( node3.isElement() && node3.nodeName() == "gloss" ) {
                        // Skip...
                        node3 = node3.nextSibling();
                    }

                    // affix, root, and piece elements
                    skipWord = false;
                    lastEnd = 0;
                    while( !node3.isNull() &&
                           node3.isElement() &&
                           ( node3.nodeName() == "piece" ||
                             node3.nodeName() == "affix" ||
                             node3.nodeName() == "root" ) &&
                           !skipWord ) {
                        if( node3.nodeName() == "affix" || node3.nodeName() == "root" ) {
                            morph = node3.toElement();
                            // string element
                            node4 = morph.firstChild();
                            if( node4.isElement() && node4.nodeName() == "string" ) {
                                string = node4.toElement();
                                // no need to do anything with this string
                                node4 = string.nextSibling();
                            } else {
                                // TODO: add to error string
                            }
                            while( !node4.isNull() && node4.isElement() && node4.nodeName() == "piece" ) {
                                piece = node4.toElement();
                                if( !piece.hasAttribute( "start" ) ||
                                    !piece.hasAttribute( "length" ) ) {
                                    // TODO: add to error string
                                    skipWord = true;
                                    //delete pStem; pStem = NULL; //-- from old code
                                    if (newWord) {
                                        delete wordIter.value();
                                        m_GS->erase(wordIter);
                                    }
                                    break;
                                }
                                if ( morph.tagName() == "affix" || morph.tagName() == "root" ) {
                                    start = piece.attribute( "start" ).toInt();
                                    pieceLength = piece.attribute("length").toInt();

                                    if (start != 0) {
                                        stem = word.left(start);
                                        affix = word.right(length - start);
                                        add_parse_triple(m_GS, word, stem, affix);
                                        if ( start < lastEnd ) {
                                            stem = word.left(lastEnd);
                                            affix = word.right(length - lastEnd);
                                            add_parse_triple(m_GS, word, stem, affix);
                                        }
                                    }
                                    lastEnd = start + pieceLength;
                                    //pStem->CutRightBeforeHere( start ); //-- from old code
                                }
                                node4 = node4.nextSibling();
                            }
                        } else {
                            // Word has unassigned pieces, skip...
                            skipWord = true;
                            if (newWord) {
                                delete wordIter.value();
                                m_GS->erase(wordIter);
                            }
                        }
                        node3 = node3.nextSibling();
                    }
                    if( skipWord ) continue;

                    /* old code
                    // Add word to gold standard words
                    goldStdWordsIt = goldStdWords.find( word );
                    if( goldStdWordsIt == goldStdWords.end() )
                    {
                        // New word...
                        goldStdWordsIt = goldStdWords.insert( word, stemList );
                                                //goldStdWordsIt.data().setAutoDelete( true );    @@@ fix this, make sure there are no memory leaks created here.
                    }
                    goldStdWordsIt.value().append( pStem );
                    */


                    // notes element
                    if( !node3.isNull() && node3.isElement() && node3.nodeName() == "notes" ) {
                        // Skip...
                        node3 = node3.nextSibling();
                    }
                    if ( !node3.isNull() ) {
                        // TODO: add to error string
                    }
                }
            }
        } else {
            QMessageBox::information( NULL, "Attention", "Unable to open " + m_XML_file_name + " .", "OK" );
            return;
        }
        //qDebug() << 453 << "Goldstandard.cpp: gold standard parsed";
        goldStdFile.close();
    } else {
        qDebug() << 458 << QString("GoldStandar::m_parse_XML: XML file not set");
        return;
    }

/* BEGINNING OF YU HU's CODE
// Yu Hu's code
//----------------------------------------------------------------------
    QString								theWord;
    CStem*								theCStem;
    CParse*								theParse;
    StringToParse*						TempSedCuts;
    StringToParse::Iterator				StringToParseIt;
    StringToParse						SedCuts;
    CCorpusWordCollection*				TempSFCut;
    StringToParse						SFCuts;
    CCorpusWord*						theCorpusWord;

    // Get the Lingustica analyses result SF or PF
    if ( !m_lexicon) return;
    TempSFCut = m_lexicon->GetWords();

    TempSFCut->Sort( KEY );

    for( int i = 0; i < TempSFCut->GetCount(); i++ )
    {
        theCorpusWord = TempSFCut->GetAt(i);
        theWord = theCorpusWord->Display();

        SFCuts.insert(theWord, theCorpusWord);

    }

    // Get SED analyses
    if ( m_Words_Templates != NULL)
    {
        TempSedCuts = m_Words_Templates ->GetParsedResult();
        for ( StringToParseIt = TempSedCuts ->begin(); StringToParseIt != TempSedCuts ->end(); 	StringToParseIt++)
        {
            theWord = StringToParseIt.key();
            theParse = StringToParseIt.data();
            theCStem = new CStem(*theParse);

            SedCuts.insert(theWord, theCStem);
        }


    }

    // Goldstandard comparison output
    double TotalPrecision;
    double TotalRecall;
    double AveragePrecision;
    double AverageRecall;

    double Ftot=0.0;
    double Fav=0.0;
    QString					outs;

    ////////////// compute precision recall		SF

    GetMorphPrecisionRecallByWord( goldStdWords, SFCuts, TotalPrecision, TotalRecall,AveragePrecision,AverageRecall);
    Ftot=2*TotalPrecision*TotalRecall/(TotalPrecision+TotalRecall);
    Fav=2*AveragePrecision*AverageRecall/(AveragePrecision+AverageRecall);
    // print out precision recall
    outs = QString("Total precision MiniLexicon = %1, total recall= %2 Ftot=%3 ").arg(TotalPrecision).arg(TotalRecall).arg(Ftot);

    QMessageBox::information ( NULL, "SF Morpheme Precision/Recall By Word", outs );

    GetMorphPrecisionRecall( goldStdWords, SFCuts, TotalPrecision, TotalRecall,AveragePrecision,AverageRecall);
    Ftot=2*TotalPrecision*TotalRecall/(TotalPrecision+TotalRecall);
    Fav=2*AveragePrecision*AverageRecall/(AveragePrecision+AverageRecall);
    // print out precision recall
    outs = QString("Total precision MiniLexicon = %1, total recall= %2 Ftot=%3 ").arg(TotalPrecision).arg(TotalRecall).arg(Ftot);

    QMessageBox::information ( NULL, "SF Morpheme Precision/Recall", outs );

    GetCutPrecisionRecall( goldStdWords, SFCuts, TotalPrecision, TotalRecall,AveragePrecision,AverageRecall);
    Ftot=2*TotalPrecision*TotalRecall/(TotalPrecision+TotalRecall);
    Fav=2*AveragePrecision*AverageRecall/(AveragePrecision+AverageRecall);
    // print out precision recall
    outs = QString("Total precision MiniLexicon = %1, total recall= %2 Ftot=%3 ").arg(TotalPrecision).arg(TotalRecall).arg(Ftot);

    QMessageBox::information ( NULL, "SF Cut Precision/Recall", outs );





    ///////////////////////////////// SED
    if ( m_Words_Templates != NULL)
    {

        GetMorphPrecisionRecallByWord( goldStdWords, SedCuts, TotalPrecision, TotalRecall,AveragePrecision,AverageRecall);

        Ftot=2*TotalPrecision*TotalRecall/(TotalPrecision+TotalRecall);
        Fav=2*AveragePrecision*AverageRecall/(AveragePrecision+AverageRecall);

        outs = QString("Total precision SED= %1, total recall= %2 Ftot=%3").arg(TotalPrecision).arg(TotalRecall).arg(Ftot);

        QMessageBox::information ( NULL, "SED Morpheme Precision/Recall By Word", outs );

        GetMorphPrecisionRecall( goldStdWords, SedCuts, TotalPrecision, TotalRecall,AveragePrecision,AverageRecall);

        Ftot=2*TotalPrecision*TotalRecall/(TotalPrecision+TotalRecall);
        Fav=2*AveragePrecision*AverageRecall/(AveragePrecision+AverageRecall);

        outs = QString("Total precision SED= %1, total recall= %2 Ftot=%3").arg(TotalPrecision).arg(TotalRecall).arg(Ftot);

        QMessageBox::information ( NULL, "SED Morpheme Precision/Recall By Word", outs );


        GetCutPrecisionRecall( goldStdWords, SedCuts, TotalPrecision, TotalRecall,AveragePrecision,AverageRecall);

        Ftot=2*TotalPrecision*TotalRecall/(TotalPrecision+TotalRecall);
        Fav=2*AveragePrecision*AverageRecall/(AveragePrecision+AverageRecall);

        outs = QString("Total precision SED= %1, total recall= %2 Ftot=%3").arg(TotalPrecision).arg(TotalRecall).arg(Ftot);

        QMessageBox::information ( NULL, "SED Cut PrecisioGetPiecen/Recall", outs );
    } */ // END OF YU HU's CODE
}
/*
void LinguisticaMainWindow::GetMorphPrecisionRecallByWord( StringToCStemList& goldStdWords,
                                                           StringToParse& lxaWords,
                                                           double& totalPrecision,
                                                           double& totalRecall,
                                                           double& averagePrecision,
                                                           double& averageRecall )
{
    CParse*	pGoldStdStem;
    int*	goldStdStemCuts;
        int	goldStdStemCutsPos;

    CParse*	pLxaStem;
    int*	lxaStemCuts;
        int	lxaStemCutsPos;

        int     totalNumLxaWordsCompared = 0;
        int     totalNumGSWordsCompared = 0;

        int     totalNumLxaMorphemes = 0;
        int     totalNumGSMorphemes = 0;
        int     totalNumCorrectMorphemes = 0;
        int     totalNumFoundMorphemes = 0;		// For precision, see generalization notes below in function

                averagePrecision = 0.0;
                averageRecall = 0.0;

    QString strWord;

    StringToCStemList::Iterator goldStdIt;
    for( goldStdIt = goldStdWords.begin(); goldStdIt != goldStdWords.end(); goldStdIt++ )
    {
        strWord = goldStdIt.key();

        // We only look through words that exist in both spaces
        if( lxaWords.find( strWord ) == lxaWords.end() ) continue; //GO TO NEXT WORD
        pLxaStem = lxaWords.find( strWord ).data(); // pLxaStem is a pointer to a Stem object

        lxaStemCuts = pLxaStem->GetPieces(); // list of integers that represent cut positions

        int numLxaMorphemes,
            numGSMorphemes,
            numCorrectMorphemes;

        lxaStemCutsPos = goldStdStemCutsPos = 0; // indeces for going through the index arrays
        totalNumLxaWordsCompared++;

        // There may be duplicates in gold standard, we should consider all
        // pGoldStdStem is a pointer to a Stem object
        for( pGoldStdStem = goldStdIt.data().first(); pGoldStdStem; pGoldStdStem = goldStdIt.data().next() )
        {
            numLxaMorphemes = 0;
            numGSMorphemes = 0;
            numCorrectMorphemes = 0;

            totalNumGSWordsCompared++; // duplicates are counted

            goldStdStemCuts = pGoldStdStem->GetPieces(); // list of integers that represent cut positions

            // The word strings should match now...
            Q_ASSERT( pLxaStem->Display() == pGoldStdStem->Display() );

            // Therefore we can look at the cuts to compare the morphemes
            lxaStemCutsPos = goldStdStemCutsPos = 0; // indeces for going through the two lists
            while( lxaStemCutsPos < pLxaStem->Size() && goldStdStemCutsPos < pGoldStdStem->Size() )
            { //Parse::Size() returns the length of m_Pieces integer vectors
                if( lxaStemCuts[ lxaStemCutsPos ] == goldStdStemCuts[ goldStdStemCutsPos ] &&
                    lxaStemCuts[ lxaStemCutsPos + 1 ] == goldStdStemCuts[ goldStdStemCutsPos + 1 ] )
                {
                    // Morphemes match, increment everything
                    numLxaMorphemes++;
                    numGSMorphemes++;
                    numCorrectMorphemes++;

                    // Move both positions
                    lxaStemCutsPos++;
                    goldStdStemCutsPos++;
                }
                else if( lxaStemCuts[ lxaStemCutsPos ] == goldStdStemCuts[ goldStdStemCutsPos ] )
                {
                    if( lxaStemCuts[ lxaStemCutsPos + 1 ] < goldStdStemCuts[ goldStdStemCutsPos + 1 ] )
                    {
                        numLxaMorphemes++;
                        lxaStemCutsPos++;
                    }
                    else
                    {
                        numGSMorphemes++;
                        goldStdStemCutsPos++;
                    }
                }
                else
                {
                    if( lxaStemCuts[ lxaStemCutsPos ] < goldStdStemCuts[ goldStdStemCutsPos ] )
                    {
                        numLxaMorphemes++;
                        lxaStemCutsPos++;
                    }
                    else
                    {
                        numGSMorphemes++;
                        goldStdStemCutsPos++;
                    }
                }
            } // end of while loop

            // Handle remaining morphemes in either group
            while( lxaStemCutsPos < pLxaStem->Size() )
            {
                numLxaMorphemes++;
                lxaStemCutsPos++;
            }
            while( goldStdStemCutsPos < pGoldStdStem->Size() )
            {
                numGSMorphemes++;
                goldStdStemCutsPos++;
            }

            averageRecall += ( (double) numCorrectMorphemes / (double) numGSMorphemes );

            totalNumGSMorphemes += numGSMorphemes;
            totalNumCorrectMorphemes += numCorrectMorphemes;
        } // END OF FOR LOOP ANALYSING EACH STEM IN CStemList OF A WORD IN GS


        // Precision generalization: if Lxa finds a morpheme M in a word W, it
        // gets credit for it if M appears in any of the analyses spelled W.
        // From John's e-mail to Colin, July 27, 2006

        numLxaMorphemes = 0;
        int numFoundMorphemes = 0;
        int piece = 1; // index for iterating through integer array
        while( piece <= pLxaStem->Size() )
        {
            numLxaMorphemes++;
            //Iterating through the list of stems for one word found in GS
            for( pGoldStdStem = goldStdIt.data().first(); pGoldStdStem; pGoldStdStem = goldStdIt.data().next() )
            { //pGoldStdStem is a pointer to a Stem object
                // check if each morpheme in the Lxa parse of a word is in pGoldStdStem
                if( pGoldStdStem->Contains( pLxaStem->GetPiece( piece ) ) )
                {
                    numFoundMorphemes++;
                    break;
                }
            }

            piece++;
        }

        totalNumLxaMorphemes += numLxaMorphemes;
        totalNumFoundMorphemes += numFoundMorphemes;

        averagePrecision += ( (double) numFoundMorphemes / (double) numLxaMorphemes );
    } // END OF FOR LOOP; FOR LOOP ITERATES OVER EVERY WORD IN GOLD STANDARD

    averagePrecision /= (double) totalNumLxaWordsCompared;
    averageRecall /= (double) totalNumGSWordsCompared;

    totalPrecision = (double) totalNumFoundMorphemes / (double) totalNumLxaMorphemes;
    totalRecall = (double) totalNumCorrectMorphemes / (double) totalNumGSMorphemes;
}


void LinguisticaMainWindow::GetCutPrecisionRecall( StringToCStemList& goldStdWords,
                                                   StringToParse& lxaWords,
                                                   double& totalPrecision,
                                                   double& totalRecall,
                                                   double& averagePrecision,
                                                   double& averageRecall )
{
    CParse*	pGoldStdStem;
    int*	goldStdStemCuts;
    int		goldStdStemCutsPos;

    CParse*	pLxaStem;
    int*	lxaStemCuts;
    int		lxaStemCutsPos;

    int totalNumLxaWordsCompared = 0;
    int totalNumGSWordsCompared = 0;

    int totalNumLxaCuts = 0;
    int totalNumGSCuts = 0;
    int totalNumCorrectCuts = 0;
    int totalNumFoundCuts = 0;		// Need different number for precision (using totalNumCorrectCuts for recall)
    int totalNumOnePieceWords = 0;	// One piece Lxa words are undefined for precision, we need to subtract when
                                    // totalling

    averagePrecision = 0.0;
    averageRecall = 0.0;

    QString strWord;

    StringToCStemList::Iterator goldStdIt;
    for( goldStdIt = goldStdWords.begin(); goldStdIt != goldStdWords.end(); goldStdIt++ )
    {
        strWord = goldStdIt.key();

        // We only look through words that exist in both spaces
        if( lxaWords.find( strWord ) == lxaWords.end() ) continue;
        pLxaStem = lxaWords.find( strWord ).data();

        lxaStemCuts = pLxaStem->GetPieces();

        int numLxaCuts = 0,
            numGSCuts = 0,
            numCorrectCuts = 0,
            numFoundCuts = 0;

        totalNumLxaWordsCompared++;

        Q3ValueList<int> unionOfGSCuts;

        // There may be duplicates in gold standard, we need the union of all their cuts
        for( pGoldStdStem = goldStdIt.data().first(); pGoldStdStem; pGoldStdStem = goldStdIt.data().next() )
        {
            totalNumGSWordsCompared++;

            goldStdStemCuts = pGoldStdStem->GetPieces();

            // The word strings should match here.
            Q_ASSERT( pLxaStem->Display() == pGoldStdStem->Display() );

            goldStdStemCutsPos = 0;
            while( goldStdStemCutsPos < pGoldStdStem->Size() )
            {
                if( unionOfGSCuts.find( goldStdStemCuts[ goldStdStemCutsPos ] ) == unionOfGSCuts.end() )
                {
                    unionOfGSCuts.append( goldStdStemCuts[ goldStdStemCutsPos ] );
                }
                goldStdStemCutsPos++;
            }
        }

        lxaStemCutsPos = 0;
        while( lxaStemCutsPos < pLxaStem->Size() )
        {
            numLxaCuts++;

            if( unionOfGSCuts.find( lxaStemCuts[ lxaStemCutsPos ] ) != unionOfGSCuts.end() )
            {
                numCorrectCuts++;
                numFoundCuts++;
            }
            lxaStemCutsPos++;
        }

        numGSCuts = unionOfGSCuts.count();

        averageRecall += ( (double) numCorrectCuts / (double) numGSCuts );

        totalNumGSCuts += numGSCuts;
        totalNumCorrectCuts += numCorrectCuts;

        if( pLxaStem->Size() < 2 )
        {
            totalNumOnePieceWords++;

            Q_ASSERT( numFoundCuts == 1 && numLxaCuts == 1 );
            numFoundCuts--;
            numLxaCuts--;

            if( numFoundCuts < 0 ) numFoundCuts = 0;
            if( numLxaCuts < 0 ) numLxaCuts = 0;
        }

        totalNumLxaCuts += numLxaCuts;
        totalNumFoundCuts += numFoundCuts;

        if( numLxaCuts > 0 ) averagePrecision += ( (double) numCorrectCuts / (double) numLxaCuts );
    }

    averagePrecision /= (double) ( totalNumLxaWordsCompared - totalNumOnePieceWords );
    averageRecall /= (double) totalNumGSWordsCompared;

    totalPrecision = (double) totalNumFoundCuts / (double) totalNumLxaCuts;
    totalRecall = (double) totalNumCorrectCuts / (double) totalNumGSCuts;
}

*/

/*
void LinguisticaMainWindow::GetMorphPrecisionRecall( StringToCStemList& goldStdWords,
                                                     StringToParse& lxaWords,
                                                     double& totalPrecision,
                                                     double& totalRecall,
                                                     double& averagePrecision,
                                                     double& averageRecall )
{
    QStringList unionOfGoldStdMorphs,
                unionOfLxaMorphs;

    QString strWord, strPiece;

    CParse* pLxaStem, * pGoldStdStem;

    int i,
        totalNumLxaWordsCompared = 0,
        totalNumGSWordsCompared = 0,
        totalNumLxaMorphemes = 0,
        totalNumGSMorphemes = 0,
        totalNumCorrectMorphemes = 0;

    StringToCStemList::Iterator goldStdIt;
    for( goldStdIt = goldStdWords.begin(); goldStdIt != goldStdWords.end(); goldStdIt++ )
    {
        strWord = goldStdIt.key();

        // We only look through words that exist in both spaces
        if( lxaWords.find( strWord ) == lxaWords.end() ) continue;
        pLxaStem = lxaWords.find( strWord ).data();

        totalNumLxaWordsCompared++;

        for( i = 1; i <= pLxaStem->Size(); i++ )
        {
            strPiece = pLxaStem->GetPiece(i).Display();
            if( unionOfLxaMorphs.findIndex( strPiece ) == -1 )
            {
                unionOfLxaMorphs.append( strPiece );
            }
        }

        // There may be duplicates in gGetPieceold standard, we need the union of all their morphemes
        for( pGoldStdStem = goldStdIt.data().first(); pGoldStdStem; pGoldStdStem = goldStdIt.data().next() )
        {
            totalNumGSWordsCompared++;

            for( i = 1; i <= pGoldStdStem->Size(); i++ )
            {
                strPiece = pGoldStdStem->GetPiece(i).Display();
                if( unionOfGoldStdMorphs.findIndex( strPiece ) == -1 )
                {
                    unionOfGoldStdMorphs.append( strPiece );
                }
            }
        }
    }

    unionOfLxaMorphs.sort();
    unionOfGoldStdMorphs.sort();

    QStringList::Iterator lxaMorphsIt = unionOfLxaMorphs.begin(),
                          goldStdMorphsIt = unionOfGoldStdMorphs.begin();

    while( lxaMorphsIt != unionOfLxaMorphs.end() &&
           goldStdMorphsIt != unionOfGoldStdMorphs.end() )
    {
        if( *goldStdMorphsIt == *lxaMorphsIt )
        {
            totalNumCorrectMorphemes++;
            totalNumLxaMorphemes++;
            totalNumGSMorphemes++;

            ++goldStdMorphsIt;
            ++lxaMorphsIt;
        }
        else if( *goldStdMorphsIt > *lxaMorphsIt )
        {
            totalNumLxaMorphemes++;

            ++lxaMorphsIt;
        }
        else // *goldStdMorphsIt < *lxaMorphsIt
        {
            totalNumGSMorphemes++;

            ++goldStdMorphsIt;
        }
    }

    totalPrecision = (double) totalNumCorrectMorphemes / (double) totalNumLxaMorphemes;
    totalRecall = (double) totalNumCorrectMorphemes / (double) totalNumGSMorphemes;

    averagePrecision = totalPrecision;
    averageRecall = totalRecall;
}

*/


