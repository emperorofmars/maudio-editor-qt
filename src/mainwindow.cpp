#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    saveOnDanger();
    delete ui;
}

void MainWindow::saveOnDanger()
{
    if(!mProject) return;

    bool success = true;

    do{
        QMessageBox msg;
        msg.setText("You have unsaved changes.");
        msg.setInformativeText("Do you want to save your changes?");
        msg.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msg.setDefaultButton(QMessageBox::Save);
        int ret = msg.exec();
        switch(ret){
        case QMessageBox::Save:
            QString file = QFileDialog::getSaveFileName(NULL, tr("Save Project As"), QDir::homePath(), tr("MAudio Project Files (*.maup);;All FIles (*)"));
            mProject->setSaveFile(file.toStdString().c_str());
            try{
                mProject->save();
            }
            catch(std::exception &e){
                int ret2 = QMessageBox::warning(this, tr("Saveing failed!"), tr("Do you want to try again?"), QMessageBox::Accepted | QMessageBox::Cancel, QMessageBox::Accepted);
                if(ret2 == QMessageBox::Accepted) success = false;
            }
            break;
        }
    }while(!success);
}

void MainWindow::on_actionNew_Project_triggered()
{
    saveOnDanger();
    mProject.reset(new maudio::Project());
    statusBar()->showMessage(tr("New project created."));
}

void MainWindow::on_actionOpen_Project_triggered()
{
    saveOnDanger();
    mProject.reset(new maudio::Project());
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), QDir::homePath(), tr("MAudio Project Files (*.maup);;All FIles (*)"));
    mProject->load(fileName.toStdString().c_str());
    statusBar()->showMessage(tr("Project \"") + fileName + tr("\" successfully loaded."));
}
