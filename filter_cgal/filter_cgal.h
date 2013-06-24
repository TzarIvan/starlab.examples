#pragma once
#include "SurfaceMeshPlugins.h"

class filter_cgal : public SurfaceMeshFilterPlugin{
    Q_OBJECT
    Q_INTERFACES(FilterPlugin)

public:
    QString name() { return "Convex Hull (CGAL)"; }
    void applyFilter(RichParameterSet*);
};
