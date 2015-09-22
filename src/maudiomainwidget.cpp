#include "include/maudiomainwidget.h"
#include <QGraphicsView>
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

std::shared_ptr<maudio::Project> MAudioMainWidget::getProject()
{
    return mProject;
}

void MAudioMainWidget::parseProject()
{
    this->clear();
    if(!mProject){
        showErrorTab();
        return;
    }
    for(unsigned int i = 0; i < mProject->getNumScenes(); i++){
        this->addTab(new QGraphicsView(), QString(mProject->getScene(i)->getName()));
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

    try{
        std::string str("scene ");
        str.append(std::to_string(mProject->getNumScenes() + 1));
        mProject->addScene(std::shared_ptr<maudio::Scene> (new maudio::Scene(str.c_str())));
        this->addTab(new QGraphicsView(), QString(str.c_str()));
    }
    catch(std::exception &e){
    }
}

void MAudioMainWidget::on_remove_current_scene()
{
    if(!mProject) return;
    mProject->removeScene(this->currentIndex());
    this->removeTab(this->currentIndex());
}

void MAudioMainWidget::on_add_node()
{

}
