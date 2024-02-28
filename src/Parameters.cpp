#ifndef PARAMETERS_CPP
#define PARAMETERS_CPP

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Parameters.h"

void CParameters::loadParam(std::string file) {

    std::ifstream fin(file.c_str());
    std::string line;

    //bucle sobre les linies del text
    while (getline(fin, line)) {
        std::istringstream sin(line.substr(line.find("=") + 1));

        //lectura individual dels parametres
        if (line.find("NumBlocks") != -1)
        {
            sin >> NumBlocks;
		}
		else if (line.find("TimeStep") != -1)
        {
            sin >> TimeStep;
		}
		else if (line.find("StopVelocity") != -1)
        {
            sin >> StopVelocity;
		}
		else if (line.find("Kna") != -1)
        {
            sin >> Kna;
		}
		else if (line.find("Knb") != -1)
		{
            sin >> Knb;
		}
		else if (line.find("Kta") != -1)
        {
            sin >> Kta;
		}
		else if (line.find("KtRough") != -1)
        {
            sin >> KtRough;
		}
		else if (line.find("DEM_path") != -1)
        {
            sin >> DEM_path;
		}
		else if (line.find("OBJECTS_path") != -1)
        {
            sin >> OBJECTS_path;
		}
		else if (line.find("Sources_path") != -1)
        {
            sin >> Sources_path;
		}
		else if (line.find("Trajectories_path") != -1)
        {
            sin >> Trajectories_path;
		}
		else if (line.find("Stoppages_path") != -1)
        {
            sin >> Stoppages_path;
		}
		else if (line.find("Passages_path") != -1)
        {
            sin >> Passages_path;
		}
		else if (line.find("Impacts_path") != -1)
        {
            sin >> Impacts_path;
		}
		else if (line.find("InitialSeed") != -1)
        {
            sin >> InitialSeed;
		}
		else if (line.find("Cone") != -1)
        {
            sin >> Cone;
		}
		else if (line.find("LimitAngle") != -1)
        {
            sin >> LimitAngle;
		}
		else if (line.find("DragLayerHeigh") != -1)
        {
            sin >> DragLayerHeigh;
		}
		else if (line.find("DragSpeedLimit") != -1)
        {
            sin >> DragSpeedLimit;
		}
		else if (line.find("DragCoeficient") != -1)
        {
            sin >> DragCoeficient;
		}
		else if (line.find("PrintMode") != -1)
        {
            sin >> PrintMode;
		}
		else if (line.find("RockDensity") != -1)
        {
            sin >> RockDensity;
		}
		else if (line.find("SlopeVar") != -1)
        {
            sin >> SlopeVar;
		}
		else if (line.find("AspectVar") != -1)
        {
            sin >> AspectVar;
		}
		else if (line.find("PowerB") != -1)
        {
            sin >> PowerB;
		}
		else if (line.find("PowerP") != -1)
        {
            sin >> PowerP;
		}
		else if (line.find("na1") != -1)
        {
            sin >> na1;
		}
		else if (line.find("na2") != -1)
        {
            sin >> na2;
		}
		else if (line.find("b1") != -1)
        {
            sin >> b1;
		}
		else if (line.find("b2") != -1)
        {
            sin >> b2;
		}
		else if (line.find("q1") != -1)
        {
            sin >> q1;
		}
		else if (line.find("q2") != -1)
        {
            sin >> q2;
		}
        
        
        
        
        else if (line.find("Ep1") != -1)
        {
            sin >> Ep1;
		}
        else if (line.find("Hp1") != -1)
        {
            sin >> Hp1;
		}
        else if (line.find("Ep2") != -1)
        {
            sin >> Ep2;
		}
        else if (line.find("Hp2") != -1)
        {
            sin >> Hp2;
		}
        else if (line.find("Ep3") != -1)
        {
            sin >> Ep3;
		}
        else if (line.find("Hp3") != -1)
        {
            sin >> Hp3;
		}
        
        
        
        
    else if (line.find("MinAreaRatio") != -1)
        {
            sin >> MinAreaRatio;
		}
    else if (line.find("VminFrag") != -1)
        {
            sin >> VminFrag;
    }
    else if (line.find("r_variant") != -1)
        {
            sin >> r_variant;
    }
		else if (line.find("StoreTrajectories") != -1)
        {
            sin >> StoreTrajectories;
		}

	}
}


void CParameters::printParam() {
	std::cout << NumBlocks << "\n";
	std::cout << TimeStep << "\n";
	std::cout << StopVelocity << "\n";
	std::cout << DEM_path << "\n";
}

#endif
