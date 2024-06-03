#include <CGAL/Simple_cartesian.h>
#include "../IO/STL/STL_reader.h"
#include "../IO/STL.h"
#include <fstream>
#include <vector>
#include <CGAL/property_map.h>
#include <CGAL/IO/read_xyz_points.h>
#include <CGAL/IO/write_xyz_points.h>
typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point;


int main(int argc, char* argv[]) {

    const char* filename;

    if (argc != 2) {
        filename = "../Kinetic_exemple/ACJasmin/mesh.stl";
    }
    
    else {
    filename = argv[1];
    }
    
    
    std::ifstream input(filename, std::ios::binary);
    if (!input) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<Point> points;
    std::vector<std::vector<std::size_t>> facets;

    if (!CGAL::IO::read_STL(input, points, facets)) {
        std::cerr << "Error: invalid STL file" << std::endl;
        return EXIT_FAILURE;
    }

    for(int i=0;i<10;i++)
    {
        std::cout<<facets.back().back()<<std::endl;
        facets.back().pop_back();
    }
    if(!CGAL::IO::write_XYZ("essaie.xyz", points))
    
    return EXIT_SUCCESS;
}