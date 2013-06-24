#include "filter_cgal.h"
Q_EXPORT_PLUGIN(filter_cgal)

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_3.h> /// To compute convex hull
#include <CGAL/Tetrahedron_3.h> /// To compute convex hull volume
#include <CGAL/Polyhedron_incremental_builder_3.h> /// To convert triangle into single face polyhedron
#include <CGAL/Polyhedron_items_with_id_3.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K; /// Chull requires exact predicate
typedef K::Point_3 Point_3; /// Type of point for chull call
typedef std::list<Point_3> Point_3_list; /// List of points
typedef CGAL::Polyhedron_3<K,CGAL::Polyhedron_items_with_id_3> Polyhedron_3; 

#include "Polyhedron3_to_SurfaceMesh.h"

/// Take a look here for better conversion to/from CGAL
// #include "CGAL/Polyhedron_copy_3.h"

void filter_cgal::applyFilter(RichParameterSet *){
    
    /// Dump data into CGAL-friendly format
    Point_3_list points;
    Vector3VertexProperty vpoint = mesh()->vertex_coordinates();
    foreach(Vertex v, mesh()->vertices()){
        Vector3& p = vpoint[v]; 
        points.push_back( Point_3(p.x(), p.y(), p.z()) );
    }
    
    /// Pre-Check
    if(points.size()<3)
        throw StarlabException("Dataset is too small");

    /// Compute hull
    Polyhedron_3 poly;
    CGAL::convex_hull_3(points.begin(), points.end(), poly);
    
    /// Post-Check
    if(poly.size_of_vertices()<3)
        throw StarlabException("Couldn't generate a solid convex hull");
    
    /// Convert back
    SurfaceMeshModel* chull = new SurfaceMeshModel("","Convex Hull");
    document()->addModel(chull);
    Polyhedron3_to_SurfaceMesh(poly, *chull);
    document()->setSelectedModel(chull);
}

