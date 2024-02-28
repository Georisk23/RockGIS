#ifndef RASTER_H
#define RASTER_H

#include <string>

using namespace std;
class CTerrain;
class CLecturaDelModel
{
public:

	CLecturaDelModel();
 

//Definició de les variables d'estat
	int nrows, ncols;
	double xllcenter, yllcenter, cellsize, nodatavalue, yulcenter;


//Funció per carregar el MDE d'un ASCII grid a una matriu!
    void loadTerrain(CTerrain *t, std::string MDEfilename, std::string ObjectsFilename);
    
	friend class CTerrain;
};

#endif
