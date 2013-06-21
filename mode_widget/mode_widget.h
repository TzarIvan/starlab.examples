#pragma once
#include "interfaces/ModePlugin.h"
#include "interfaces/ModePluginDockWidget.h"

class mode_widget : public ModePlugin{
    Q_OBJECT
    Q_INTERFACES(ModePlugin)

/// @{ StarlabPlugin
    QIcon icon(){ return QIcon(":/icons/mode_widget.png"); }
    bool isApplicable(){  return true; }
/// @}

/// @{ ModePlugin
    void create();
    void destroy(){ 
        qDebug() << "example_mode_withwidget::::destroyed()";
        dockwidget->deleteLater(); 
    }
    void suspend(){ 
        qDebug() << "example_mode_withwidget::suspended()";    
        dockwidget->setEnabled(false); 
    }
    void resume(){ 
        qDebug() << "example_mode_withwidget::resumed()";    
        dockwidget->setEnabled(true); 
    }
    bool documentChanged(){
        qDebug() << "example_mode_withwidget::documentChanged()";
        return true;        
    }
/// @} 

/// @{ Local data    
    ModePluginDockWidget* dockwidget;
/// @}

/// @{ Python console functions \see core/gui_python
/// Public slots can be used by the python console. This, for example, 
/// can be accessed in the python console by typing "mode.myfunction(123)"
public slots:
    void myfunction(int i){
        qDebug() << "I am " << i;
    }
/// @}
};
