#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Kinetic_surface_reconstruction_3.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/Point_set_3/IO.h>
#include <CGAL/Real_timer.h>
#include <CGAL/IO/PLY.h>
#include <CGAL/IO/polygon_soup_io.h>
#include <CGAL/pca_estimate_normals.h>
#include <CGAL/mst_orient_normals.h>
#include <CGAL/bounding_box.h>
#include <sstream>

#include "include/Parameters.h"
#include "include/Terminal_parser.h"

using Kernel    = CGAL::Exact_predicates_inexact_constructions_kernel;
using FT        = typename Kernel::FT;
using Point_3   = typename Kernel::Point_3;
using Vector_3  = typename Kernel::Vector_3;
using Segment_3 = typename Kernel::Segment_3;

using Point_set    = CGAL::Point_set_3<Point_3>;
using Point_map    = typename Point_set::Point_map;
using Normal_map   = typename Point_set::Vector_map;

using KSR = CGAL::Kinetic_surface_reconstruction_3<Kernel, Point_set, Point_map, Normal_map>;

using Parameters      = CGAL::KSR::All_parameters<FT>;
using Terminal_parser = CGAL::KSR::Terminal_parser<FT>;
using Timer = CGAL::Real_timer;

typedef CGAL::Simple_cartesian<double> Kernel2;
typedef Kernel2::Point_3 Point;

template <typename T>
std::string to_stringp(const T a_value, const int n = 6)
{
  std::ostringstream out;
  out.precision(n);
  out << std::fixed << a_value;
  return out.str();
}

void parse_terminal(Terminal_parser& parser, Parameters& parameters) {
  // Set all parameters that can be loaded from the terminal.
  // add_str_parameter  - adds a string-type parameter
  // add_val_parameter  - adds a scalar-type parameter
  // add_bool_parameter - adds a boolean parameter

  std::cout << std::endl;
  std::cout << "--- INPUT PARAMETERS: " << std::endl;

  parser.add_str_parameter("-data", parameters.data);
  
  // Shape detection.
  parser.add_val_parameter("-kn"   , parameters.k_neighbors); // pas important 
  parser.add_val_parameter("-dist" , parameters.maximum_distance); // important pour le niveau de precision epsilon
  parser.add_val_parameter("-angle", parameters.maximum_angle); // pas tres important
  parser.add_val_parameter("-minp" , parameters.min_region_size); // important  sigma 


  // Shape regularization.
  parser.add_bool_parameter("-regparallel", parameters.regparallel);
  parser.add_bool_parameter("-regcoplanar", parameters.regcoplanar);
  parser.add_bool_parameter("-regorthogonal", parameters.regorthogonal);
  parser.add_bool_parameter("-regsymmetric", parameters.regsymmetric);

  parser.add_val_parameter("-regoff", parameters.maximum_offset);
  parser.add_val_parameter("-regangle", parameters.angle_tolerance);

  // Shape regularization.
  parser.add_bool_parameter("-reorient", parameters.reorient);

  // Partitioning.
  parser.add_val_parameter("-k", parameters.k_intersections);
  parser.add_val_parameter("-odepth", parameters.max_octree_depth);
  parser.add_val_parameter("-osize", parameters.max_octree_node_size);

  // Reconstruction.
  parser.add_val_parameter("-lambda", parameters.graphcut_beta);
  parser.add_val_parameter("-ground", parameters.use_ground);

  // Debug.
  parser.add_bool_parameter("-debug", parameters.debug);
  parser.add_bool_parameter("-stl", parameters.stl);

  // Verbose.
  parser.add_bool_parameter("-verbose", parameters.verbose);
}

int main(const int argc, const char** argv) {
  // Parameters.
  std::cout.precision(20);
  std::cout << std::endl;
  std::cout << "--- PARSING INPUT: " << std::endl;
  const auto kernel_name = boost::typeindex::type_id<Kernel>().pretty_name();
  std::cout << "* used kernel: " << kernel_name << std::endl;
  const std::string path_to_save = "";
  Terminal_parser parser(argc, argv, path_to_save);

  Parameters parameters;
  parse_terminal(parser, parameters);

  // If no input data is provided, use input from data directory.
  if (parameters.data.empty())
    parameters.data = "data data/flame.ply";

  if(parameters.stl)
  {
    std::ifstream input(parameters.data, std::ios::binary);
    if (!input) {
        std::cerr << "Error: cannot open file " << parameters.data << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<Point> points;
    std::vector<std::vector<std::size_t>> facets;

    if (!CGAL::IO::read_STL(input, points, facets)) {
        std::cerr << "Error: invalid STL file" << std::endl;
        return EXIT_FAILURE;
    }
    if(!CGAL::IO::write_XYZ(parameters.data+".xyz", points))
    {
      std::cerr << "Error: invalid xyz file" << std::endl;
        return EXIT_FAILURE;
    }
    parameters.data+=".xyz";
  }
  // Input.
  Point_set point_set(parameters.with_normals);
  std::cout<<parameters.data<<std::endl;
  CGAL::IO::read_point_set(parameters.data, point_set);

  if (point_set.size() == 0) {
    std::cout << "input file not found or empty!" << std::endl;
    return EXIT_FAILURE;
  }

  if (!point_set.has_normal_map()) {
    point_set.add_normal_map();
    CGAL::pca_estimate_normals<CGAL::Parallel_if_available_tag>(point_set, 9);
    CGAL::mst_orient_normals(point_set, 9);
  }

  for (std::size_t i = 0; i < point_set.size(); i++) {
    Vector_3 n = point_set.normal(i);
    if (abs(n * n) < 0.05)
      std::cout << "point " << i << " does not have a proper normal" << std::endl;
  }

  if (parameters.maximum_distance == 0) {
    CGAL::Bbox_3 bbox = CGAL::bbox_3(CGAL::make_transform_iterator_from_property_map(point_set.begin(), point_set.point_map()),
      CGAL::make_transform_iterator_from_property_map(point_set.end(), point_set.point_map()));

    FT d = CGAL::approximate_sqrt
    ((bbox.xmax() - bbox.xmin()) * (bbox.xmax() - bbox.xmin())
      + (bbox.ymax() - bbox.ymin()) * (bbox.ymax() - bbox.ymin())
      + (bbox.zmax() - bbox.zmin()) * (bbox.zmax() - bbox.zmin()));

    parameters.maximum_distance = d * 0.03;
  }

  if (parameters.min_region_size == 0)
    parameters.min_region_size = static_cast<std::atomic_size_t>(point_set.size() * 0.01);

  std::cout << std::endl;
  std::cout << "--- INPUT STATS: " << std::endl;
  std::cout << "* number of points: " << point_set.size() << std::endl;

  std::cout << "verbose " << parameters.verbose << std::endl;
  std::cout << "maximum_distance " << parameters.maximum_distance << std::endl;
  std::cout << "maximum_angle " << parameters.maximum_angle << std::endl;
  std::cout << "min_region_size " << parameters.min_region_size << std::endl;
  std::cout << "k " << parameters.k_intersections << std::endl;
  std::cout << "graphcut_beta " << parameters.graphcut_beta << std::endl;

  std::cout << parameters.regorthogonal << " " << parameters.regsymmetric << std::endl;

  auto param = CGAL::parameters::maximum_distance(parameters.maximum_distance)
    .maximum_angle(parameters.maximum_angle)
    .k_neighbors(parameters.k_neighbors)
    .minimum_region_size(parameters.min_region_size)
    .debug(parameters.debug)
    .verbose(parameters.verbose)
    .max_octree_depth(parameters.max_octree_depth)
    .max_octree_node_size(parameters.max_octree_node_size)
    .reorient_bbox(parameters.reorient)
    .regularize_parallelism(parameters.regparallel)
    .regularize_coplanarity(parameters.regcoplanar)
    .regularize_orthogonality(parameters.regorthogonal)
    .regularize_axis_symmetry(parameters.regsymmetric)
    .angle_tolerance(parameters.angle_tolerance)
    .maximum_offset(parameters.maximum_offset);

  // Algorithm.
  KSR ksr(point_set, param);
  std::cout<<"before detect shape"<<std::endl;
  Timer timer;
  timer.start();
  std::size_t num_shapes = ksr.detect_planar_shapes(param);

  std::cout << num_shapes << " detected planar shapes" << std::endl;

  FT after_shape_detection = timer.time();
  std::cout<<"before last partition and after detect shape"<<std::endl;
  ksr.initialize_partition(param);

  FT after_init = timer.time();

  ksr.partition(parameters.k_intersections);

  FT after_partition = timer.time();

  std::vector<Point_3> vtx;
  std::vector<std::vector<std::size_t> > polylist;

  std::map<typename KSR::KSP::Face_support, bool> external_nodes;

  external_nodes[KSR::KSP::Face_support::ZMIN] = true;
  std::cout<<"before reconstruct and after partition"<<std::endl;
  if (parameters.use_ground)
    ksr.reconstruct_with_ground(parameters.graphcut_beta, std::back_inserter(vtx), std::back_inserter(polylist));
  else
    ksr.reconstruct(parameters.graphcut_beta, external_nodes, std::back_inserter(vtx), std::back_inserter(polylist));

  FT after_reconstruction = timer.time();

  if (polylist.size() > 0)
    CGAL::IO::write_polygon_soup("resultat/polylist_" + std::to_string(parameters.graphcut_beta) + (parameters.use_ground ? "_g" : "_") + ".off", vtx, polylist);

  timer.stop();
  const FT time = static_cast<FT>(timer.time());

  std::vector<FT> lambdas{0.3, 0.5};

  for (FT l : lambdas) {
    if (l == parameters.graphcut_beta)
      continue;

    vtx.clear();
    polylist.clear();

    
    if (parameters.use_ground)
      ksr.reconstruct_with_ground(l, std::back_inserter(vtx), std::back_inserter(polylist));
    else
      ksr.reconstruct(l, external_nodes, std::back_inserter(vtx), std::back_inserter(polylist));

    std::cout<<"before last write and after reconstruct"<<std::endl;
    if (polylist.size() > 0)
      CGAL::IO::write_polygon_soup("resultat/polylist_P" + std::to_string(l) + (parameters.use_ground ? "_g" : "_") + ".off", vtx, polylist);
  }

  std::cout << "Shape detection:        " << after_shape_detection << " seconds!" << std::endl;
  std::cout << "Kinetic partition:      " << (after_partition - after_shape_detection) << " seconds!" << std::endl;
  std::cout << " initialization:        " << (after_init - after_shape_detection) << " seconds!" << std::endl;
  std::cout << " partition:             " << (after_partition - after_init) << " seconds!" << std::endl;
  std::cout << "Kinetic reconstruction: " << (after_reconstruction - after_partition) << " seconds!" << std::endl;
  std::cout << "Total time:             " << time << " seconds!" << std::endl << std::endl;

  return EXIT_SUCCESS;
}
