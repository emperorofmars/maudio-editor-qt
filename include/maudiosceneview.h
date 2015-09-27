/*
 * Project Maudio-Editor-Qt
 * Copyright (C) 2015 Martin Schwarz
 * See LICENSE.txt for the full license
 */

#ifndef MAUDIOSCENEVIEW_H
#define MAUDIOSCENEVIEW_H

#include <QGraphicsView>
#include "maudio/scene/Scene.hpp"
#include <memory>

class MAudioSceneView : public QGraphicsView, maudio::BaseObserver
{
    Q_OBJECT
public:
    explicit MAudioSceneView(std::shared_ptr<maudio::Scene> scene, QWidget *parent = 0);

    bool getModified();
    void setSaved();

    virtual void notify(const maudio::IObservable *origin, maudio::NoticeType type, const char *message);

signals:

public slots:
    void on_add_node();
    void on_remove_node();

private:
    std::shared_ptr<maudio::Scene> mScene;
    bool mModified = false;
    //TODO: mNodes
    //std::shared_ptr<MAudioNodeView> mNodes;
};

#endif // MAUDIOSCENEVIEW_H
