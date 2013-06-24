#pragma once

/// Conversion class
class Polyhedron3_to_SurfaceMesh{   
public:
    /// For constructor use: i.e. Polyhedron3_to_SurfaceMesh(poly, mesh);
    Polyhedron3_to_SurfaceMesh(Polyhedron_3& poly, Surface_mesh& mesh){
        this->operator ()(poly, mesh);
    }
    /// For functor use: i.e. std::for_each
    Polyhedron3_to_SurfaceMesh(){}
    void operator()(Polyhedron_3& _poly, Surface_mesh& _mesh){
        std::for_each(_poly.facets_begin(), _poly.facets_end(), Polygon_inserter(_poly, _mesh));
    }
    
private:
    class Polygon_inserter{
    private:
        typedef Polyhedron_3::Halfedge_around_facet_circulator Halfedge_facet_circulator;        
        Polyhedron_3& _poly;
        Surface_mesh& _mesh;
    
    public:
        Polygon_inserter(Polyhedron_3& poly, Surface_mesh& mesh) : 
            _poly(poly), _mesh(mesh){
            
            /// Clear the mesh
            mesh.clear();

            std::size_t i = 0; 
            Polyhedron_3::Vertex_iterator it;
            for(it = _poly.vertices_begin(); it!=_poly.vertices_end(); it++){
                /// Give indexes to each vertex
                /// @see http://cgal-discuss.949826.n4.nabble.com/Getting-facet-indexes-from-polyhedron-3-td4553195.html
                it->id() = i++; 
                
                /// And insert them into the mesh
                Point_3& p = it->point();    
                Surface_mesh::Vertex v = mesh.add_vertex( Surface_mesh::Point(p.x(),p.y(),p.z()) );
                
                /// Make sure indexes match
                assert(v.idx() == (int) it->id());
            }
            
            // qDebug("Created %d vertices", mesh.n_vertices());
        }
        
        void operator()(Polyhedron_3::Facet& f){
            // qDebug("F[?]");   

            /// Stick all polyhedron vertices in a simple std::vector (similar to IO_obj.cpp)
            Halfedge_facet_circulator j = f.facet_begin();
            std::vector<Surface_mesh::Vertex> vertices;
            do{
                // qDebug() << "found vertex " << j->vertex()->id();   
                vertices.push_back( Surface_mesh::Vertex(j->vertex()->id()) );
            }
            while ( ++j != f.facet_begin());
                       
            /// And then add a face to the mesh
            _mesh.add_face( vertices ); 
        }
    };    
};
