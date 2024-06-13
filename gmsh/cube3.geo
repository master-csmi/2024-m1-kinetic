// Gmsh project created on Tue Jun 11 14:48:20 2024
SetFactory("OpenCASCADE");
//+
Point(1) = {0, 0, 0, 1.0};
//+
Point(2) = {0, 1, 0, 1.0};
//+
Point(3) = {1, 1, 0, 1.0};
//+
Point(4) = {1, 0, 0, 1.0};
//+
Point(5) = {0, 0, 1, 1.0};
//+
Point(6) = {0, 1, 1, 1.0};
//+
Point(7) = {1, 1, 1, 1.0};
//+
Point(8) = {1, 0, 1, 1.0};
//+
Line(1) = {1,2};
Line(2) = {2,3};
Line(3) = {3,4};
Line(4) = {4,1};

Line(5) = {5,6};
Line(6) = {6,7};
Line(7) = {7,8};
Line(8) = {8,5};

Line(9) = {5,1};
Line(10) = {6,2};
Line(11) = {7,3};
Line(12) = {8,4};

// Create surfaces
Line Loop(1) = {1, 2, 3, 4};  // Bottom face
Plane Surface(1) = {1};

Line Loop(2) = {5, 6, 7, 8};  // Top face
Plane Surface(2) = {2};

Line Loop(3) = {1, 10, -5, -9};  // Front face
Plane Surface(3) = {3};

Line Loop(4) = {2, 11, -6, -10};  // Right face
Plane Surface(4) = {4};

Line Loop(5) = {3, 12, -7, -11};  // Back face
Plane Surface(5) = {5};

Line Loop(6) = {4, 9, -8, -12};  // Left face
Plane Surface(6) = {6};

// Create volume
Surface Loop(1) = {1, 2, 3, 4, 5, 6};
Volume(1) = {1};

// Mesh the volume
Mesh 3;

