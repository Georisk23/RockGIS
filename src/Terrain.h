#ifndef TERRAIN_H
#define TERRAIN_H

#include <string>
#include "Raster.h"
#include "Vec3.h"



using namespace std;

enum method {NEIGHBOUR, BILINEAR, BICUBIC};

class CTerrain
{
public:

// funcions públiques (interfície de l'usuari)
	CTerrain();									// Constructor estàndard.
	CTerrain(std::string TerrainFilename,std::string ObjectsFilename);		// Constructor si donem el nom del fitxer.
	 
	double Aspect(double X, double Y, int method=BILINEAR );	// Retorna el valor de l'Aspect per al punt de coordenades X,Y.
	double Height( double X, double Y, int method=BILINEAR );// Retorna el valor de Height per al punt de coordenades X,Y.
	double Slope(double X, double Y, int method=BILINEAR);   // Retorna el valor de l'Slope per al punt de coordenades X,Y.
	bool   SlopeAndAngle(double X, double Y, int method, double* slope, double* angle );//, double* dx, double* dy);
    void   loadTerrain(std::string MDEfilename, std::string ObjectsFilename); 	// Funció per carregar un model. Destrueix l'existent i carrega el nou.
	CVec3 NormalVector(double X, double Y);
	int xIndex(double X, double Y);
	int yIndex(double X, double Y);
// variables privades
// Definició de les variables d'estat
private:
	int nrows, ncols;
	double xllcenter, yllcenter, cellsize, nodatavalue, yulcenter;

	CLecturaDelModel model;
	float *mdt;
	float *aspect;
	float *slope;
    
//Funcions privades
    void printMDT();
    void printASPECT();
    void printError(char const * qui);

	friend class CLecturaDelModel; 

};

#endif
