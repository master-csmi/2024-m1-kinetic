#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <IO/STL/STL_reader.h>
#include <fstream>
#include <vector>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point;
typedef CGAL::Polyhedron_3<Kernel> Polyhedron;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file.stl>" << std::endl;
        return EXIT_FAILURE;
    }

    const char* filename = argv[1];
    std::ifstream input(filename, std::ios::binary);
    if (!input) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<Point> points;
    std::vector<std::vector<std::size_t>> facets;

    if (!CGAL::read_STL(input, points, facets)) {
        std::cerr << "Error: invalid STL file" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Successfully read STL file: " << filename << std::endl;
    std::cout << "Number of points: " << points.size() << std::endl;
    std::cout << "Number of facets: " << facets.size() << std::endl;

    // Optionally, build a polyhedron
    Polyhedron P;
    CGAL::build_from_polygon_soup(points, facets, P);

    std::cout << "Polyhedron built with " << P.size_of_vertices() << " vertices, "
              << P.size_of_facets() << " facets." << std::endl;

    return EXIT_SUCCESS;
}
