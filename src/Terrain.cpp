#ifndef TERRAIN_CPP
#define TERRAIN_CPP

#include "Vec3.h"
#include "Terrain.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
//Define number pi
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

CTerrain::CTerrain()
{
	mdt=NULL;
	aspect=NULL;
	slope=NULL;
	//printf("constructor 1!\n");

}

CTerrain::CTerrain(std::string TerrainFilename, std::string ObjectsFilename)
{
	mdt=NULL;
	aspect=NULL;
	slope=NULL;
	//printf("constructor 2. Opening files= %s\n", TerrainFilename.c_str() );

	//if(TerrainFilename!="")
	model.loadTerrain(this,TerrainFilename,ObjectsFilename);

}

	//Funció per carregar el MDE en una matriu!
void CTerrain::loadTerrain(std::string TerrainFilename,std::string ObjectsFilename)
{
	model.loadTerrain(this,TerrainFilename,ObjectsFilename);
}


	//Funció per veure si s'ha carregat la matriu (nomes 10 elements)
void CTerrain::printMDT()
{
		std::cout<<"Impressio MDT: \n";
		for(int i=0;i<1000;i++)
		{
			std::cout<<mdt[i]<<" ";
		}
		std::cout<<std::endl;

}
void CTerrain::printASPECT()
{
		std::cout<<"Impressio MDT: \n";
		for(int i=0;i<100;i++)
		{
			std::cout<<aspect[i]<<"\n";
		}
		std::cout<<std::endl;

}


void CTerrain::printError(char const * qui)
{
 //char out[]= {"out of range in "};
 //char final[]= {" matrix\n"};

	//cout << out + qui + final;
	printf("out of range in %s matrix\n", qui );
	exit(0);
}

/*********************************************************************************
** Càlcul de la cota interpolada dels quatre veïns amb l'interpolador bilineal. **
**********************************************************************************/
double CTerrain::Height(double X, double Y, int method)
{
int i,j;
double xindex, yindex, a, b, H;

	xindex= (X-xllcenter)/cellsize;
	yindex= (yulcenter-Y)/cellsize;
	//printf("xll= %lf yll=%lf yul=%lf\nY= %lf  csize= %lf j= %lf\n", xllcenter, yllcenter, yulcenter, Y , cellsize , yindex );

	if(xindex<0 || yindex<0 )
		printError("mdt");

	i= (int)xindex;
	j= (int)yindex;

	if(method==NEIGHBOUR)
	{
		std::cout << "NO IMPLEMENTAT";
	}
	else if(method==BILINEAR)
	{
		if( i>=0 && i<ncols-1 && j>=0 && j<nrows-1 )
		{

			/*	Interpolation scheme:

					H01               H11
					   ┌────┬────────┐
					   │    │        │		Getting data from mdt:
					   │    │        │			H00=mdt[ncols*(j+1)+i];
					   │    │        │			H10=mdt[ncols*(j+1)+i+1];
					 b ├────┼────────┤			H01=mdt[ncols*j+i];
					 ↑ │    │        │			H11=mdt[ncols*j+i+1];
					 ↑ │    │        │
					 ↑ └────┴────────┘
					H00 →→→→a         H10

			 */

			// Normalized square (1x1)
			a= xindex-i;
			b= 1-(yindex-j);

			H= 	mdt[ncols*(j+1)+i]		*(1-a)*(1-b)+
				mdt[ncols*(j+1)+i+1]	*a*(1-b)+
				mdt[ncols*j+i]			*(1-a)*b+
				mdt[ncols*j+i+1]		*a*b;

			return H;
		}
		else if( i>=0 && i<ncols-1 && j>=0 && j<nrows )
		{
			a= xindex-i;
			H= (1-a)*mdt[ncols*j+i]+
				a*mdt[ncols*j+i+1];

			return H;
		}
		else
		{
			printf("A fragment has gone out of the model\n");
			//return 9999;
			//printf("xll= %lf yll=%lf yul=%lf\nY= %lf  csize= %lf j= %lf\n", xllcenter, yllcenter, yulcenter, Y , cellsize , yindex );
			printError("mdt");
		}
	}

}

/*********************************************************************************
** Càlcul del pendent i l'azimut de la línia de màxim pendent interpolats dels  **
** quatre veïns amb l'interpolador bilineal.                                    **
**********************************************************************************/
bool CTerrain::SlopeAndAngle(double X, double Y, int method, double* slope, double* aspect )
{
int i,j;
double xindex, yindex, a=0.0, b=0.0;
double dx=0.0, dy=0.0;


	xindex= (X-xllcenter)/cellsize;
	yindex= (yulcenter-Y)/cellsize;
	//printf("xll= %lf yll=%lf yul=%lf\nY= %lf  csize= %lf j= %lf\n", xllcenter, yllcenter, yulcenter, Y , cellsize , yindex );

	if(xindex<0 || yindex<0 )
		printError("mdt");

	i= (int)xindex;
	j= (int)yindex;

	if(method==NEIGHBOUR)
	{
		std::cout << "NO IMPLEMENTAT";
	}
	else if(method==BILINEAR)
	{
		if( i>=0 && i<ncols-1 && j>=0 && j<nrows-1 )
		{

			/*	Interpolation scheme:

					H01               H11
					   ┌────┬────────┐
					   │    │        │		Getting data from mdt:
					   │    │        │			H00=mdt[ncols*(j+1)+i];
					   │    │        │			H10=mdt[ncols*(j+1)+i+1];
					 b ├────┼────────┤			H01=mdt[ncols*j+i];
					 ↑ │    │        │			H11=mdt[ncols*j+i+1];
					 ↑ │    │        │
					 ↑ └────┴────────┘
					H00 →→→→a         H10

			 */

			a= (xindex-i);
			b= 1-(yindex-j);

			dx= mdt[ncols*(j+1)+i]		*(b-1)+
				mdt[ncols*(j+1)+i+1]	*(1-b)-
				mdt[ncols*j+i]			*b+
				mdt[ncols*j+i+1]		*b;

			dy= mdt[ncols*(j+1)+i]		*(a-1)-
				mdt[ncols*(j+1)+i+1]	*a+
				mdt[ncols*j+i]			*(1-a)+
				mdt[ncols*j+i+1]		*a;

			*slope= atan( sqrt(dx*dx+dy*dy)/cellsize ) * (180.0/M_PI); // pendent de la caiguda


			//std::cout << "		Angle atan2(dx/dy): " << atan2(-dx,-dy)*(180/M_PI) << "\n";
			*aspect=(atan2(-dx,-dy)*(180/M_PI));

			//Correct if necessary (positives angles from N clockwise, negatives from N couter-clockwise)
			if (*aspect < 0.0)
			{
				*aspect=*aspect+360.0;
			}

			//std::cout << "		Aspect2: " << *aspect << "  Slope2: " << *slope  << "\n";
			//std::cout << "			dx: " << dx << "  dy: " << dy  << "\n\n";
			//std::cout << "			H01: " << mdt[ncols*j+i] << "  H11: " << mdt[ncols*j+i+1]  << "\n";
			//std::cout << "			H00: " << mdt[ncols*(j+1)+i] << "  H10: " << mdt[ncols*(j+1)+i+1]  << "\n\n";
			//std::cout << "			a: " << a << "  b: " << b  << "\n\n";

			return true;
		}
		else
		{
			//printf("A fragment has gone out of the model- ANGLE COMPUTATION\n");
			printError("mdt");
		}
	}

	return false;

}


CVec3 CTerrain::NormalVector(double X,double Y) {
	int i,j;
	double xindex, yindex, a=0.0, b=0.0;
	double dx, dy;

	xindex= (X-xllcenter)/cellsize;
	yindex= (yulcenter-Y)/cellsize;
	//printf("xll= %lf yll=%lf yul=%lf\nY= %lf  csize= %lf j= %lf\n", xllcenter, yllcenter, yulcenter, Y , cellsize , yindex );

	if(xindex<0 || yindex<0 )
		printError("mdt");

	i= (int)xindex;
	j= (int)yindex;

	if( i>=0 && i<ncols-1 && j>=0 && j<nrows-1 )
	{

		/*	Interpolation scheme:

				H01               H11
				   ┌────┬────────┐
				   │    │        │		Getting data from mdt:
				   │    │        │			H00=mdt[ncols*(j+1)+i];
				   │    │        │			H10=mdt[ncols*(j+1)+i+1];
				 b ├────┼────────┤			H01=mdt[ncols*j+i];
				 ↑ │    │        │			H11=mdt[ncols*j+i+1];
				 ↑ │    │        │
				 ↑ └────┴────────┘
				H00 →→→→a         H10

		 */

		a= (xindex-i);
		b= 1-(yindex-j);

		dx= mdt[ncols*(j+1)+i]		*(b-1)+
			mdt[ncols*(j+1)+i+1]	*(1-b)-
			mdt[ncols*j+i]			*b+
			mdt[ncols*j+i+1]		*b;

		dy= mdt[ncols*(j+1)+i]		*(a-1)-
			mdt[ncols*(j+1)+i+1]	*a+
			mdt[ncols*j+i]			*(1-a)+
			mdt[ncols*j+i+1]		*a;

	}
	else
	{
		//printf("A fragment has gone out of the model - Normal computation\n");
		printError("mdt");
	}

	/*
	printf("	|	> Normal computation\n");
	printf("	|		dx:	%.8lf	dy:	%.8lf\n",dx/0.2,dy/0.2);
	printf("	|			Heigths:\n");
	printf("	|			%.8lf	%.8lf\n",mdt[ncols*j+i],mdt[ncols*j+i+1]);
	printf("	|			%.8lf	%.8lf\n",mdt[ncols*(j+1)+i],mdt[ncols*(j+1)+i+1]);
	*/
	CVec3 n{-dx/cellsize,-dy/cellsize,1};
	CVec3 nNorm=n/n.length();
    return nNorm;
}


/*********************************************************************************
** Càlcul del pendent interpolat dels quatre veïns amb l'interpolador bilineal. **
** Versió a descartar perquè ens estalviem els fitxers fets amb un SIG.			**
**********************************************************************************/
double CTerrain::Slope(double X, double Y, int method)
{
int i,j;
double xindex, yindex, a, b, Slope;

	xindex= (X-xllcenter)/cellsize + 0.5;
	yindex= (yulcenter-Y)/cellsize + 0.5;

	if(xindex<0 || yindex<0 )
		printError("slope");

	i= (int)xindex;
	j= (int)yindex;

	if(method==NEIGHBOUR)
	{
		if( i>=0 && i<ncols && j>=0 && j<nrows )
			return slope[ncols*j+i];
		else
			//printf("A fragment has gone out of the model - Slope\n");
			printError("slope");
	}

}

/*********************************************************************************
** Càlcul de l'aspect interpolat dels quatre veïns amb l'interpolador bilineal. **
** Versió a descartar perquè ens estalviem els fitxers fets amb un SIG.			**
**********************************************************************************/
double CTerrain::Aspect(double X, double Y, int method )
{
int i,j;
double xindex, yindex, a, b, Aspect;

	xindex= (X-xllcenter)/cellsize + 0.5;
	yindex= (yulcenter-Y)/cellsize + 0.5;

	if(xindex<0 || yindex<0 )
		printError("aspect");

	i= (int)xindex;
	j= (int)yindex;

	if(method==NEIGHBOUR)
	{
		if( i>=0 && i<ncols && j>=0 && j<nrows )
			return aspect[ncols*j+i];
		else
			//printf("A fragment has gone out of the model - Aspect\n");
			printError("aspect");
	}

}

int CTerrain::xIndex(double X, double Y)
{
int xIndex;
double xindex;
	xindex= (X-xllcenter)/cellsize;
	xIndex= (int)xindex;
	return xIndex;
}

int CTerrain::yIndex(double X, double Y)
{
int yIndex;
double yindex;
	yindex= (yulcenter-Y)/cellsize;
	yIndex= (int)yindex;
	return yIndex;
}



#endif
