
#ifndef MEDIT_MAINWINDOW_H
#define MEDIT_MAINWINDOW_H

#include <QMainWindow>

namespace Ui{

class MainWindow;

}

class MainWindow : public QMainWindow{
	Q_OBJECT
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;
};

#endif // MEDIT_MAINWINDOW_H
