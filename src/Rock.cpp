#ifndef ROCK_CPP
#define ROCK_CPP



#include <stdio.h>
#include <stdlib.h>	// srand, rand
#include <cstdlib>  // ((double) rand() / (RAND_MAX))
#include <iostream>
#include <math.h>	// cos, sin, M_PI
#include <iomanip>	// setprecioson
#include <fstream>	// outfile
#include <vector>
#include <algorithm>// sort
#include <numeric>  // accumulate
#include <typeinfo> // typeid
#include <string>

//Define number pi
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif



#include "Rock.h"
#include "Vec3.h"
#include "Mat3.h"


	//Print cinematics
void CRock::printCinematics(char* File,CTerrain& Terrain,int Precision,CParameters& Parameters,std::string &OutputToStore)
{
    //compuite velocity (m/s) and energy (kJ)
		double Vmodul=pow((pow(Vcm[0],2)+pow(Vcm[1],2)+pow(Vcm[2],2)),0.5);
		double energy=0.5*Parameters.RockDensity*Volume*Vmodul*Vmodul/1000;

    char info[600];
    sprintf(info,"%.3lf,%.3lf,%.3lf,%.3lf,%.8lf,%.3lf,%d,%.3lf,%.3lf\n",Rcm[0],Rcm[1],Rcm[2],Vmodul,Volume,height2ground(Terrain),id,energy,time);
    OutputToStore.append(info);
    //std::cout << results;
}

//Funcio per fer caure el bloc
void CRock::rockfall(CTerrain& Terrain,CParameters& Parameters,std::string &Output_Trajectories,std::string &Output_Stoppages,std::string &Output_PASSAGES)
{
		xIndex=Terrain.xIndex(Rcm[0],Rcm[1]);
		yIndex=Terrain.yIndex(Rcm[0],Rcm[1]);
		
		do
		{
			//std::cout << Impact << " ";
			if(Impact==0)
			{
				if(Parameters.PrintMode==1) {std::cout << "\n\n\n	> Flying\n";}
				// Fly until impact is detected
				fly(Terrain,Parameters,Output_Trajectories,Output_PASSAGES);

			}
			else if (Impact==1)
			{

				// Impact has been detected: apply rebound to modify speed
				if(Parameters.PrintMode==1) {std::cout << "	> Rebound\n";}

				//store impact information
				//printCinematics("impacts.csv",Terrain,4);

				//Apply rebound algorithm
				//double rebound2(Terrain,Parameters);
				double NormalEfectiveEnergy=rebound2(Terrain,Parameters);

				//Stop criterion
				if(StopCriterion(Parameters)==true){break;}

				//nova implementacio algorisme Roger
				double InitialArea=6.0*pow(Volume,2./3.);

				double NewArea = Parameters.na1*pow(NormalEfectiveEnergy,Parameters.na2);
				double AreaRatio = NewArea/(NewArea+InitialArea);

				double b = Parameters.b1*AreaRatio+Parameters.b2;
				double q = Parameters.q1*AreaRatio+Parameters.q2;

				// Corregim parametres b,q en cas que surtin dels seus limits de definicio
				if(b<1.0){b=1.0;}
				if(q<0){q=0;}
				if(q>1.0){q=1.0;}

				/*
				if(Parameters.PrintMode==1)
				{
				std::cout << "\n		> NormalEfectiveEnergy (kJ): " << NormalEfectiveEnergy << "\n";
				std::cout << "		> InitialArea: " << InitialArea << "\n";
				std::cout << "		> NewArea: " << NewArea << "\n";
				std::cout << "		> AreaRatio: " << AreaRatio << "\n";
				}
				*/

				//actualitza els parametres p i b de la classe parameters AIXO ES UNA GUARRADA ja que la classe parameters esta pensada per variables fixes
				Parameters.PowerP=q;
				Parameters.PowerB=b;
				
				double MinAreaRatioRandom = Parameters.MinAreaRatio*(1+0.06*(2.0*((double) rand() / (RAND_MAX))-1.0)); // HARDCODED
				
				if (AreaRatio>MinAreaRatioRandom)
				{

						// If breackage criterion is accomplished update block state to "fragmented"
					Fragmented=1;
					if(Parameters.PrintMode==1) {std::cout << "	> Fragmentation\n";}
					// And then generate and simulate all the fragments
					Breakage(Terrain,Parameters,Output_Trajectories,Output_Stoppages,Output_PASSAGES);
				}

				//do{cout << '\n' << "Press a key to continue...";} while (cin.get() != '\n');

			}

		} while(StopCriterion(Parameters)==false);

		// Store data of the final fragments
		if (Fragmented==0)
		{
			std::string Stop=Parameters.Stoppages_path;
			char *cstr2 = &Stop[0u];
			printCinematics(cstr2,Terrain,8,Parameters,Output_Stoppages);
		}
}



//Funcio per generar i simular els fragments
void CRock::Breakage(CTerrain& Terrain,CParameters& Parameters,std::string &Output_Trajectories,std::string &Output_Stoppages,std::string &Output_PASSAGES)
{

		std::vector<double> Fragments=MassDistribution(Volume,"Fractal",Parameters);
		
		//std::cout << Rcm[0] << ";" << Rcm[1] << ";" << Rcm[2] << ";" << Volume << ";" << Fragments.size()<<"\n";
		
		// Base velocity vector for centering the cone
		double p[] = {Vcm[0],Vcm[1],Vcm[2]};
		std::vector<double> ReferenceVector(p, p+3);

		// Cone oberture
		double Angle=(Parameters.Cone/2)*(M_PI/180);

		//per cada fragment li associem un vector de sortida
		for (int q=0;q<Fragments.size();q++)
		{

			//vector definition
			std::vector<double> UnitariVector;
			std::vector<double> RotatedUnitariVector;
			std::vector<double> ScaledVelocity;

			ScaledVelocity.push_back(0.0);
			ScaledVelocity.push_back(0.0);
			ScaledVelocity.push_back(0.0);


			double w[3];

			//escalat de la velocitat (REDUCTION TO 70% OF INITIAL SPEED
			double EnergyLoss = 0.01; //percentatge d'energia que es perd estsava a 0.2 HARDCODED
			double VelocityModule = sqrt(1-EnergyLoss)*sqrt(pow(Vcm[0],2)+pow(Vcm[1],2)+pow(Vcm[2],2));

			//std::cout << "TOKEN 1\n";
			// iteare until finding a valid release vector
			int iter=0;
			do
			{
				//std::cout << "TOKEN 1\n";
				// Generate unitari vector in cone arround Z axis
				UnitariVector = UnitariInCone(Angle);

				// Rotate cone axis to match release velocity after rebound
				RotatedUnitariVector = EulerRotateVector(ReferenceVector,UnitariVector);

				//fill scaled velocity vector
				ScaledVelocity[0] = VelocityModule*RotatedUnitariVector[0];
				ScaledVelocity[1] = VelocityModule*RotatedUnitariVector[1];
				ScaledVelocity[2] = VelocityModule*RotatedUnitariVector[2];

				//if(Parameters.PrintMode==1) {std::cout <<  VectorInGround(Terrain,Parameters,ScaledVelocity) << "\n";}

				iter=iter+1;
				if(iter>100000){
					//std::cout << "           100k iteracions buscant vector... Seguir? [enter] \n";
					//getchar();
					break;
					iter=1;
					}

			} while(VectorInGround(Terrain,Parameters,ScaledVelocity)==true);

			//simulation of the fragment!!!
			CRock *prova;
			//id=100000*id+q+1;
			float reduction=1.0; // HARDCODED
			prova= new CRock{{Rcm[0],Rcm[1],Rcm[2]},{ScaledVelocity[0],ScaledVelocity[1],ScaledVelocity[2]},{0.0,0.0,-9.81},{Wcm[0]*reduction,Wcm[1]*reduction,Wcm[2]*reduction},Fragments[q],pow((Fragments[q]*3.0)/(4.0*M_PI),1.0/3.0),0,0,id,0,time};
			//Fem que el nou rock es simuli

			prova->rockfall(Terrain,Parameters,Output_Trajectories,Output_Stoppages,Output_PASSAGES);

			delete prova;
			//std::cout<<"\e[A"; // Per borrar la linia que posa el delete...
		}
}



//funcio per calcular volums en el metode de distribucio fragtal de la massa
double CRock::Vol_f_N(double Lmax, int N, int k, double D)
{
		double Vol_f_N = pow(Lmax*pow(N/k,-1.0/D),3.0);
		return Vol_f_N;
}

	//funcio que genera els fragments en funcio del metode escollit
std::vector<double> CRock::MassDistribution(double InitialVolume,std::string MassDistMethod,CParameters& Parameters)
{
		std::vector<double> fragments; //Vector containing all fragments

		//std::cout << "METHOD SELECTED:\n";

		if (MassDistMethod == "EquiDist")
		{
			//std::cout << "Equidistributed method\n";

			// TESTING: algorisme divideix el bloc en n trossos iguals
			for (int q=0;q<20;q++)
			{
				double VolFragment=InitialVolume/20.0;
				fragments.push_back(VolFragment);
			}
		}

		else if (MassDistMethod == "Fractal")
		{
			//	std::cout << "Fractal method\n";


			double q = Parameters.PowerP;
			double b = Parameters.PowerB;


			if(Parameters.PrintMode==1)
			{
			std::cout << "	|	 Fragmentation using q: " << q << "  \n";
			std::cout << "	|	 Fragmentation using b: " << b << "  \n";
			}

			int n=1;

      double lmax=q*pow(pow(b,n),Parameters.r_variant);
      double Df=3.0+(log(pow(b,Parameters.r_variant)-lmax)/log(b));
      
      double vmin=Parameters.VminFrag;
      
      if(Parameters.PrintMode==1)
      {
      std::cout << "	|	 lmax: " << lmax << "\n";
      std::cout << "	|	 Df: " << Df << "\n";
      }

			while(true)
			{
				
				double newFrag=Volume*lmax*pow(n,-1.0/Df);
				double sum = std::accumulate(fragments.begin(), fragments.end(), 0.0);
				
				if(Parameters.PrintMode==1)
				{
				std::cout << "lmax: " << lmax << "\n";
				std::cout << "Df: " << Df << "\n";
				std::cout << "NewFrag: " << newFrag << "\n";
				std::cout << "Suma: " << sum << "\n";
				}
				
				
				if(sum+newFrag<Volume && newFrag>vmin)
				{					
					fragments.push_back(newFrag);
					
					if(Parameters.PrintMode==1){std::cout << newFrag << "\n\n ";}
					
				}else{
					if(n=1){
						//std::cout << "Testing\n";
						//std::cout << "Sobres: " << InitialVolume-sum << "\n";
						fragments.push_back(InitialVolume-sum);
						//fragments[0]=fragments[0]+(InitialVolume-sum);
						}
					break;
					}
					
				n+=1;
			}



		}
		if(fragments.size()==1){fragments[0]=Volume;}

	return fragments;
}



// Generate unitary vector in cone arround Z axis
std::vector<double> CRock::UnitariInCone(double Obertura)
{
		std::vector<double> Unitari;

		double z=1-((1-cos(Obertura))*((double) rand() / (RAND_MAX)));
		double fi=2*M_PI*((double) rand() / (RAND_MAX));
		double x,y;
		x=pow(1-pow(z,2),0.5)*cos(fi);
		y=pow(1-pow(z,2),0.5)*sin(fi);

		Unitari.push_back(x);
		Unitari.push_back(y);
		Unitari.push_back(z);

		return Unitari;
}



// Rotate a vector so as the initial Z axis matches the output velocity after breackage
std::vector<double> CRock::EulerRotateVector(std::vector<double> ReferenceVector,std::vector<double> RotatingVector)
{
		//Compute rotation angles
		double r = sqrt(pow(ReferenceVector[0],2)+pow(ReferenceVector[1],2)+pow(ReferenceVector[2],2));
		double theta = acos(ReferenceVector[2]/r);
		double phi=atan2(ReferenceVector[1]/r,ReferenceVector[0]/r);

		//at this point we have (r,theta,phi)
		//std::cout << "\n[r,theta,phi] = " << r << " " << theta << " " << phi << "\n";


		double Rot[3][3];
		// Rot=Rz(phi)*Ry(theta)  First rotation Y(theta) then Z(phi)
		double cp = cos(phi);
		double sp = sin(phi);
		double ct = cos(theta);
		double st = sin(theta);

		Rot[0][0] = cp*ct;
		Rot[0][1] = -sp;
		Rot[0][2] = cp*st;

		Rot[1][0] = sp*ct;
		Rot[1][1] = cp;
		Rot[1][2] = sp*st;

		Rot[2][0] = -st;
		Rot[2][1] = 0.0;
		Rot[2][2] = ct;

		/*
		std::cout << "Rotation matrix\n";
		for (int a=0;a<3;a++)
		{
			for (int b=0;b<3;b++){
				std::cout << Rot[a][b] << "  ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
		*/

		double Transformed[3];
		Transformed[0]=0.0;
		Transformed[1]=0.0;
		Transformed[2]=0.0;

		for (int q=0;q<3;q++)
		{
		for (int w=0;w<3;w++){
			double tosum = Rot[q][w]*RotatingVector[w];
			Transformed[q]=Transformed[q]+tosum;
		}
	}

  	//put transformed results in output vector
	std::vector<double> Rotated(Transformed, Transformed+3);
	return Rotated;
}



// Make the block advance through the air and change state when impact is detected
void CRock::fly(CTerrain& Terrain,CParameters& Parameters,std::string &Output_Trajectories,std::string &Output_PASSAGES)
{

		if(Parameters.PrintMode==1) {
			std::cout << "	|	> Initial state:\n";
			printf("	|		R=(%.3lf,%.3lf,%.3lf)	V=(%.3lf,%.3lf,%.3lf)\n",Rcm[0],Rcm[1],Rcm[2],Vcm[0],Vcm[1],Vcm[2]);
		}
		double dH;
		std::vector<double> New_Position;
		std::vector<double> Impact_Position;

		do
		{

			//movem la pedra un dt
			New_Position = advance2(Parameters.TimeStep);

			//actualitzem dH
			dH=height2ground(Terrain,New_Position);

			//std::cout << dH << " ,  ";
      //HARDCODED
      if(dH>100){
        Vcm[0]=0.0;
        Vcm[1]=0.0;
        Vcm[2]=0.0;
        Impact=1;
        std::cout << "Se fue...";
      }

			if (dH <= 0.0)
			{
				// Look for dt of exact impact point
				double dtImpact = dt2impact(Terrain,Parameters);

				Impact_Position = advance2(dtImpact);

				//Incidence angle

						std::vector<double> Rb;
						std::vector<double> Rg;
						std::vector<double> V1;
						std::vector<double> V2;

						Rb.push_back(Rcm[0]);
						Rb.push_back(Rcm[1]);
						Rb.push_back(Rcm[2]);

						double dH;
						dH=height2ground(Terrain,Rb);

						Rg.push_back(Rb[0]);
						Rg.push_back(Rb[1]);
						Rg.push_back(Rb[2]-dH);

						V1.push_back(Rb[0]-Impact_Position[0]);
						V1.push_back(Rb[1]-Impact_Position[1]);
						V1.push_back(Rb[2]-Impact_Position[2]);

						V2.push_back(Rg[0]-Impact_Position[0]);
						V2.push_back(Rg[1]-Impact_Position[1]);
						V2.push_back(Rg[2]-Impact_Position[2]);

						/*
						//calcul angle impacte (activar si cal)
						double angle;
						double V1M;
						double V2M;

						V1M=pow((pow(V1[0],2)+pow(V1[1],2)+pow(V1[2],2)),0.5);
						V2M=pow((pow(V2[0],2)+pow(V2[1],2)+pow(V2[2],2)),0.5);

						angle = acos((V1[0]*V2[0]+V1[1]*V2[1]+V1[2]*V2[2])/(V1M*V2M))*(180.0 / M_PI);

						//std::cout << "			Incidence Angle: " << angle << "\n";
						*/

				//advance time to impact
				advance(dtImpact,Terrain,Parameters);
				time=time+dtImpact;

				Impact=1;
			}
			else
			{
				advance(Parameters.TimeStep,Terrain,Parameters);
				time=time+Parameters.TimeStep;
								
				//AQUI LA COMPROVACIO DELS INDEXOS!!!!!!!!!!!!!!!
				int xIndexNew=Terrain.xIndex(Rcm[0],Rcm[1]);
				int yIndexNew=Terrain.yIndex(Rcm[0],Rcm[1]);
				
				if(xIndexNew!=xIndex || yIndexNew!=yIndex){
					//actualitzem els idexos
					xIndex=xIndexNew;
					yIndex=yIndexNew;
					//printf("[%i,%i]\n",xIndex,yIndex);
					double Vmodul=pow((pow(Vcm[0],2)+pow(Vcm[1],2)+pow(Vcm[2],2)),0.5);
					double energy=0.5*Parameters.RockDensity*Volume*Vmodul*Vmodul/1000;
					char info[50];
					sprintf(info,"%i;%i;%.6lf;%.6lf\n",xIndex,yIndex,energy,dH);
					Output_PASSAGES.append(info);
				}
		
			}


			//Save in output.csv !!!!!!!!!!      !!!!!!!!!      !!!!!!!!!!!!        !!!!!!!!!!!!!
			std::string Traject=Parameters.Trajectories_path;
			char *cstr = &Traject[0u];

			if(Parameters.StoreTrajectories==1){printCinematics(cstr,Terrain,4,Parameters,Output_Trajectories);}
			
			//CONTROL LINES FOR MONASTERIO DE PIEDRA
			double slopeASC= Terrain.Slope( Rcm[0], Rcm[1] , NEIGHBOUR);
			
				
			if (slopeASC!=0 && slopeASC!=pantalla)
			{
				double Emax;
				double H;
				
				std::cout << "IMPACTE a " << slopeASC << "\n";
				if (slopeASC==1){Emax=Parameters.Ep1;H=Parameters.Hp1;}
				else if (slopeASC==2){Emax=Parameters.Ep2;H=Parameters.Hp2;}
				else if (slopeASC==3){Emax=Parameters.Ep3;H=Parameters.Hp3;}
				
				
				double breakage;
				double Vmodul=pow((pow(Vcm[0],2)+pow(Vcm[1],2)+pow(Vcm[2],2)),0.5);
				double energy=0.5*Parameters.RockDensity*Volume*Vmodul*Vmodul/1000;
				
                double Himpact=height2ground(Terrain);
				
                std::cout << "Himp:  " << Himpact << "    Energy: " << energy << "\n";
                
				if (Himpact<=H) //impacte a la pantalla
				{
                    std::cout << "H<Pantalla. E=  " << energy << "\n";
					//if (energy <= Emax) // BINARI!!!!!!!!!!
					if (energy <= Emax) //energia baixa: el para
					{
						//aturem bloc
						Vcm[0] = 0.0;
						Vcm[1] = 0.0;
						Vcm[2] = 0.0;
						Impact=1;
						pantalla=slopeASC;
						breakage=0;
			
					}
					else //energia alta: el bloc travessa la pantalla
					{
						pantalla=slopeASC;

						double RemainEnergy=(Emax-energy)*0.8;
						double RemainVelocity=1.0*sqrt((2.0*RemainEnergy*1000.0)/(Parameters.RockDensity*Volume));
						
						Vcm[0] = (Vcm[0]/Vmodul)*RemainVelocity;
						Vcm[1] = (Vcm[1]/Vmodul)*RemainVelocity;
						Vcm[2] = (Vcm[2]/Vmodul)*RemainVelocity;
						breakage=1;
					}
					
									
					
					//store impact
					std::string Imp=Parameters.Impacts_path;
					char *impacPath = &Imp[0u];
									std::ofstream outfile;
					outfile.open(impacPath, std::ios::app);
					outfile << std::setprecision(6);
					outfile << std::fixed;
					// PATH  HEIGTH2GROUNG  ENERGY
					outfile << slopeASC << "," << Rcm[0] << "," << Rcm[1] << ","  << height2ground(Terrain) << "," << energy << "," << id << "," << time << "," << breakage << std::endl;
					outfile.close();
				}
				else  //si passa per sobre
				{
					if (slopeASC!=pantalla) //si no haviem guardat abans ja aquesta dada
					{
												
						//store impact
						std::string Imp=Parameters.Impacts_path;
						char *impacPath = &Imp[0u];
										std::ofstream outfile;
						outfile.open(impacPath, std::ios::app);
						outfile << std::setprecision(6);
						outfile << std::fixed;
						// PATH  HEIGTH2GROUNG  ENERGY
						outfile << slopeASC << "," << Rcm[0] << "," << Rcm[1] << ","  << height2ground(Terrain) << "," << energy << "," << id << "," << time << "," << breakage << std::endl;
						outfile.close();
					}

				}
			}
				
				
				
				
				
	

				
		}while(Impact==0);

		if(Parameters.PrintMode==1) {
			std::cout << "	|	> Final state:\n";
			printf("	|		R=(%.3lf,%.3lf,%.3lf)	V=(%.3lf,%.3lf,%.3lf)\n",Rcm[0],Rcm[1],Rcm[2],Vcm[0],Vcm[1],Vcm[2]);
		}

}



// Computes the exact time to hit the ground using bisection method
double CRock::dt2impact(CTerrain& Terrain,CParameters& Parameters)
{
		std::vector<double> a_Position;
		std::vector<double> b_Position;
		std::vector<double> c_Position;

		double dt = Parameters.TimeStep;

		int count = 1;

		double a = 0.0;
		double b = dt;
		double c = (a+b)/2;

		a_Position = advance2(a);
		b_Position = advance2(b);
		c_Position = advance2(c);

		double Ha = height2ground(Terrain,a_Position);
		double Hb = height2ground(Terrain,b_Position);
		double Hc = height2ground(Terrain,c_Position);

		if(Parameters.PrintMode==1) {std::cout << "	|	> Finding impact point\n";}
		if(Parameters.PrintMode==1) {std::cout << "	|		Impact iteration: " << count << "  a: " << a << "  b: " << b  << "   Hc: " << Hc << "\n";}

		double ToleranciaSuperior = 0.01;

		if (Ha<ToleranciaSuperior)
			{
				c=a;
				Hc=Ha;
				if(Parameters.PrintMode==1) {std::cout << "	|		Ha: " << Ha << "  is close enough\n"; }
			}
			else
			{
				while( (Hc <= 0.0) || (Hc > ToleranciaSuperior) )
				{
					count = count + 1;

					c = (a+b)/2;
					c_Position = advance2(c);
					Hc = height2ground(Terrain,c_Position);

					if(Parameters.PrintMode==1) {std::cout << "	|		Impact iteration: " << count << "  a: " << a << "  b: " << b  << "   Hc: " << Hc << "\n";} ////////////////////////////OJOOOOOOOOOOOOO LINITACIO FALLA A VEGADES

					if ( Ha*Hc < 0.0)
					{
						// Gone to far, dt in [a,c]
						b = c;

					}
					else
					{
						// Not far enough, det in [a,c]
						a = c;
						Ha = Hc;
					}


				//DEBUGING__________________________________________________________________________________________________
				if (count>20)
				{
					if(Parameters.PrintMode==1) {std::cout << "	|		Impact iteration by bisection FAILED\n";}

					double step = dt/1000;
					std::vector<double> R;
					double heigh;

					for (int w=0;w<1000;w++)
					{
						R = advance2(w*step);
						//heigh = height2ground(Terrain,R);
						heigh = Terrain.Height(R[0],R[1]);
						std::cout <<  w*step << " " << heigh << "\n" ;
					}

					break;
					//exit(0);

				}
				//_________________________________________________________________________________________________________

				}

			}


		if(Parameters.PrintMode==1) {std::cout << "	|	> Height to ground at impact point : " << Hc << " in " << count-1 << " iterations\n";}

		return c;
}



double CRock::rebound2(CTerrain& Terrain,CParameters& Parameters)
{
	//velocitat linial i angular (encara no convertits)/////////////////////////////////////////
	CVec3 v{Vcm[0],Vcm[1],Vcm[2]};
	CVec3 w{Wcm[0],Wcm[1],Wcm[2]};

	CVec3 en = Terrain.NormalVector(Rcm[0], Rcm[1]);

	double vn=v.dot(en);

	CVec3 vl=v-(en*vn);

	CVec3 el=vl/vl.length();

	CVec3 et=en.cross(el);

	//perturbation
	CVec3 Zglob{0,0,1.0};

	CVec3 ra=v.cross(Zglob);
	ra=ra/ra.length();

	CVec3 rb=(v-Zglob*v.z);
	rb=rb/rb.length();

	/*
	std::cout << "Ra:	";
	ra.print();
	std::cout << "Rb:	";
	rb.print();
	std::cout << "\n";
	*/
	
	
	float MaxVertDev=Parameters.SlopeVar;												//0.8 C32!!!!!!!!!!!!!!!!!! (abans 0.7)
	float VertDevScale=exp(-0.05*Volume);								//-0.1 C32!!!!!!!!!!!!!!!!!! (Amabs -0.1)
	float a1=MaxVertDev*VertDevScale*((double) rand() / (RAND_MAX));
		
	//float a1=0.25*((double) rand() / (RAND_MAX));
	
	
	float a2=-Parameters.AspectVar*(2.0*((double) rand() / (RAND_MAX))-1.0);

	CMat3 Ra;
	Ra.RotationMatrix(a1,ra);
	CMat3 Rb;
	Rb.RotationMatrix(a2,rb);

	/*
	std::cout << "Ma:\n";
	Ra.print();
	std::cout << "\n";

	std::cout << "Mb:\n";
	Rb.print();
	std::cout << "\n";
	*/
	CMat3 R1;
	CMat3 R2;
	//if(double a=((double) rand() / (RAND_MAX))>0.5){R1=Ra;R2=Rb;std::cout<<"PRIMER\n";}else{R1=Rb;R2=Ra;std::cout<<"SEGON\n";}
	R1=Ra;//sense estocasticitat en aquest tema:
	R2=Rb;

	/*
	std::cout << "R1:\n";
	R1.print();
	std::cout << "\n";

	std::cout << "R2:\n";
	R2.print();
	std::cout << "\n";
	*/

	CVec3 elp=R1*el;
	CVec3 etp=R1*et;
	CVec3 enp=R1*en;

	CVec3 elpp=R2*elp;
	CVec3 etpp=R2*etp;
	CVec3 enpp=R2*enp;



	CVec3 vL{v.dot(elpp),v.dot(etpp),v.dot(enpp)};
	CVec3 wL{w.dot(elpp),w.dot(etpp),w.dot(enpp)};


	float IncidenceAngle=abs(asin((-enpp.dot(v))/v.length())*(180.0/M_PI));

	float kn=Parameters.Kna*pow(IncidenceAngle,Parameters.Knb);
	if(kn>2.0){kn=2.0;}

	//float kt=Parameters.Kta/((Radius*vL.z*vL.z)+Parameters.Kta);
	//float kt=0.8;


  // Mallorca style
  float kt0=0.93;
	float ktMin=0.15;
	float RefVal=Parameters.Kta;
	float VolExp=Parameters.KtRough;

	float kt=(kt0-ktMin)*RefVal/(pow(vL.z,2.0)/pow(Volume,VolExp)+RefVal)+ktMin;
	

	/*
	float kn=30.0/((R*vL.z*vL.z)+30.0);
	if(kn<0.3){kn=0.3;}
	float kt=100.0/((R*vL.z*vL.z)+100.0);
	if(kt<0.4){kt=0.4;}
	*/
	////////////////////////////millorable simplificant, no cal la matriu

	CMat3 m{
		(5.0/7.0)*kt,-(2.0/7.0)*kt,0,
		-(5.0/7.0)*kt,(2.0/7.0)*kt,0,
		0,0,0
		};


	CVec3 v20{vL.y,Radius*wL.x,0};
	CVec3 r20=m*v20;

	CVec3 v21{vL.x,Radius*wL.y,0};
	CVec3 r21=m*v21;

	float r22=-kn*vL.z;

	CVec3 vreL{r21.x,r20.x,r22};
	///testing###################################################################################################3
	//CVec3 vreL{0.7*vL.x,0.7*vL.y,-0.5*vL.z};

	double NormalEfectiveEnergy=0.5*Parameters.RockDensity*Volume*vreL.z*vreL.z;

	CVec3 wreL{r20.y/Radius,r21.y/Radius,0.0};


	CMat3 TMat;
	TMat.TransformationMatrix(elpp,etpp,enpp);
	//TMat.print();

	CVec3 vreG=TMat*vreL;
	CVec3 wreG=TMat*wreL;

	//update speed
	Vcm[0] = vreG.x;
	Vcm[1] = vreG.y;
	Vcm[2] = vreG.z;
	//update rotation
	Wcm[0] = wreG.x;
	Wcm[1] = wreG.y;
	Wcm[2] = wreG.z;

	//get out of impact
	Impact=0;

	if(Parameters.PrintMode==1) {


		printf("	|	> Global incidence values\n");
		printf("	|		VinG:	%.8lf,	%.8lf,	%.8lf\n",v.x,v.y,v.z);
		printf("	|		winG:	%.8lf,	%.8lf,	%.8lf\n",w.x,w.y,w.z);
		printf("	|		normal:	%.8lf,	%.8lf,	%.8lf\n",en.x,en.y,en.z);
		printf("	|	> Local incidence values\n");
		printf("	|		VinL:	%.8lf,	%.8lf,	%.8lf\n",vL.x,vL.y,vL.z);
		printf("	|		winL:	%.8lf,	%.8lf,	%.8lf\n",wL.x,wL.y,wL.z);
		printf("	|	> Impact caracteristics\n");
		printf("	|		Angle		Kn		Kt		a1		a2\n");
		printf("	|		%.8lf	%.8lf	%.8lf	%.8lf	%.8lf\n",IncidenceAngle,kn,kt,a1,a2);
		printf("	|	> Local reflected values\n");
		printf("	|		VreL:	%.8lf,	%.8lf,	%.8lf\n",vreL.x,vreL.y,vreL.z);
		printf("	|		wreL:	%.8lf	%.8lf	%.8lf\n",wreL.x,wreL.y,wreL.z);
		printf("	|	> Global reflected values\n");
		printf("	|		VreL:	%.8lf,	%.8lf,	%.8lf\n",vreG.x,vreG.y,vreG.z);
		printf("	|		wreL:	%.8lf,	%.8lf,	%.8lf\n",wreG.x,wreG.y,wreG.z);

	}

	return NormalEfectiveEnergy;




}


// Criterion for stopping blocks
bool CRock::StopCriterion(CParameters& Parameters)
{
		double Vmodul;
		Vmodul=pow((pow(Vcm[0],2.0)+pow(Vcm[1],2)+pow(Vcm[2],2.0)),0.5);
		if (Vmodul <= Parameters.StopVelocity || Fragmented == 1) {
			return true;
		} else {
			return false;
		}
}



// Check if a generated vector in a cone gets into the ground
bool CRock::VectorInGround(CTerrain& Terrain,CParameters& Parameters,std::vector<double> Velocity)
{
		std::vector<double> Rb;
		std::vector<double> Rg;
		std::vector<double> V1;
		std::vector<double> V2;

		//reduce normal timestep since it can be extremely near!
		double dt = Parameters.TimeStep;

		Rb.push_back(Rcm[0] + Velocity[0]*dt);
		Rb.push_back(Rcm[1] + Velocity[1]*dt);
		Rb.push_back(Rcm[2] + Velocity[2]*dt);

		double dH;
		dH=height2ground(Terrain,Rb);

		Rg.push_back(Rb[0]);
		Rg.push_back(Rb[1]);
		Rg.push_back(Rb[2]-dH);

		V1.push_back(Rb[0]-Rcm[0]);
		V1.push_back(Rb[1]-Rcm[1]);
		V1.push_back(Rb[2]-Rcm[2]);

		V2.push_back(Rg[0]-Rcm[0]);
		V2.push_back(Rg[1]-Rcm[1]);
		V2.push_back(Rg[2]-Rcm[2]);

		double angle;
		double V1M;
		double V2M;

		V1M=pow((pow(V1[0],2.0)+pow(V1[1],2)+pow(V1[2],2)),0.5);
		V2M=pow((pow(V2[0],2.0)+pow(V2[1],2)+pow(V2[2],2)),0.5);

		angle = acos((V1[0]*V2[0]+V1[1]*V2[1]+V1[2]*V2[2])/(V1M*V2M))*(180.0 / M_PI);

		//std::cout << "Angle: " << angle << "\n";

		//   REFLECTED ANGLE!!!!!!!!!!!!!!!!!!!!!
		//std::cout << "			Reflected Angle: " << angle << "\n";

		if (angle <= Parameters.LimitAngle || dH <= 0.0)
		{
			//std::cout << "                      Aquest peta. dH: " << dH << "  Anngle sortida: " << angle << " \n";
			//getchar();
			return true;
		}
		else
		{
			return false;
		}

}



// Advance a dt and update values
void CRock::advance(double dt,CTerrain& Terrain,CParameters& Parameters)
{
		Rcm[0] = Rcm[0] + Vcm[0]*dt;
		Rcm[1] = Rcm[1] + Vcm[1]*dt;
		Rcm[2] = Rcm[2] + Vcm[2]*dt;

		Vcm[0] = Vcm[0]+Acm[0]*dt;
		Vcm[1] = Vcm[1]+Acm[1]*dt;
		Vcm[2] = Vcm[2]+Acm[2]*dt;

		double Vmodul=pow((pow(Vcm[0],2.0)+pow(Vcm[1],2.0)+pow(Vcm[2],2)),0.5);
		double h2g=height2ground(Terrain);

		//DRAG FORCE calculation to update Acm
		if(h2g <= Parameters.DragLayerHeigh && Vmodul <= Parameters.DragSpeedLimit)
		{
			//Update acceleration to add drag force
			double Drag[3];
			Acm[0]=-(Parameters.DragCoeficient*Vcm[0])/Vmodul;
			Acm[1]=-(Parameters.DragCoeficient*Vcm[1])/Vmodul;
			Acm[2]=-9.81-(Parameters.DragCoeficient*Vcm[2])/Vmodul;


			//Print data for checking
			//std::cout <<     "DRAG!        In drag zone + low speed          h2g: " << h2g << "		Vm: " << Vmodul << "		Acm: " << Acm[0] << "  " << Acm[1] << "  " << Acm[2] << "\n";
		}
		else
		{
			//Update acceleration to delete drag
			Acm[0]=0.0;
			Acm[1]=0.0;
			Acm[2]=-9.81;

			//Print data for checking
			if(height2ground(Terrain) <= Parameters.DragLayerHeigh){
				//std::cout << "NO DRAG:     In drag zone but high speed       h2g: " << h2g << "		Vm: " << Vmodul << "\n";
			}else{
				//std::cout << "NO DRAG:     Out of drag zone                  h2g: " << h2g << "		Vm: " << Vmodul << "\n";
			}
		}

}



// Gives the expected position if a dt is advanced
std::vector<double> CRock::advance2(double dt)
{
		std::vector<double> NewPosition(3,0.0);

		NewPosition[0] = Rcm[0] + Vcm[0]*dt;
		NewPosition[1] = Rcm[1] + Vcm[1]*dt;
		NewPosition[2] = Rcm[2] + Vcm[2]*dt;

		return NewPosition;
}



// Updates the speed of the block with the input velocity
void CRock::updatespeed(CTerrain& Terrain, double Vnew[3])
{
		Vcm[0] = Vnew[0];
		Vcm[1] = Vnew[1];
		Vcm[2] = Vnew[2];
}



//Funcio per calcular l'alçada respecte el terreny a la que ens trobem
double CRock::height2ground(CTerrain& Terrain)
{
		double H, dHz;
		H= Terrain.Height(Rcm[0],Rcm[1],BILINEAR);
		dHz = Rcm[2]-H;
		return dHz;
}



double CRock::height2ground(CTerrain& Terrain,std::vector<double> Position)
{
		double H, dHz;
		H= Terrain.Height(Position[0],Position[1]);
		dHz = Position[2]-H;
		return dHz;
}

#endif
