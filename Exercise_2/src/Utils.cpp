#include "Utils.hpp"
#include <fstream>
#include <sstream>
#include <locale>
#include <vector>
#include <cmath>

using namespace std;




//Definizione della tolleranza per confronto tra double
const double EPSILON = 1e-10;
const double EPSILON_SQUARED = EPSILON * EPSILON;


//Funzione per calcolare la lunghezza di un lato dato due vertici
double calculateEdgeLength(const Vector2d& vertex1, const Vector2d& vertex2) {
    double dx = vertex2.x() - vertex1.x();
    double dy = vertex2.y() - vertex1.y();
    return sqrt(dx * dx + dy * dy);
}

//controllo del lato
bool checkEdgeLengths(const PolygonalMesh& mesh) {
    for (size_t c = 0; c < mesh.numCell2D; ++c) {
        const vector<unsigned int>& edges = mesh.cell2DEdges[c];

        for (unsigned int e : edges) {
            const unsigned int origin = mesh.cell1DVertices[e][0];
            const unsigned int end = mesh.cell1DVertices[e][1];

            const Vector2d& vertex1 = mesh.cell0DCoordinates[origin];
            const Vector2d& vertex2 = mesh.cell0DCoordinates[end];

            double edgeLength = calculateEdgeLength(vertex1, vertex2);
            if (edgeLength < EPSILON) {
                cerr << "Lato con lunghezza nulla" << endl;
                return false;
            }
        }
    }
    return true;
}
//Funzione per calcolare l'area del poligono
double calculatePolygonArea(const vector<Vector2d>& vertices) {
    double area = 0.0;
    int numVertices = vertices.size();

    for (int i = 1; i < numVertices-1; ++i) {
        int j = (i + 1);
        area +=(0.5)*(( vertices[i].x()-vertices[0].x()) * (vertices[j].y()-vertices[0].y()) - (vertices[j].x() - vertices[0].x()) * (vertices[i].y()-vertices[0].y()));
    }


    return fabs(area); //val assoluto area
}

//controllo area poligono
bool checkPolygonAreaNonZero(const PolygonalMesh& mesh) {
    for (size_t c = 0; c < mesh.numCell2D; ++c) {
        const vector<unsigned int>& vertexIds = mesh.cell2DVertices[c];
        vector<Vector2d> vertices;

        for (unsigned int vertexId : vertexIds) {
            vertices.push_back(mesh.cell0DCoordinates[vertexId]);
        }

        double area = calculatePolygonArea(vertices);
        if (area < EPSILON_SQUARED) {
            cerr << "Poligono con area nulla o troppo piccola" <<c << endl;
            return false;
        }
    }
    return true;
}


bool importMesh(const string& filepath, PolygonalMesh& mesh)
{
    if(!importCell0Ds(filepath + "/Cell0Ds.csv", mesh))
    {
        return false;
    }
    else
    {
        // Stampa le informazioni sulle celle 0D
        cout << "Informazioni sulle celle 0D:" << endl;
        for(auto it = mesh.cell0DMarkers.begin(); it != mesh.cell0DMarkers.end(); it++)
        {
            cout << "Marker: " << it->first << ", Celle: ";
            for(const unsigned int id : it->second)
                cout << id << " ";
            cout << endl;
        }
    }

    if(!importCell1Ds(filepath + "/Cell1Ds.csv", mesh))
    {
        return false;
    }
    else
    {
        // Stampa le informazioni sulle celle 1D
        cout << "Informazioni sulle celle 1D:" << endl;
        for(auto it = mesh.cell1DMarkers.begin(); it != mesh.cell1DMarkers.end(); it++)
        {
            cout << "Marker: " << it->first << ", Celle: ";
            for(const unsigned int id : it->second)
                cout << id << " ";
            cout << endl;
        }
    }

    if(!importCell2Ds(filepath + "/Cell2Ds.csv", mesh))
    {
        return false;
    }
    else
    {
        // Stampa le informazioni sulle celle 2D
        cout << "Informazioni sulle celle 2D:" << endl;
        for(size_t i = 0; i < mesh.numCell2D; ++i)
        {
            cout << "ID: " << mesh.cell2DIds[i] << ", Vertici: ";
            for(const auto& vertex : mesh.cell2DVertices[i])
            {
                cout << vertex << " ";
            }
            cout << ", Lati: ";
            for(const auto& edge : mesh.cell2DEdges[i])
            {
                cout << edge << " ";
            }
            cout << endl;



        }
        //Test lati non-zero
        if (!checkEdgeLengths(mesh)) {
            cerr << "Errore: almeno un lato della mesh ha lunghezza nulla o troppo piccola." << endl;
            return false;
        }
        //Test aree non-zero
        if (!checkPolygonAreaNonZero(mesh)) {
            cerr << "Errore: Almeno uno dei poligoni ha un'area nulla o troppo piccola." << endl;
            return false;
        }


    }

    return true;
}



bool importCell0Ds(const string& filename, PolygonalMesh& mesh)
{
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Errore: Impossibile aprire il file " << filename << endl;
        return false;
    }

    // Ignora la riga di intestazione
    string header;
    getline(file, header);

    list<string> listLines;
    string line;
    while (getline(file, line)) {
        listLines.push_back(line);
    }

    file.close();

    mesh.numCell0D = listLines.size();

    if (mesh.numCell0D == 0) {
        cerr << "Non ci sono celle 0D nel file" << endl;
        return false;
    }

    mesh.cell0DIds.reserve(mesh.numCell0D);
    mesh.cell0DCoordinates.reserve(mesh.numCell0D);

    for (const string& line : listLines) {
        istringstream converter(line);

        unsigned int id, marker;
        double x, y;
        char delimiter;

        // Legge i valori separati dal punto e virgola
        if (!(converter >> id >> delimiter >> marker >> delimiter >> x >> delimiter >> y)) {
            cerr << "Errore: Dati non validi nel file " << filename << endl;
            return false;
        }

        mesh.cell0DIds.push_back(id);
        mesh.cell0DCoordinates.push_back(Vector2d(x, y));

        if (marker != 0) {
            mesh.cell0DMarkers[marker].push_back(id);
        }
    }

    return true;
}

bool importCell1Ds(const string &filename, PolygonalMesh& mesh)
{
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Errore: Impossibile aprire il file " << filename << endl;
        return false;
    }

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front(); // Ignora la riga di intestazione

    mesh.numCell1D = listLines.size();

    if (mesh.numCell1D == 0)
    {
        cerr << "Non ci sono celle 1D nel file" << endl;
        return false;
    }

    mesh.cell1DIds.reserve(mesh.numCell1D);
    mesh.cell1DVertices.reserve(mesh.numCell1D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id, marker, origin, end;
        char delimiter;

        // Legge i valori dalla riga utilizzando il delimitatore punto e virgola
        if (!(converter >> id >> delimiter >> marker >> delimiter >> origin >> delimiter >> end))
        {
            cerr << "Errore: Dati non validi nel file " << filename << endl;
            return false;
        }

        mesh.cell1DIds.push_back(id);
        mesh.cell1DVertices.push_back(Vector2i(origin, end));

        if (marker != 0)
        {
            mesh.cell1DMarkers[marker].push_back(id);
        }
    }

    return true;
}


bool importCell2Ds(const string& filename, PolygonalMesh& mesh) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Errore: Impossibile aprire il file " << filename << endl;
        return false;
    }

    list<string> lines;
    string line;

    //Legge tutte le righe e le memorizza nella lista
    while (getline(file, line)) {
        lines.push_back(line);
    }

    file.close();

    //Ignora le intestazioni della prima riga
    lines.pop_front();


    for (const string& line : lines) {
        istringstream iss(line);

        unsigned int id, marker, numVertices, numEdges;
        char separator; //separatore tra i numeri
        vector<unsigned int> vertices, edges;

        //Legge l'ID e il marker
        iss >> id >> separator >> marker >> separator;

        //numero di vertici e  vertici
        iss >> numVertices >> separator;
        for (unsigned int i = 0; i < numVertices; ++i) {
            unsigned int vertexId;
            iss >> vertexId >> separator;
            vertices.push_back(vertexId);
        }

        //numero di lati e lati
        iss >> numEdges >> separator;
        for (unsigned int i = 0; i < numEdges; ++i) {
            unsigned int edgeId;
            iss >> edgeId >> separator;
            edges.push_back(edgeId);
        }

        //Aggiunta dei dati letti alla PolygonalMesh
        mesh.numCell2D++;
        mesh.cell2DIds.push_back(id);
        mesh.cell2DVertices.push_back(vertices);
        mesh.cell2DEdges.push_back(edges);
        mesh.cell2DMarkers[id] = {marker};
    }

    return true;
}
