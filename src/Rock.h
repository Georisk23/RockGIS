#ifndef ROCK_H
#define ROCK_H

#include "Terrain.h"
#include "Parameters.h"


class CRock
{
public:
//CONSTRUCTOR
	//CRock(){puts("CONSTRUIT_________________");};

//DESTRUCTOR
	~CRock(){};

//Definicio de les variables d'estat
	double Rcm[3], Vcm[3], Acm[3], Wcm[3];
	double Volume;
	double Radius;
	int Impact; //0=Fly, 1=Impact
	int Fragmented; //0=No, 1=YES
	int id; //Tracking number
	int pantalla;
	int xIndex;
	int yIndex;
	double CellEnergy;
	double time;

//Funcio per imprimir vector posicions
	void printCinematics(char* File,CTerrain& Terrain,int Precision,CParameters& Parameters,std::string &OutputToStore);

//Funcio per fer caure el bloc
	void rockfall(CTerrain& Terrain,CParameters& Parameters,std::string &Output_Trajectories,std::string &Output_Stoppages,std::string &Output_PASSAGES);

//adançar un dt
	void advance(double dt,CTerrain& Terrain,CParameters& Parameters);
	std::vector<double> advance2(double dt);

//actualitzar velocitat
	void updatespeed(CTerrain& Terrain,double Vnew[3] );

//alçada respecte MDT
	double height2ground(CTerrain& Terrain);
	double height2ground(CTerrain& Terrain,std::vector<double> Position);

//funcio parabola
	void fly(CTerrain& Terrain,CParameters& Parameters,std::string &Output_Trajectories,std::string &Output_PASSAGES);

//funciodeteccio impacte Bisection method
	double dt2impact(CTerrain& Terrain,CParameters& Parameters);

//funcio rebot
	double rebound(CTerrain& Terrain,CParameters& Parameters);
	double rebound2(CTerrain& Terrain,CParameters& Parameters);

//funcio Breakage que genera tots els fragments
	void Breakage(CTerrain& Terrain,CParameters& Parameter,std::string &Output_Trajectories,std::string &Output_Stoppages,std::string &Output_PASSAGES);

//funciuo citeri parada
	bool StopCriterion(CParameters& Parameters);

//funciuo citeri fragmentcio
	bool BreakageCriterion(CTerrain& Terrain,CParameters& Parameters);

//funcio per calcular volums en el metode de distribucio fragtal de la massa
	double Vol_f_N(double Lmax, int N, int k, double D);

//funcio que genera els fragments en funcio del metode escollit
	std::vector<double> MassDistribution(double InitialVolume,std::string MassDistMethod,CParameters& Parameters);

//funcio que genera vectors unitaris dins un con centrat en Z
	std::vector<double> UnitariInCone(double Obertura);

//funcio que rota un vector centrant l'eix generatriu a la velocitat de referencia
	std::vector<double> EulerRotateVector(std::vector<double> ReferenceVector,std::vector<double> RotatingVector);

//funció que comprova que el vector de breckage no es fica dins el terreny
	bool VectorInGround(CTerrain& Terrain,CParameters& Parameters,std::vector<double>);

};

#endif
