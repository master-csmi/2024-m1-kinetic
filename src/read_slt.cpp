#include <CGAL/Simple_cartesian.h>
#include "../IO/STL/STL_reader.h"
#include "../IO/STL.h"
#include <fstream>
#include <vector>

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

    std::cout << "Successfully read STL file: " << filename << std::endl;
    std::cout << "Number of points: " << points.size() << std::endl;
    std::cout << "Number of facets: " << facets.size() << std::endl;

    return EXIT_SUCCESS;
}
