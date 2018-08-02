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
#include "evaluation.h"
//#include "CorpusWordCollection.h"
//#include "TemplateCollection.h"
//#include "Typedefs.h"

/* Things modified in other source code files:
 * - in QtLing.pro, added "xml" to the list after QT +=
 * - Modified Lexicon.h
 * - Modified mainwindow.h
 */

GoldStandard::GoldStandard() { }

GoldStandard::GoldStandard(QString& XML_file_name):
    m_XML_file_name(XML_file_name) { }


GoldStandard& GoldStandard::operator=(const GoldStandard & gs)
{
    if (&gs != this) {
        m_results = gs.m_results;
        m_gs_parses = gs.m_gs_parses;
        m_retrieved_parses = gs.m_retrieved_parses;
        m_true_positive_parses = gs.m_true_positive_parses;
        m_XML_directory = gs.m_XML_directory;
        m_XML_file_name = gs.m_XML_file_name;
    }
    return *this;
}


GoldStandard::~GoldStandard()
{ }

/*!
 * \brief Evaluates Linguistica's output using a Gold Standard;
 * stores results and relevant parses in the GoldStandard class.
 * \param Object containing the parses generated by Linguistica.
 * \return TRUE if evaluation succeeded, FALSE if not.
 *
 * After running this function, relevant output results are stored in the following members:
 * * `m_results` stores relevant numerical results, including precision and recall.\n
 *   \see EvaluationResults.
 * * `m_true_positive_parses` stores parses that are "true positives", i.e.\n
 *   parses that satisfy the following conditions:
 *     1. belongs to a word existing both in Linguistica's analysis and in the Gold Standard
 *     2. this parse is found both in `p_word_collection` and in the Gold Standard.
 * * `m_retrieved_parses` stores any parses that are found in `p_word_collection` and satisfy coondition 1. above.
 */
bool GoldStandard::evaluate(CWordCollection* p_word_collection)
{
    return evaluate(p_word_collection,
                   m_results,
                   &m_true_positive_parses,
                   &m_retrieved_parses);
}

/*!
 * \brief Evaluates parses found in a CWordCollection object
 * \param Object containing collection of parses to be evaluated.
 * \param Location where numerical results (including precision and recall) are stored.
 * \param NULL by default, if not NULL, location where "true positive" parses are stored.
 * \param NULL by default, if not NULL, location where "retrieved" parses are stored.
 * \return TRUE if evaluation succeeded, FALSE if not.
 * \see GoldStandard::evaluate(CWordCollection* p_word_collection)
 */
bool GoldStandard::evaluate(CWordCollection* p_word_collection,
                            EvaluationResults& results,
                            ParseMapHandler* p_true_positive_parses,
                            ParseMapHandler* p_retrieved_parses)
{
    if (p_word_collection->get_count() == 0) {
        QString errorMsg = "Did not find analysis Lexicon given by Linguistica.\n Please load file and analyse using Ctrl+S.";
        QMessageBox::information(nullptr, "Gold Standard: Error", errorMsg, QMessageBox::Ok);
        return false;
    }
    results.clear();
    int true_positives = 0;     // number of correct parses found by lxa
    int total_retrieved = 0;    // number of parses retrieved by lxa
    int total_correct = 0;      // number of parses in gold standard
    int temp_overlap_word_count = 0; // number of words found in both gold standard and lxa
    int temp_gs_word_count = 0; // number of words foudn in gold standard

    ParseMap::const_iterator gs_pm_iter;
    Parse_triple_map::const_iterator gs_ptm_iter, lxa_ptm_iter;
    const Parse_triple_map *p_lxa_ptm, *p_gs_ptm;
    CWord *p_word;

    for (gs_pm_iter = m_gs_parses->constBegin();
         gs_pm_iter != m_gs_parses->constEnd();
         gs_pm_iter++) {
        temp_gs_word_count++;
        const QString& str_word = gs_pm_iter.key();
        p_word = p_word_collection->get_word(str_word);

        if (p_word == NULL) {
            continue;
        }
        // only consider words that appear both in gold standard and lxa output result
        // ptm stands for parse_triple_map
        temp_overlap_word_count++;
        p_lxa_ptm = p_word->get_parse_triple_map();
        p_gs_ptm = gs_pm_iter.value();

        // increment correct retrieved and total correct by iterating through ptm of gs
        for (gs_ptm_iter = p_gs_ptm->constBegin();
             gs_ptm_iter != p_gs_ptm->constEnd();
             gs_ptm_iter++) {
            total_correct++;
            const QString& stem = gs_ptm_iter.key();
            if (p_lxa_ptm->contains(stem)) {
                true_positives++;
                if (p_true_positive_parses != NULL) {
                    const QString& suffix = gs_ptm_iter.value()-> m_suffix;
                    // store parse into collection of true positive parses
                    p_true_positive_parses->add_parse_triple(str_word, stem, suffix); // **** NEED TO CHANGE **** //
                    // qDebug() << "[TRUE POSITIVE] " << str_word << ": " << stem << "-" << suffix;
                }
            }
        }

        // increment total_retrived
        for (lxa_ptm_iter = p_lxa_ptm->constBegin();
             lxa_ptm_iter != p_lxa_ptm->constEnd();
             lxa_ptm_iter++) {
            const QString& stem = lxa_ptm_iter.key();
            if (stem != str_word) {// only count parses with non-null suffixes
                total_retrieved++;
                if (p_retrieved_parses != NULL) {
                    const QString& suffix = lxa_ptm_iter.value()->m_suffix;
                    // store parse into collection of retrieved parses
                    p_retrieved_parses->add_parse_triple(str_word, stem, suffix); // **** NEED TO CHANGE **** //
                    // qDebug() << "[RETRIEVED] " << str_word << ": " << stem << "-" << suffix;
                }
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

    results.set_true_positive_count(true_positives);
    results.set_retrieved_parse_count(total_retrieved);
    results.set_retrieved_word_count(p_word_collection->get_count());
    results.set_gs_parse_count(total_correct);
    results.set_gs_word_count(temp_gs_word_count);
    results.set_overlap_word_count(temp_overlap_word_count);

    results.calculate_precision_recall();

    //qDebug() << "True positives: " << true_positives;
    //qDebug() << "Total # of correct parses in GS: " << total_correct;
    //qDebug() << "Total # retrieved: " << total_retrieved;
    return true;
}


/*!
 * \brief Evaluates parses produced by Morfessor using a Gold Standard;
 * stores results in the given EvalParses object, which stores the parses produced by Morfessor.
 * \param Object containing the parses generated by Morfessor.
 * \return TRUE if evaluation succeeded, FALSE if not.
 *
 * After running this function, relevant numerical results, including precision and recall,
 * are stored in the `m_results` member of the EvalParses object.
 */
bool GoldStandard::evaluate(EvalParses* p_eval_parses)
{
    p_eval_parses->m_evaluated = true;
    return evaluate(p_eval_parses->m_parses, p_eval_parses->m_results);
}

/*!
 * \brief Evaluates parses found in a `ParseMapHandler` object
 * \param Object containing collection of parses to be evaluated.
 * \param Location where numerical results (including precision and recall) are stored.
 * \param NULL by default, if not NULL, location where "true positive" parses are stored.
 * \param NULL by default, if not NULL, location where "retrieved" parses are stored.
 * \return TRUE if evaluation succeeded, FALSE if not.
 */
bool GoldStandard::evaluate(ParseMapHandler& eval_parses,
                            EvaluationResults& results,
                            ParseMapHandler* p_true_positive_parses,
                            ParseMapHandler* p_retrieved_parses)
{
    if (eval_parses->size() == 0) {
        QString errorMsg = "Parses are not loaded";
        QMessageBox::information(nullptr, "Gold Standard: Error", errorMsg, QMessageBox::Ok);
        return false;
    }
    int true_positives = 0; // the total number of correct parses found by lxa
    int total_retrieved = 0; // the total number of parses found by lxa
    int total_correct = 0; // the total number of parses in gold standard
    int temp_overlap_word_count = 0;
    int temp_gs_word_count = 0;

    ParseMap::const_iterator gs_pm_iter;
    ParseMap::const_iterator eval_pm_iter;
    Parse_triple_map::const_iterator gs_ptm_iter, eval_ptm_iter;
    const Parse_triple_map *p_eval_ptm, *p_gs_ptm;

    for (gs_pm_iter = m_gs_parses->constBegin();
         gs_pm_iter != m_gs_parses->constEnd();
         gs_pm_iter++) {
        temp_gs_word_count++;
        const QString& str_word = gs_pm_iter.key();
        eval_pm_iter = eval_parses->find(str_word);

        if (eval_pm_iter == eval_parses->constEnd()) {
            continue;
        }

        // only consider words that appear both in gold standard and lxa output result
        // ptm stands for parse_triple_map
        temp_overlap_word_count++;
        p_eval_ptm = eval_pm_iter.value();
        p_gs_ptm = gs_pm_iter.value();

        // increment correct retrieved and total correct by iterating through ptm of gs
        for (gs_ptm_iter = p_gs_ptm->constBegin();
             gs_ptm_iter != p_gs_ptm->constEnd();
             gs_ptm_iter++) {
            total_correct++;
            const QString& stem = gs_ptm_iter.key();
            if (p_eval_ptm->contains(stem)) {
                true_positives++;
                if (p_true_positive_parses != NULL) {
                    const QString& suffix = gs_ptm_iter.value()->m_suffix;
                    // store parse into collection of true positive parses
                    p_true_positive_parses->add_parse_triple(str_word, stem, suffix);
                    // qDebug() << "[TRUE POSITIVE] " << str_word << ": " << stem << "-" << suffix;
                }
            }
        }

        // increment total_retrived
        for (eval_ptm_iter = p_eval_ptm->constBegin();
             eval_ptm_iter != p_eval_ptm->constEnd();
             eval_ptm_iter++) {
            const QString& stem = eval_ptm_iter.key();
            if (stem != str_word) {// only count parses with non-null suffixes
                total_retrieved++;
                if (p_retrieved_parses != NULL) {
                    const QString& suffix = eval_ptm_iter.value()->m_suffix;
                    // store parse into collection of retrieved parses
                    p_retrieved_parses->add_parse_triple(str_word, stem, suffix);
                    // qDebug() << "[RETRIEVED] " << str_word << ": " << stem << "-" << suffix;
                }
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

    results.set_true_positive_count(true_positives);
    results.set_retrieved_parse_count(total_retrieved);
    results.set_gs_parse_count(total_correct);
    results.set_gs_word_count(temp_gs_word_count);
    results.set_overlap_word_count(temp_overlap_word_count);

    results.calculate_precision_recall();

    //qDebug() << "True positives: " << true_positives;
    //qDebug() << "Total # of correct parses in GS: " << total_correct;
    //qDebug() << "Total # retrieved: " << total_retrieved;
    return true;
}

void delete_ptm(QMap<QString, Parse_triple*>* ptm)
{
    typedef QMap<QString, Parse_triple*> Parse_triple_map;
    Parse_triple_map::iterator ptm_iter;
    for (ptm_iter = ptm->begin(); ptm_iter != ptm->end(); ptm_iter++) {
        delete ptm_iter.value();
    }
    delete ptm;
}

/*!
 * \brief Reads in a GoldStandard XML file, and stores parses given by
 * the Gold Standard in m_gs_parses. m_XML_file_name must be set before
 * using this function.
 * \return True if reading succeeds. False if an error occurred (such as
 * invalid XML file format.
 */
bool GoldStandard::read_XML()
{
// XML file must be first opend to use this function


    if (m_XML_file_name.isEmpty())
        return false;

    m_gs_parses = ParseMapHandler();
    QFile goldStdFile( m_XML_file_name );

    if (!goldStdFile.open(QIODevice::ReadOnly))
        return false;

    QDomDocument doc( "Alchemist" ) /* author_data, document_data */;

    QString errorMsg;
    int errorLine, errorColumn;
    if( !doc.setContent( &goldStdFile, &errorMsg, &errorLine, &errorColumn ) )
    {
//Maybe we should put this back in.
//				QMessageBox::warning( this, "Gold Standard : XML Error",
//				  QString( errorMsg + "\nLine: %1" + "Col: %2" ).arg( errorLine ).arg( errorColumn ), QMessageBox::Ok, NULL, NULL );

        return false;
    }
    QDomElement alchemist_doc, morph, piece, string, wordnode
                /* gloss, element, notes, morpheme, allomorph,
                lmnt, feature, name, feature_id, instance_id */;
    //
    QDomNode node1, node2, node3, node4;

    QString word, stem, affix;
    ParseMap::iterator word_iter;
    Parse_triple* new_pt;
    Parse_triple_map* new_ptm;

    bool skipWord = false;
    int lastEnd, start, length, pieceLength;

    alchemist_doc = doc.documentElement();

    if( alchemist_doc.tagName() != "alchemist-doc" ) {
        errorMsg = "The XML document \"" + alchemist_doc.tagName() + "\" is not an alchemist document.";
        /* old version
        QMessageBox::information( NULL, "Gold Standard : XML Error", errorMsg, "OK" );
        */
        QMessageBox::information(nullptr, "Gold Standard : XML Error", errorMsg, QMessageBox::Ok);
        return false;
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

            new_ptm = new Parse_triple_map();

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
                            delete_ptm(new_ptm);
                            break;
                        }
                        if ( morph.tagName() == "affix" || morph.tagName() == "root" ) {
                            start = piece.attribute( "start" ).toInt();
                            pieceLength = piece.attribute("length").toInt();

                            // discard word if it has empty spaces in it
                            if (start > lastEnd) {
                                skipWord = true;
                                delete_ptm(new_ptm);
                                break;
                            } else {
                                if (start != 0) {
                                    stem = word.left(start);
                                    affix = word.right(length - start);
                                    new_pt = new Parse_triple(stem, affix, QString());
                                    new_ptm->insert(stem, new_pt);
                                    if ( start < lastEnd ) {
                                        stem = word.left(lastEnd);
                                        affix = word.right(length - lastEnd);
                                        new_pt = new Parse_triple(stem, affix, QString());
                                        new_ptm->insert(stem, new_pt);
                                    }
                                }
                                lastEnd = start + pieceLength;
                                //pStem->CutRightBeforeHere( start ); //-- from old code
                            }
                        }
                        node4 = node4.nextSibling();
                    }
                } else {
                    // Word has unassigned pieces, skip...
                    skipWord = true;
                    delete_ptm(new_ptm);
                }
                node3 = node3.nextSibling();
            }
            if( skipWord ) continue;

            word_iter = m_gs_parses->find(word);
            if (word_iter == m_gs_parses->end()) {
                word_iter = m_gs_parses->insert(word, new_ptm);
            } else {
                Parse_triple_map* existing_ptm = word_iter.value();
                for (Parse_triple_map::const_iterator ptm_iter = new_ptm->constBegin();
                     ptm_iter != new_ptm->constEnd(); ptm_iter++) {
                    existing_ptm->insert(ptm_iter.key(), ptm_iter.value());
                }
                delete_ptm(new_ptm);
            }

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
    //qDebug() << 453 << "Goldstandard.cpp: gold standard parsed";
    goldStdFile.close();
    return true;
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


