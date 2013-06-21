include($$[STARLAB])
include($$[OPENNI])
StarlabTemplate(plugin)

HEADERS += mode_kinect.h \
    KinectHelper.h
SOURCES += mode_kinect.cpp \
    KinectHelper.cpp
RESOURCES += resources.qrc
