#include "mode_kinect.h"
Q_EXPORT_PLUGIN(mode_kinect)

#include "KinectHelper.h"

const int FPS = 60;

void mode_kinect::create(){   
    // qDebug() << "mode_kinect::create()" << QThread::currentThreadId();
    
    /// create kinect & workers
    khelper = new KinectHelper();  
    connect( parent, SIGNAL(destroyed()), khelper, SLOT(deleteLater()) );
    
    /// Setup viewer BBOX
    /// @todo match exactly the perspective projection of the kinect
    BBox3 bbox = khelper->bbox();
    Vector3 minbound = bbox.min();
    Vector3 maxbound = bbox.max();
    qglviewer::Vec min_bound(minbound.x(),minbound.y(),minbound.z());
    qglviewer::Vec max_bound(maxbound.x(),maxbound.y(),maxbound.z());
    drawArea()->camera()->fitBoundingBox( min_bound, max_bound );
    drawArea()->camera()->setSceneRadius((max_bound - min_bound).norm() * 0.4);
    drawArea()->camera()->setSceneCenter((min_bound + max_bound) * 0.5);
    drawArea()->camera()->showEntireScene();    
    
    /// create a dockwidget to display the color/depth frames
    ModePluginDockWidget* dockwidget = new ModePluginDockWidget("Kinect Widget",mainWindow(),parent);
    QLabel* colorLabel = new QLabel();
    dockwidget->addWidget(colorLabel);
    connect( parent, SIGNAL(destroyed()), dockwidget, SLOT(deleteLater()) );
    mainWindow()->addDockWidget(Qt::RightDockWidgetArea,dockwidget);
    dockwidget->hide(); /// (will be raised when data arrives)

    /// hook widgets to kinect
    khelper->setColorLabel(colorLabel);
    khelper->setWidget(dockwidget);
    
#define ENABLE_KINECT_THREAD
#ifdef ENABLE_KINECT_THREAD
    QThread* k_thread = new QThread(khelper);
    khelper->moveToThread(k_thread);
    k_thread->start();
#endif
    
    /// Creates constant framerate events
    QTimer* timer = new QTimer(this);
    timer->setSingleShot(false);
    timer->setInterval((1.0/FPS)*1000.0); 
    
    /// Timed events, first work, then display
    connect(timer, SIGNAL(timeout()), this, SLOT(work()) );
    connect(timer, SIGNAL(timeout()), drawArea(), SLOT(updateGL()) );
   
    /// start fetching stuff from kinect & the refresh timer
    timer->start();
    khelper->start();
}

void mode_kinect::work(){
    QMutexLocker locker(khelper->mutex());       

    /// Now process the RGB buffer
    rgb = khelper->colorBuffer();
    for (int w = 0; w<rgb.width(); ++w){
        for(int h = 0; h<rgb.height(); ++h){
            // rgb.pixel(w,h);
            rgb.setPixel(w,h, 0);

            /*..... DO SOMETHING ....*/
        }
    }
    
    /// Now process the POINTS buffer
    KinectHelper::PImage& points = khelper->pointBuffer();                
    for (int y = 0; y<points.cols(); ++y){
        for(int x = 0; x<points.rows(); ++x){
            points(x,y);
            /*..... DO SOMETHING ....*/                
        }
    }

//    drawArea()->updateGL();
//    QApplication::processEvents();
}

void mode_kinect::decorate(){
    /// These will be executed in the main GUI thread! If you have to 
    /// do something special simply define your local function
    QMutexLocker locker(khelper->mutex());
    khelper->drawCloud();
    khelper->drawColor(&rgb);
}
