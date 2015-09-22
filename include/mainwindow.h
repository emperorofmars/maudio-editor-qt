#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "maudio/scene/Project.hpp"
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

private slots:
    void on_actionNew_Project_triggered();

    void on_actionOpen_Project_triggered();

    void on_actionAdd_Scene_triggered();

    void on_actionAdd_Node_triggered();

private:
    void onProjectOpened();
    void onProjectClosed();

    Ui::MainWindow *ui;

    std::shared_ptr<maudio::Project> mProject;
};

#endif // MAINWINDOW_H
