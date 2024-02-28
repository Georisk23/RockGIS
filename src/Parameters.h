#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string>

class CParameters
{
public:

//Primers parametres
    int    NumBlocks;
    double TimeStep;
    double StopVelocity;
    double Kna;
    double Knb;
    double Kta;
    double KtRough;
    int    InitialSeed;
	std::string DEM_path;
	std::string OBJECTS_path;
	std::string Sources_path;
	std::string Trajectories_path;
	std::string Stoppages_path;
	std::string Passages_path;
	std::string Impacts_path;
	double Cone;
	double LimitAngle;
	double DragLayerHeigh;
	double DragSpeedLimit;
	double DragCoeficient;
	int    PrintMode;
	double RockDensity;
	double SlopeVar;
	double AspectVar;
	double PowerB;
	double PowerP;
	double MinAreaRatio;
	double VminFrag;
	double r_variant;
	double na1;
	double na2;
	double b1;
	double b2;
	double q1;
	double q2;
	int StoreTrajectories;
	double damageP1;
	double damageP2;
	double damageP3;
    double Ep1;
    double Hp1;
    double Ep2;
    double Hp2;
    double Ep3;
    double Hp3;
    
//Funcio per carregar parametres
	void loadParam(std::string file);

//Funcio per visualitzar parametres
	void printParam();

};

#endif
