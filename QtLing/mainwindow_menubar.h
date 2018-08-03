#ifndef MAINWINDOW_MENUBAR_H
#define MAINWINDOW_MENUBAR_H

#include <QObject>
#include <QWidget>
#include <QMenuBar>

class MainWindow;

/*!
 * \brief The MainMenuBar class is the main menubar object on top of the main
 * window; also controls enabling/disabling of actions related to evaluation.
 *
 * Originally the menubar was implemented by modifying the menuBar() member
 * variable of MainWindow. Now it is implemented by defining a new MainMenuBar
 * class with our own custom features and calling setMenuBar() (see
 * mainwindow_actions.cpp:115-116).
 */
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
