#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <iomanip>	// setprecioson
#include <time.h>
#include <math.h>	// M_PI

#include "Rock.h"
#include "Terrain.h"
#include "Parameters.h"
#include "Source.h"

#include "Vec3.h"
#include "Mat3.h"

//Define number pi
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

clock_t tStart = clock();

bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

int main(int argc, char *argv[])
{

	//DISCLAIMER
	std::cout << "\n###################################################\n";
	std::cout << "\nRockGIS      A rockfall propagation code considering fragmentation\n\n";
	std::cout << "Universitat Politècnica de Catalunya. BarcelonaTech.\n";
	std::cout << "Department of Civil and Environmental Engineering. Geotechnical Engineering and Geosciences Division\n\n";
	std::cout << "\nThis is a research code under development, more information can be found on https://rockmodels.upc.edu\n\n";
	std::cout << "DISCLAIMER:\n";
	std::cout << "This software is provided by the copyright holders and contributors ‘as is’ and any express or implied warranties, including, but not limited to, the implied warranties of merchantability and fitness for a particular purpose are disclaimed. In no event shall the copyright owner or contributors be liable for any direct, indirect, incidental, special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or services; loss of use, data, or profits; or business interruption) however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of the use of this software, even if advised of the possibility of such damage.\n";
	std::cout << "\n\n###################################################\n\n";

	// Start execution
	std::cout << "  Execution started...\n";

	//Check if parameter file has been parsed on commandline
	if (argc == 1)
	{
		std::cout << "  FATAL ERROR: No parameter file parsed through command line! Exiting program.\n\n";
		return false;
	}

	//Check if parameter file does exist
	std::string parameters_file = argv[1]; //argv[1] is the name of parameter file parsed
	if (is_file_exist(parameters_file.c_str())==0)
	{
		std::cout << "  FATAL ERROR: Parameter file not found! Exiting program.\n\n";
		return false;
	}

	// Load parameter file
	std::cout << "  Loading parameters...\n";
	CParameters Parameters;
	Parameters.loadParam(parameters_file);
	//Parameters.printParam();

    // Create Terrain object
    std::cout << "  Loading terrain...\n";
    CTerrain Terrain;


    if (is_file_exist(Parameters.DEM_path.c_str())==0)
  	{
  		std::cout << "  FATAL ERROR: DEM file not found. Exiting program.\n\n";
  		return false;
  	}
    if (is_file_exist(Parameters.Sources_path.c_str())==0)
  	{
  		std::cout << "  FATAL ERROR: Sources file not found. Exiting program.\n\n";
  		return false;
  	}

    //Cerrega el MDE a terreny
    Terrain.loadTerrain(Parameters.DEM_path,Parameters.OBJECTS_path);

	//std::string Traject="Output/"+std::to_string(Parameters.CombID)+"/trejectories.csv";
	std::string Traject=Parameters.Trajectories_path;
	char *cstr = &Traject[0u];
    //std::cout <<"RockGIS running:  " << cstr <<"\n";

	//capçalera output
	std::ofstream outfile;
	outfile.open(cstr, std::ios::trunc);
	//outfile << "x;y;z;vx;vy;vz;volume\n";
	outfile << "x,y,z,velocity,volume,h2g,id,energy,t\n";
	outfile.close();

	//std::string Stop="Output/"+std::to_string(Parameters.CombID)+"/stoppages.csv";
    std::string Stop=Parameters.Stoppages_path;
	char *cstr2 = &Stop[0u];
    //std::cout << cstr2;

	std::ofstream outfile2;
	outfile2.open(cstr2, std::ios::trunc);
	outfile2 << "x,y,z,velocity,volume,h2g,id,energy,t\n";
	outfile2.close();
	
    std::string Pass=Parameters.Passages_path;
	char *cstr3 = &Pass[0u];
    std::string Output_Trajectories="x,y,z,velocity,volume,h2g,id,energy,t\n";
    std::string Output_Stoppages="x,y,z,velocity,volume,h2g,id,energy,t\n";
    std::string Output_PASSAGES="";
  
  
	//borrar fitxer pantalles
	std::string Imp=Parameters.Impacts_path;
	char *impacPath = &Imp[0u];
	std::ofstream outfileP;
	outfileP.open(impacPath, std::ios::trunc);
	outfileP << "IdP,x,y,h2g,energy,id,time,breakage,damage\n";
	outfileP.close();

  
	//Carrega les fonts
	std::vector<CSource> Sources;
	CSource AuxSources;
	Sources = AuxSources.LoadSources(Parameters);

	int id=1;


  std::cout << "  Computing trajectories...\n";

	for (int n=0; n<Sources.size(); n++)
	{
		//if(n!=0){std::cout<<"\e[A";}
		std::cout<<"N: "<<n<<"\n";
		if (Sources[n].NumBlocks == 0) //IBSD mode
		{
			for (int j=0; j<Sources[n].Volumes.size(); j++)
			{
				//std::cout << "> Propagation started. Source: " << n+1 << " / " << Sources.size() << "   Rockfall: " << j+1 << " / " << Sources[n].Volumes.size() << "\n";
				srand(Parameters.InitialSeed+((n+1)*(j+1)));
				double x = ((double) rand() / (RAND_MAX));
				CRock *Rock;
				Rock = new CRock{{Sources[n].Rcm[0],Sources[n].Rcm[1],Sources[n].Rcm[2]},{Sources[n].Vcm[0]+0.0*(1.0-(2.0*x)),Sources[n].Vcm[1],Sources[n].Vcm[2]},{0.0,0.0,-9.81},{0.0,0.0,0.0},Sources[n].Volumes[j],pow((Sources[n].Volumes[j]*3.0)/(4.0*M_PI),1.0/3.0),0,0,Sources[n].ID,0,0.0};
				id=id+1;

				float alturaMedida=Rock->height2ground(Terrain);
				Rock->Rcm[2]=Rock->Rcm[2]+(Sources[n].Rcm[2]-alturaMedida);
        
				Rock->rockfall(Terrain,Parameters,Output_Trajectories,Output_Stoppages,Output_PASSAGES);
					
				delete Rock;
			}
			//RESET SCREEN COUNTERS
			Parameters.damageP1=0.0;
			Parameters.damageP2=0.0;
			Parameters.damageP3=0.0;

		}
		else  // Single volume mode
		{
			for (int b=0; b<Sources[n].NumBlocks; b++)
			{
				srand(Parameters.InitialSeed+((n+1)*(b+1)));
				double x = ((double) rand() / (RAND_MAX));
				
				for (int j=0; j<Sources[n].Volumes.size(); j++)
				{
					//std::cout << "> Propagation started. Source: " << n+1 << " / " << Sources.size() << "   Rockfall: " << j+1 << " / " << Sources[n].Volumes.size() << "\n";
					CRock *Rock;
					Rock = new CRock{{Sources[n].Rcm[0],Sources[n].Rcm[1],Sources[n].Rcm[2]},{Sources[n].Vcm[0]+0.0*(1.0-(2.0*x)),Sources[n].Vcm[1],Sources[n].Vcm[2]},{0.0,0.0,-9.81},{0.0,0.0,0.0},Sources[n].Volumes[j],pow((Sources[n].Volumes[j]*3.0)/(4.0*M_PI),1.0/3.0),0,0,Sources[n].ID,0,0.0};
					id=id+1;

			float alturaMedida=Rock->height2ground(Terrain);
			Rock->Rcm[2]=Rock->Rcm[2]+(Sources[n].Rcm[2]-alturaMedida);
			
					Rock->rockfall(Terrain,Parameters,Output_Trajectories,Output_Stoppages,Output_PASSAGES);
					delete Rock;

				}
			}
		}

	}



std::cout << "  Storing results...\n";

outfile.open(cstr, std::ios::trunc);
//outfile << "x;y;z;vx;vy;vz;volume\n";
outfile << Output_Trajectories;
outfile.close();

outfile.open(cstr2, std::ios::trunc);
//outfile << "x;y;z;vx;vy;vz;volume\n";
outfile << Output_Stoppages;
outfile.close();

outfile.open(cstr3, std::ios::trunc);
//outfile << "x;y;z;vx;vy;vz;volume\n";
outfile << Output_PASSAGES;
outfile.close();

std::cout << "  DONE!!!\n\n";

    return 0;
}
