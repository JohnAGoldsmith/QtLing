#include "mainwindow_menubar.h"
#include "mainwindow.h"

MainMenuBar::MainMenuBar()
{

}

MainMenuBar::MainMenuBar(MainWindow *parent):
    gs_is_ready(false),
    lexicon_is_ready(false),
    eval_parse_is_ready(false)
{
    setParent(parent);
    m_parent_window = parent;
    fileMenu = addMenu(tr("&File"));
    fileMenu->addAction(parent->openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(parent->saveAsAct);
    fileMenu->addAction(parent->exitAct);

    editMenu = addMenu(tr("&Edit"));
#ifndef QT_NO_CLIPBOARD
    editMenu->addAction(parent->cutAct);
    editMenu->addAction(parent->copyAct);
    editMenu->addAction(parent->pasteAct);
#endif
    editMenu->addAction(parent->findAct);
    editMenu->addAction(parent->findLeftAct);
    editMenu->addAction(parent->findRightAct);

    helpMenu = addMenu(tr("&Help"));
    helpMenu->addAction(parent->aboutAct);
    helpMenu->addAction(parent->aboutQtAct);

    evalMenu = addMenu(tr("&Evaluate"));
    evalMenu->addAction(parent->importAct);
    evalMenu->addAction(parent->evalAct);
    evalMenu->addAction(parent->importMorfessorAct);
    evalMenu->addAction(parent->evalMorfessorAct);
}

void MainMenuBar::check_and_enable_evalAct()
{
    if (lexicon_is_ready && gs_is_ready) {
        if (!m_parent_window->evalAct->isEnabled()) {
            m_parent_window->evalAct->setEnabled(true);
        }
    }
}

void MainMenuBar::check_and_enable_evalMorfessorAct()
{
    if (lexicon_is_ready && gs_is_ready && eval_parse_is_ready) {
        if (!m_parent_window->evalMorfessorAct->isEnabled()) {
            m_parent_window->evalMorfessorAct->setEnabled(true);
        }
    }
}

void MainMenuBar::gs_ready()
{
    // qDebug() << 136 << "MainMenu.cpp: gs_loaded";
    gs_is_ready = true;
    check_and_enable_evalAct();
    check_and_enable_evalMorfessorAct();
}

void MainMenuBar::lexicon_ready()
{
    lexicon_is_ready = true;
    check_and_enable_evalAct();
    check_and_enable_evalMorfessorAct();
}

void MainMenuBar::eval_parse_ready()
{
    eval_parse_is_ready = true;
    check_and_enable_evalMorfessorAct();
}
