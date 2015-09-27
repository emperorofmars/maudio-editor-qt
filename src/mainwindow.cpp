/*
 * Project Maudio-Editor-Qt
 * Copyright (C) 2015 Martin Schwarz
 * See LICENSE.txt for the full license
 */

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
    mProjectView = ui->MainTabBar;
}

MainWindow::~MainWindow()
{
    saveOnDanger(true);
    delete ui;
}

bool MainWindow::saveOnDanger(bool onExit)
{
    bool returnVal = true;

    auto project = mProjectView->getProject();
    if(!project) return returnVal;
    if(!mProjectView->getModified()) return returnVal;

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
            bool success = true;
            do{
                if(!saveDialog()){
                    int ret2 = QMessageBox::warning(this, tr("Saveing failed!"), tr("Do you want to try again?"), QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
                    if(ret2 == QMessageBox::Ok) success = false;
                    else{
                        returnVal = false;
                        success = true;
                    }
                }
            }
            while(!success);
            break;
        }
        case QMessageBox::Cancel:
        {
            returnVal = false;
            break;
        }
    }

    return returnVal;
}

bool MainWindow::saveDialog()
{
    auto project = mProjectView->getProject();
    if(!project) return false;

    bool ret = true;

    QString startpath = mSaveLocation;
    if(startpath == QString()) startpath = QDir::homePath();
    QString file = QFileDialog::getSaveFileName(this, tr("Save Project As"), startpath, tr("MAudio Project Files (*.maup);;All FIles (*)"));
    if(file == QString()) return false;

    project->setSaveFile(file.toStdString().c_str());
    try{
        project->save();
        mSaveLocation = file;
    }
    catch(std::exception &e){
        ret = false;
    }

    if(ret) mProjectView->setSaved();
    return ret;
}

void MainWindow::onProjectOpened()
{
    ui->actionProject_Settings->setEnabled(true);
    ui->actionSave_As->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionAdd_Scene->setEnabled(true);
    ui->actionAdd_Node->setEnabled(true);
    ui->buttonAddNode->setEnabled(true);
    ui->buttonAddScene->setEnabled(true);
    ui->buttonDeleteNode->setEnabled(true);
}

void MainWindow::onProjectClosed()
{
    ui->actionProject_Settings->setEnabled(false);
    ui->actionSave_As->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionAdd_Scene->setEnabled(false);
    ui->actionAdd_Node->setEnabled(false);
    ui->buttonAddNode->setEnabled(false);
    ui->buttonAddScene->setEnabled(false);
    ui->buttonDeleteNode->setEnabled(false);
}

void MainWindow::on_actionNew_Project_triggered()
{
    if(!saveOnDanger()) return;
    std::shared_ptr<maudio::Project> project(new maudio::Project("New_Project"));
    mProjectView->setProject(project);

    onProjectOpened();
    this->setWindowTitle(tr("MAudio Editor: ") + QString(project->getName()));
    statusBar()->showMessage(tr("New project created."));
}

void MainWindow::on_actionOpen_Project_triggered()
{
    if(!saveOnDanger()) return;
    std::shared_ptr<maudio::Project> project(new maudio::Project());

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Project"), QDir::homePath(), tr("MAudio Project Files (*.maup);;All FIles (*)"));
    if(fileName == QString()) return;
    project->load(fileName.toStdString().c_str());
    mProjectView->setProject(project);

    mSaveLocation = fileName;

    onProjectOpened();
    this->setWindowTitle(tr("MAudio Editor: ") + QString(project->getName()));
    statusBar()->showMessage(tr("Project \"") + fileName + tr("\" successfully loaded."));
}

void MainWindow::on_actionAdd_Scene_triggered()
{
    mProjectView->on_add_scene();
    statusBar()->showMessage(tr("Added scene"));
}

void MainWindow::on_actionAdd_Node_triggered()
{
    mProjectView->on_add_node();
    statusBar()->showMessage(tr("Added Node"));
}

void MainWindow::on_actionDelete_Scene_triggered()
{
    mProjectView->on_remove_current_scene();
    statusBar()->showMessage(tr("Removed scene"));
}

void MainWindow::on_actionSave_triggered()
{
    if(mSaveLocation == QString()) saveDialog();
    else{
        auto project = mProjectView->getProject();
        if(!project) return;
        project->save();
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    saveDialog();
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}
