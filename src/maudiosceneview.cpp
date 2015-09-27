/*
 * Project Maudio-Editor-Qt
 * Copyright (C) 2015 Martin Schwarz
 * See LICENSE.txt for the full license
 */

#include "maudiosceneview.h"

MAudioSceneView::MAudioSceneView(std::shared_ptr<maudio::Scene> scene, QWidget *parent) :
    QGraphicsView(new QGraphicsScene(), parent)
{
    mScene = scene;
}

bool MAudioSceneView::getModified(){
    return mModified;
}

void MAudioSceneView::setSaved(){
    mModified = false;
}

void MAudioSceneView::notify(const maudio::IObservable *origin, maudio::NoticeType type, const char *message)
{

}

void MAudioSceneView::on_add_node(){
    mModified = true;
}

void MAudioSceneView::on_remove_node(){
    mModified = true;
}
