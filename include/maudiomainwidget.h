/*
 * Project Maudio-Editor-Qt
 * Copyright (C) 2015 Martin Schwarz
 * See LICENSE.txt for the full license
 */

#ifndef MAUDIOMAINWIDGET_H
#define MAUDIOMAINWIDGET_H

#include <QTabWidget>
#include "maudio/scene/Project.hpp"
#include "maudiosceneview.h"
#include <memory>
#include <vector>

class MAudioMainWidget : public QTabWidget, maudio::BaseObserver
{
    Q_OBJECT
public:
    explicit MAudioMainWidget(QWidget *parent = 0);

    void setProject(std::shared_ptr<maudio::Project> project);
    std::shared_ptr<maudio::Project> getProject();

    bool getModified();
    void setSaved();

    virtual void notify(const maudio::IObservable *origin, maudio::NoticeType type, const char *message);

signals:

public slots:
    void on_add_scene();
    void on_remove_current_scene();
    void on_add_node();
    void on_remove_node();
private:
    void parseProject();
    void showErrorTab();

    std::shared_ptr<maudio::Project> mProject;
    std::vector<MAudioSceneView *> mSceneViews;
    bool mModified = false;
};

#endif // MAUDIOMAINWIDGET_H
