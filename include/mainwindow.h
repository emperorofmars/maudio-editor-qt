#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "maudio/scene/Project.hpp"
#include "maudiomainwidget.h"
#include <QMainWindow>
#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool saveOnDanger(bool onExit = false);
    bool saveDialog();

private slots:
    void on_actionNew_Project_triggered();

    void on_actionOpen_Project_triggered();

    void on_actionAdd_Scene_triggered();

    void on_actionAdd_Node_triggered();

    void on_actionDelete_Scene_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

private:
    void onProjectOpened();
    void onProjectClosed();

    Ui::MainWindow *ui;

    MAudioMainWidget *mProjectView = NULL;
    QString mSaveLocation;
};

#endif // MAINWINDOW_H
