#include "mode_widget.h"
Q_EXPORT_PLUGIN(mode_widget)

#include "parameters/RichParameterSet.h"
#include "parameters/ParametersFrame.h"

void mode_widget::create(){
    qDebug() << "example_mode_withwidget::created()";  
    
    /// Create a simple widget with a button
    dockwidget = new ModePluginDockWidget(mainWindow());
    
    /// Fill it with pre-made parameter frame
    RichParameterSet* pars = new RichParameterSet(dockwidget);
    pars->addParam( new RichFloat("PAR1",0,"PARAMETER 1","THIS IS PAR1"));
    pars->addParam( new RichFloat("PAR2",0,"PARAMETER 2","THIS IS PAR2"));
    pars->addParam( new RichFloat("PAR3",0,"PARAMETER 3","THIS IS PAR3"));
    ParametersFrame* frame = new ParametersFrame(dockwidget);
    frame->load(pars);
    dockwidget->setWidget(frame);
    mainWindow()->addDockWidget(Qt::RightDockWidgetArea,dockwidget);
}

