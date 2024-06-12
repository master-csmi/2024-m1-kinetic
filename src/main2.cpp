#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include "../IO/STL.h"
#include <iostream>
#include <fstream>
#include vector 


using KernelType = CGAL::Exact_predicates_exact_constructions_kernel;
    //using KernelType = CGAL::Exact_predicates_inexact_constructions_kernel;
using mesh_cgal_type = CGAL::Surface_mesh<KernelType::Point_3>;     

typedef KERNEL::Point_3 Point;



int main(int argc , char* argv){
    if (argc < 2){
        std::cerr << "give a stl file name " << std::endl;
        return EXIT_FAILURE;}

    const char * filename ;

    filename = argv[1];

    std::ifstream input(filename,std::ios::binary)
    if (!input){
        std::cerr << "Invalid file / cannot open "<< std::endl;
        return EXIT_FAILURE;
    }
    std::vector<KernelType::Point_3> points;
    std::vector<std::vector<std::size_t> > polygons; 

    std::cout << "Successfully read STL file: " << filename << std::endl;
    std::cout << "Number of points: " << points.size() << std::endl;
    std::cout << "Number of facets: " << facets.size() << std::endl;

    // Display the first few facets (change the number 5 to display more or fewer facets)
    size_t num_facets_to_display = std::min(facets.size(), static_cast<size_t>(5));
    for (size_t i = 0; i < num_facets_to_display; ++i) {
        std::cout << "Facet " << i << ":\n";
        for (size_t j = 0; j < facets[i].size(); ++j) {
            const Point& p = points[facets[i][j]];
            std::cout << "  Vertex " << j << ": (" << p.x() << ", " << p.y() << ", " << p.z() << ")\n";
        }}

    
    




    return 0;
}