#ifndef MAUDIOMAINWIDGET_H
#define MAUDIOMAINWIDGET_H

#include <QTabWidget>
#include "maudio/scene/Project.hpp"
#include <memory>
#include <vector>

class MAudioMainWidget : public QTabWidget, maudio::BaseObserver
{
    Q_OBJECT
public:
    explicit MAudioMainWidget(QWidget *parent = 0);

    void setProject(std::shared_ptr<maudio::Project> project);
    std::shared_ptr<maudio::Project> getProject();

    virtual void notify(const maudio::IObservable *origin, maudio::NoticeType type, const char *message);

signals:

public slots:
    void on_add_scene();
    void on_remove_current_scene();
    void on_add_node();

private:
    void parseProject();
    void showErrorTab();

    std::shared_ptr<maudio::Project> mProject;
    //TODO: SceneView Class
    //std::vector<std::unique_ptr<SceneView>> mSceneViews;
};

#endif // MAUDIOMAINWIDGET_H
