#pragma once
#include <QThread>
#include <QtConcurrentRun>
#include <QDebug>
#include <QtOpenGL>
#include <QQueue>

#include "Starlab.h"
#include "OpenNI.h"

using namespace openni;
using namespace Starlab;

/// FD
class ModeKinectListener;
class KinectWidget;

class KinectHelper : public QObject{
    Q_OBJECT
    
/// @{ data
public:
    typedef Starlab::BBox3 BBox3;
    typedef Eigen::Vector3d Point;
    typedef Eigen::Matrix<Point, Eigen::Dynamic, Eigen::Dynamic> PImage; ///< an image of Points    
private:
    typedef QQueue<VideoFrameRef> DepthQueue;
    typedef QQueue<VideoFrameRef> ColorQueue;
    
private:
    BBox3 _bbox;
public:
    BBox3 bbox(){ return _bbox; }
    QMutex* mutex(){ return &_mutex; }
private:
    QTimer* timer;
    QMutex _mutex;
    PImage* points_front_buffer; ///< &cloud_buffer_1
    PImage* points_back_buffer;  ///< &cloud_buffer_2  
    PImage points_buffer_1;
    PImage points_buffer_2;
    
    QImage* color_front_buffer; ///< &color_buffer_1
    QImage* color_back_buffer;  ///< &color_buffer_2
    QImage color_buffer_1;
    QImage color_buffer_2;
public:
    /// Lock mutex before accessing this resource!!
    QImage& colorBuffer(){ return *color_front_buffer; }
    /// Lock mutex before accessing this resource!!
    PImage& pointBuffer(){ return *points_front_buffer; }
    
private:
    Status rc;
    Device device;
    VideoStream depth, color;
    ModeKinectListener* depthListener;
    ModeKinectListener* colorListener;
/// @}
    
/// @{
    bool has_consumed_first_frame; ///< false
    bool data_ready(){ return has_consumed_first_frame; }
/// @} 
///    
/// @{ constructor/destructor    
public:
    KinectHelper(QObject* parent=0);
    void start();    
private:
    /// Use destroy()!
    ~KinectHelper(){}
public slots: 
    void destroy();
private:
    int initialize_device();
/// @}
    
/// @{
public slots:
    /// called whenever a new depth frame is received. This function moves the 
    /// vertices and modifies the mesh
    void updateDepthFrame(VideoFrameRef frame);
    /// called whenever a new color frame is received. It converts the color frame 
    /// into a QImage which is passed to the main (GUI) thread through a signal
    void updateColorFrame(VideoFrameRef frame);
public slots:
    /// slot to consume the queue elements
    void consume();
private:    
    void queuesStatus(); ///< debugging helper
    DepthQueue   depthQueue;
    ColorQueue   colorQueue;
    void consume_depth(VideoFrameRef frame);
    void consume_color(VideoFrameRef frame);
/// @}
    
/// @{ hooks to be used in client application
private:
    BBox3 compute_frame_bbox(VideoFrameRef frame);    
public slots:    
    /// Draws the point cloud in OpenGL
    void drawCloud();
signals:
    /// Updates the bounding box
    void scene_bbox_updated(BBox3 box);
/// @}

    
/// @{ Color GUI support
public slots:
    void drawColor(QImage* image= NULL);
    void setWidget(QWidget* widget){ this->widget = widget; }
    void setColorLabel(QLabel* label){ this->colorLabel = label; }
private:
    QLabel*  colorLabel; ///< NULL
    QWidget* widget; ///< NULL
    /// If widget was not set and drawColor was called, 
    /// create a simple standalone window
    void autoCreateWidgetsIfNecessary();    
/// @} 
};


/// The Frame Listener class emits a signal whenever it receives a new frame
class ModeKinectListener : public QObject, public VideoStream::NewFrameListener{
    Q_OBJECT
public:
    ModeKinectListener(QObject* parent) : QObject(parent){}
private:
    VideoFrameRef newFrame;
public:
    void onNewFrame(VideoStream& stream){
        stream.readFrame(&newFrame);
        /// Emits a signal whenever a new frame is available
        emit new_frame_available(newFrame);
    }
signals:
    void new_frame_available(VideoFrameRef);
};

