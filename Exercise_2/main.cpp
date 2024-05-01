#include "Utils.hpp"

using namespace std;

int main()
{
    string filepath = "PolygonalMesh";
    PolygonalMesh mesh;

    if(!importMesh(filepath, mesh))
    {
        cerr << "Errore: Importazione dei dati della mesh fallita." << endl;
        return 1;
    }



    return 0;
}
