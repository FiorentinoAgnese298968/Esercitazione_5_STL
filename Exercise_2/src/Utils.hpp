#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;


bool importMesh(const string& filepath, PolygonalMesh& mesh);

bool importCell0Ds(const string& filepath, PolygonalMesh& mesh);

bool importCell1Ds(const string& filepath, PolygonalMesh& mesh);

bool importCell2Ds(const string& filepath, PolygonalMesh& mesh);

#endif // UTILS_HPP

