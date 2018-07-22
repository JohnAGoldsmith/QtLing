#ifndef MAINWINDOW_MENUBAR_H
#define MAINWINDOW_MENUBAR_H

#include <QObject>
#include <QWidget>
#include <QMenuBar>

class MainWindow;

class MainMenuBar : public QMenuBar
{
    Q_OBJECT
    MainWindow      *m_parent_window;
    QMenu           *fileMenu;
    QMenu           *editMenu;
    QMenu           *helpMenu;
    QMenu           *evalMenu;

    bool            gs_is_ready;
    bool            lexicon_is_ready;
    bool            eval_parse_is_ready;

    void            check_and_enable_evalAct();
    void            check_and_enable_evalMorfessorAct();

public:
    MainMenuBar();
    MainMenuBar(MainWindow* parent);

public slots:
    void            gs_ready();
    void            lexicon_ready();
    void            eval_parse_ready();
};

#endif // MAINWINDOW_MENUBAR_H
