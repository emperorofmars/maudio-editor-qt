/*
 * Project Maudio-Editor-Qt
 * Copyright (C) 2015 Martin Schwarz
 * See LICENSE.txt for the full license
 */

#include "maudiomainwidget.h"
#include <QLabel>
#include <string>

MAudioMainWidget::MAudioMainWidget(QWidget *parent) :
    QTabWidget(parent)
{
    this->setTabsClosable(true);
}

void MAudioMainWidget::setProject(std::shared_ptr<maudio::Project> project)
{
    mProject = project;
    parseProject();
}

bool MAudioMainWidget::getModified(){
    for(unsigned int i = 0; i < mSceneViews.size(); i++){
        if(mSceneViews[i]->getModified()) return true;
    }
    return mModified;
}

void MAudioMainWidget::setSaved(){
    for(unsigned int i = 0; i < mSceneViews.size(); i++){
        mSceneViews[i]->setSaved();
    }
    mModified = false;
}

std::shared_ptr<maudio::Project> MAudioMainWidget::getProject()
{
    return mProject;
}

void MAudioMainWidget::parseProject()
{
    this->clear();
    mSceneViews.clear();
    if(!mProject){
        showErrorTab();
        return;
    }
    for(unsigned int i = 0; i < mProject->getNumScenes(); i++){
        this->addTab(new QGraphicsView(this), QString(mProject->getScene(i)->getName()));
    }
    if(mProject->getNumScenes() == 0){
        on_add_scene();
    }
}

void MAudioMainWidget::showErrorTab()
{
    this->addTab(new QLabel("No project opened."), QString("Tab"));
}

void MAudioMainWidget::notify(const maudio::IObservable *origin, maudio::NoticeType type, const char *message)
{

}

void MAudioMainWidget::on_add_scene()
{
    if(!mProject) return;

    mModified = true;

    try{
        std::string str("scene ");
        str.append(std::to_string(mProject->getNumScenes() + 1));
        std::shared_ptr<maudio::Scene> scene(new maudio::Scene(str.c_str()));
        mProject->addScene(scene);
        MAudioSceneView *sceneView = new MAudioSceneView(scene);
        mSceneViews.push_back(sceneView);
        this->addTab(sceneView, QString(str.c_str()));
    }
    catch(std::exception &e){
    }
}

void MAudioMainWidget::on_remove_current_scene()
{
    if(!mProject) return;

    mModified = true;

    int index = this->currentIndex();
    mProject->removeScene(index);
    mSceneViews.erase(mSceneViews.begin() + index);
    this->removeTab(index);
}

void MAudioMainWidget::on_add_node(){
    if(!mProject) return;
    if(this->currentIndex() >= (int)mSceneViews.size()) return;
    mSceneViews[this->currentIndex()]->on_add_node();
}

void MAudioMainWidget::on_remove_node(){
    if(!mProject) return;
    if(this->currentIndex() >= (int)mSceneViews.size()) return;
    mSceneViews[this->currentIndex()]->on_remove_node();
}
