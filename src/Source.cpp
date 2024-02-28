#ifndef SOURCE_CPP
#define SOURCE_CPP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Source.h"

std::vector<CSource> prova()
{
}

std::vector<CSource> CSource::LoadSources(CParameters& Parameters)
{
	//std::cout << "Loading sources...\n\n";

	std::vector<CSource> FONTS;

	CSource CurrentSource;

	//std::ifstream fin(file.c_str());
	std::ifstream fin(Parameters.Sources_path);
	std::string line;

	//bucle sobre les linies del text
	while (getline(fin, line)) {
		std::istringstream sin(line.substr(line.find("=") + 1));

		if(line[0]=='#' and line[1]=='#')
		{
			//std::cout <<"\nFont starts here\n"; //Aqui es comença una font
			continue;
		}
		if(line[0]=='%' and line[1]=='%')
		{
			FONTS.push_back(CurrentSource);
			//std::cout <<"Font ends here\n"; //Aqui s'acaba una font
			continue;
		}
		if(line[0] == 0)
		{
			//std::cout <<"Espai blanc\n";
			continue;
		}

		std::istringstream parametre(line.substr(line.find("=") + 1));

		if (line.find("ID") != -1)
		{
			parametre >> CurrentSource.ID;
			//std::cout << "Rx : " << CurrentSource.Rcm[0] << "\n";;
		}
		else if (line.find("Rx") != -1)
		{
			parametre >> CurrentSource.Rcm[0];
			//std::cout << "Rx : " << CurrentSource.Rcm[0] << "\n";;
		}
		else if (line.find("Ry") != -1)
		{
			double Ry;
			parametre >> CurrentSource.Rcm[1];
			//std::cout << "Ry : " << CurrentSource.Rcm[1] << "\n";;
		}
		else if (line.find("Hz") != -1)
		{
			parametre >> CurrentSource.Rcm[2];
			//std::cout << "Rz : " << CurrentSource.Rcm[2] << "\n";;
		}
		else if (line.find("Vx") != -1)
		{
			parametre >> CurrentSource.Vcm[0];
			//std::cout << "Vx : " << CurrentSource.Vcm[0] << "\n";;
		}
		else if (line.find("Vy") != -1)
		{
			parametre >> CurrentSource.Vcm[1];
			//std::cout << "Vy : " << CurrentSource.Vcm[1] << "\n";;
		}
		else if (line.find("Vz") != -1)
		{
			parametre >> CurrentSource.Vcm[2];
			//std::cout << "Vz : " << CurrentSource.Vcm[2] << "\n";;
		}
		else if (line.find("NumBlocks") != -1)
		{
			parametre >> CurrentSource.NumBlocks;
			//std::cout << "Numblocks : " << CurrentSource.NumBlocks << "\n";
		}
		else
		{
			//son els volums!
			//netegem
			CurrentSource.Volumes.clear();

			std::stringstream ss(line);

			double i;

			while (ss >> i)
			{
				CurrentSource.Volumes.push_back(i);

				if (ss.peek() == ',')
				ss.ignore();
			}
			//std::cout << CurrentSource.Volumes[0] << "  " << CurrentSource.Volumes[1] << "  " << CurrentSource.Volumes[2] << "  ";
			//std::cout << CurrentSource.Volumes.size() << "\n";
		}



	}
	//std::cout << FONTS.size() << "\n";
	return FONTS;
}


#endif
