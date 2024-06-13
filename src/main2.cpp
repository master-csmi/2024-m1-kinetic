#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/self_intersections.h>
#include <CGAL/draw_surface_mesh.h>
#include <CGAL/Surface_mesh.h>
#include "../IO/STL.h"
#include <iostream>
#include <fstream>
#include <vector>

#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Polygon_mesh_processing/border.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point;
typedef Kernel::Triangle_3 Triangle;
typedef CGAL::Surface_mesh<Point> mesh_cgal_type;

typedef boost::graph_traits<mesh_cgal_type>::halfedge_descriptor        halfedge_descriptor;
typedef boost::graph_traits<mesh_cgal_type>::edge_descriptor            edge_descriptor;

namespace PMP = CGAL::Polygon_mesh_processing;

int main(int argc , char* argv[]){
    if (argc < 2){
        std::cerr << "give a stl file name " << std::endl;
        return EXIT_FAILURE;}

    const char * filename ;

    filename = argv[1];

    std::ifstream input(filename,std::ios::binary);
    if (!input){
        std::cerr << "Invalid file / cannot open "<< std::endl;
        return EXIT_FAILURE;
    }
    std::vector<Point> points;
    std::vector<std::vector<std::size_t> > polygone; 

    if (!CGAL::IO::read_STL(input,points,polygone)){
        std::cerr << "Cannot open file" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Successfully read STL file: " << filename << std::endl;
    std::cout << "Number of points: " << points.size() << std::endl;
    std::cout << "Number of polygone: " << polygone.size() << std::endl;

    // Display the first few facets 
    size_t num_facets_to_display = std::min(polygone.size(), static_cast<size_t>(5));
    for (size_t i = 0; i < num_facets_to_display; ++i) {
        std::cout << "polygone " << i << ":\n";
        for (size_t j = 0; j < polygone[i].size(); ++j) {
            const Point& p = points[polygone[i][j]];
            std::cout << "  Vertex " << j << ": (" << p.x() << ", " << p.y() << ", " << p.z() << ")\n";
        }}


    if(!CGAL::Polygon_mesh_processing::orient_polygon_soup(points,polygone)){
        std::cerr <<"Orientation failed"<< std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "orientation succeed" <<std::endl ;

    if(!CGAL::Polygon_mesh_processing::is_polygon_soup_a_polygon_mesh(polygone)){
        std::cerr << " not a polygon mesh " << std::endl;}
    else
    std::cout << " is a polygon mesh ";

    // recherche pour afficher le CGALMESH 
    mesh_cgal_type meshCgal;
        CGAL::Polygon_mesh_processing::polygon_soup_to_polygon_mesh( points, polygone, meshCgal );
        // que des Tmesh ?
        //bool selfIntersect = CGAL::Polygon_mesh_processing::does_self_intersect( meshCgal );
        bool isClosed = CGAL::is_closed( meshCgal );
        //if ( selfIntersect )
        //{
          //  std::cout << "does_self_intersect";
            
        //}
        if ( isClosed )
        {
            std::cout << "CGAL::is_closed";
            
        }

double target_edge_length = (argc > 2) ? std::stod(std::string(argv[2])) : 0.04;

PMP::isotropic_remeshing(faces(meshCgal), target_edge_length, meshCgal);

CGAL::IO::write_polygon_mesh("out.off",meshCgal,CGAL::parameters::stream_precision(17));
    return 0;
}