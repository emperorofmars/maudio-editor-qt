#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(close()));
}

MainWindow::~MainWindow()
{
    saveOnDanger(true);
    delete ui;
}

bool MainWindow::saveOnDanger(bool onExit)
{
    bool success = true;
    bool returnVal = true;

    if(!mProject) return returnVal;

    do{
        QMessageBox msg;
        msg.setWindowTitle("Save File?");
        msg.setText("You have unsaved changes.");
        msg.setInformativeText("Do you want to save your changes?");
        QMessageBox::StandardButtons btns = QMessageBox::Save | QMessageBox::Discard;
        if(!onExit) btns |= QMessageBox::Cancel;
        msg.setStandardButtons(btns);
        msg.setDefaultButton(QMessageBox::Save);
        int ret = msg.exec();
        switch(ret){
            case QMessageBox::Save:
            {
                QString file = QFileDialog::getSaveFileName(this, tr("Save Project As"), QDir::homePath(), tr("MAudio Project Files (*.maup);;All FIles (*)"));
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
            case QMessageBox::Cancel:
            {
                returnVal = false;
                break;
            }
        }
    }
    while(!success);

    return returnVal;
}

void MainWindow::onProjectOpened()
{
    ui->actionProject_Settings->setEnabled(true);
    ui->actionSave_As->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionAdd_Scene->setEnabled(true);
}

void MainWindow::onProjectClosed()
{
    ui->actionProject_Settings->setEnabled(false);
    ui->actionSave_As->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionAdd_Scene->setEnabled(false);
    ui->actionAdd_Node->setEnabled(false);
}

void MainWindow::on_actionNew_Project_triggered()
{
    if(!saveOnDanger()) return;
    mProject.reset(new maudio::Project("New_Project"));

    onProjectOpened();
    this->setWindowTitle(tr("MAudio Editor: ") + QString(mProject->getName()));
    statusBar()->showMessage(tr("New project created."));
}

void MainWindow::on_actionOpen_Project_triggered()
{
    if(!saveOnDanger()) return;
    mProject.reset(new maudio::Project());
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), QDir::homePath(), tr("MAudio Project Files (*.maup);;All FIles (*)"));
    mProject->load(fileName.toStdString().c_str());

    onProjectOpened();
    this->setWindowTitle(tr("MAudio Editor: ") + QString(mProject->getName()));
    statusBar()->showMessage(tr("Project \"") + fileName + tr("\" successfully loaded."));
}

void MainWindow::on_actionAdd_Scene_triggered()
{
    std::shared_ptr<maudio::Scene> scene(new maudio::Scene());
    mProject->addScene(scene);
    QWidget *tab = new QWidget(ui->tabWidget);
    ui->tabWidget->addTab(tab, QIcon(), tr("1: ") + QString(scene->getName()));
    ui->actionAdd_Node->setEnabled(true);

    statusBar()->showMessage(tr("Added scene"));
}

void MainWindow::on_actionAdd_Node_triggered()
{

}
