#ifndef SOURCE_H
#define SOURCE_H

#include "Terrain.h"
#include "Parameters.h"


class CSource
{
public:

//DESTRUCTOR
	~CSource(){};

//Definicio de les variables d'estat
	double Rcm[3], Vcm[3];
	int NumBlocks;
	std::vector<double> Volumes;
	int ID;

//Funcions
std::vector<CSource> LoadSources(CParameters& Parameters);


};

#endif
