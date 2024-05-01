#ifndef POLYGONAL_MESH_HPP
#define POLYGONAL_MESH_HPP

#include "Eigen/Eigen"
#include <vector>
#include <map>

using namespace Eigen;
using namespace std;

struct PolygonalMesh
{
    unsigned int numCell0D = 0;
    vector<unsigned int> cell0DIds = {};
    vector<Vector2d> cell0DCoordinates = {};
    map<unsigned int, vector<unsigned int>> cell0DMarkers = {};

    unsigned int numCell1D = 0;
    vector<unsigned int> cell1DIds = {};
    vector<Vector2i> cell1DVertices = {};
    map<unsigned int, vector<unsigned int>> cell1DMarkers = {};

    unsigned int numCell2D = 0;
    vector<unsigned int> cell2DIds = {};
    vector<vector<unsigned int>> cell2DVertices = {};
    vector<vector<unsigned int>> cell2DEdges = {};
    map<unsigned int, vector<unsigned int>> cell2DMarkers = {};
};

#endif // POLYGONAL_MESH_HPP
