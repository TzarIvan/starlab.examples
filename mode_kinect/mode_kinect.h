#pragma once
#include "ModePlugin.h"
#include "ModePluginDockWidget.h"
using namespace Starlab;

class KinectHelper;

class mode_kinect : public ModePlugin{
    Q_OBJECT
    Q_INTERFACES(ModePlugin)
    
    QString name(){ return "Mode Kinect"; }
    QIcon icon(){ return QIcon(":/icons/kinect.png"); }
    bool isApplicable(){  return true; }
    void create();
    bool documentChanged(){ return true; }

private:
    KinectHelper* khelper;
    QImage rgb;
public slots:
    void work();   
    void decorate();
};
