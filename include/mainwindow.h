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

    void saveOnDanger();

private slots:
    void on_actionNew_Project_triggered();

    void on_actionOpen_Project_triggered();

private:
    Ui::MainWindow *ui;

    std::shared_ptr<maudio::Project> mProject;
};

#endif // MAINWINDOW_H
