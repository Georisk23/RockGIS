#ifndef RASTER_CPP
#define RASTER_CPP


#include "Raster.h"
#include "Terrain.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

CLecturaDelModel::CLecturaDelModel()
{

}

	//Funció per carregar el MDE d'un ASCIIGRID a una matriu!
void CLecturaDelModel::loadTerrain(CTerrain *t, std::string TerrainFilename, std::string ObjectsFilename)
{
FILE *pt;
char aux[20];
int size;

	if(t->mdt!=NULL) delete [] t->mdt;
	if(t->aspect!=NULL) delete [] t->aspect;
	if(t->slope!=NULL) delete [] t->slope;

		//Paths
	std::string MDE=TerrainFilename;

	if( (pt= fopen(MDE.c_str(),"r"))==NULL )
	{
		printf( "Problems opening file: %s\n" , MDE.c_str() );
		exit(0);
	}
	fscanf(pt,"%s %d" , &aux[0] , &ncols );
	fscanf(pt,"%s %d" , &aux[1] , &nrows );
	fscanf(pt,"%s %lf" , &aux[2] , &xllcenter );
	fscanf(pt,"%s %lf" , &aux[3] , &yllcenter );
	fscanf(pt,"%s %lf" , &aux[4] , &cellsize );
	fscanf(pt,"%s %lf" , &aux[5] , &nodatavalue );

	//std:cout << aux[0] <<  aux[1] << aux[2] << aux[3] << aux[4] << aux[5] << "\n" ;
	//printf("LEC %d %d %lf %lf %lf %lf\n", ncols,nrows,xllcenter,yllcenter,cellsize,nodatavalue );

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*
	std::ifstream fin(TerrainFilename.c_str());
    std::string line;


	for( int k=0 ; k<20 ; k++ ) {

		linegetline(fin, line);
        std::istringstream sin(line.substr(line.find("=") + 1));

        std::cout << "\nARG!!!\n";
        std::cout << line;

        //lectura individual dels parametres
        if (line.find("xllcorner") != -1)
        {
            std::cout << "xllcorner!!!";
		}
	}
*/
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



	//pause
	//getchar();

//calculem i guardem yullcenter

	yulcenter= yllcenter+cellsize*(nrows-1);
	size= ncols*nrows;

	if( (t->mdt=new float[size]) == NULL )
	{
		cout<<"No s'ha pogut reservar memoria pel MDT\n";
		exit(0);
    }

//carreguem matriu MDT
	for( int k=0 ; k<size ; k++ )
	{
		fscanf( pt , "%f" , &t->mdt[k] );
	}
	fclose (pt);

		t->ncols= ncols;
		t->nrows= nrows;
		t->xllcenter= xllcenter;//-cellsize/2.0;
		t->yllcenter= yllcenter;//-cellsize/2.0;
		t->yulcenter= yulcenter;//-cellsize/2.0;
		t->cellsize= cellsize;
		t->nodatavalue= nodatavalue;

		//std::cout << "\nTerrain loaded and stored succesfully \n";

//carreguem dades pantalles
		std::string OBJECTS=ObjectsFilename;
		if( (pt= fopen(OBJECTS.c_str(),"r"))==NULL )
		{
			printf("Problems opening OBJECTS.txt " );
			exit(0);		
		}	

  		fscanf(pt,"%s%d" , &aux[0] , &ncols ); 
		fscanf(pt,"%s%d" , &aux[0] , &nrows ); 
		fscanf(pt,"%s%lf" , &aux[0] , &xllcenter ); 
		fscanf(pt,"%s%lf" , &aux[0] , &yllcenter ); 
		fscanf(pt,"%s%lf" , &aux[0] , &cellsize ); 
		fscanf(pt,"%s%lf" , &aux[0] , &nodatavalue );
 //printf("LEC %d %d %lf %lf %lf %lf\n", ncols,nrows,xllcenter,yllcenter,yulcenter,cellsize );
		if( (t->slope=new float[size]) == NULL )
		{
			cout<<"Error storing memory for OBJECTS\n";
			//system("PAUSE");
			exit(0);
        }

	    for( int k=0 ; k<size ; k++ )
		     fscanf( pt , "%f" , &t->slope[k] );
		
			
		fclose (pt);



}



#endif
