#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>
#include "Board.h"
#include <ui_main_window.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNewProjectClicked();
    void onImportProjectClicked();
    void onHistoryClicked();
    void onHelpClicked();
    void onExampleClicked();

private:
    Ui::MainWindow *ui;
    Board* project;
};
#endif // MAINWINDOW_H
