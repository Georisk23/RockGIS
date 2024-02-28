# -*- coding: utf-8 -*-

import numpy as np
import sys
from qgis.core import QgsProject, QgsRasterLayer, QgsLayerTreeGroup

# Variables to modify #############################################################

# Carpeta de treball on hi ha MDE, PASSAGES etc...
path_to_DEM="Z:\\TUTORIAL\\StudyCase\\Data\\DEM.txt"
path_to_passages="Z:\\TUTORIAL\\StudyCase\\Results\\PASSAGES.txt"
Output_folder="Z:\\TUTORIAL\\StudyCase\\Results\\"
# Percentiles to calculate
Percentils_Calcul=[95]   #you can add watever percentile you want top computelike this: [90,95,96,98,99,100]
carregar_resultats=True

# ###################################################################################



# From here no need to modify
fitxerFrequencia=Output_folder+"FREQUENCY.asc"

class Matrius:
   
    def __init__(self,matriu=[],ncols=4,nrows=4,xllcorner=0,yllcorner=0,cellsize=0,nodata=-9999):
        self.matriu = matriu
        self.ncols = ncols
        self.nrows = nrows
        self.xllcorner = xllcorner
        self.yllcorner = yllcorner
        self.cellsize = cellsize
        self.nodata = nodata
        
    def zeros(self,f,c):
        
        "crea matriu de f files i c columnes amb zeros"
        
        for i in range(f):
            l=[]
            for j in range(c):
                l+=[0]
            self.matriu.append(l)
		
    
    def zeros2(self,f,c):
        
        "crea matriu de f files i c columnes amb vectors buits"

        for i in range(f):
            l=[]
            for j in range(c):
                l.append([])
            self.matriu.append(l)

    def percentil(self,p):
        
        "calcula el percentil d'una matriu"
        
        P=Matrius([],self.ncols,self.nrows,self.xllcorner,self.yllcorner,self.cellsize,0)
        f=self.nrows
        c=self.ncols
        P.zeros(f,c)
        
        for i in range(f):
            l=[]
            for j in range(c):
                array = np.array(self.matriu[i][j])
                if array.size != 0:
                    P.matriu[i][j]=np.percentile(array, p)
                else:
                    P.matriu[i][j]=0
                
        return P

    def exporta(self,arxiu):
        
        "exporta una matriu a archiu ASCII. Entrada el nom de l'arxiu entre apostrofs"
        
        arxiu=open(arxiu,'w')

        arxiu.write('ncols         '+str(self.ncols)+'\n')
        arxiu.write('nrows         '+str(self.nrows)+'\n')
        arxiu.write('xllcorner     '+str(int(self.xllcorner))+'\n')
        arxiu.write('yllcorner     '+str(int(self.yllcorner))+'\n')
        arxiu.write('cellsize      '+str(self.cellsize)+'\n')
        arxiu.write('NODATA_value  '+str(self.nodata)+'\n')
      
        for i in range(self.nrows):
            linia=''
            for j in range(self.ncols):
                #print "j:",j,"Ncols-1:",self.ncols-1
                    linia=linia+str(self.matriu[i][j])+' '               
            arxiu.write(linia+'\n')
        arxiu.close()
        
    def importa(self,arxiu):
        
        """importa a una matriu un archiu ASCII. Entrada el nom de l'arxiu entre apostrofs
        NOTA: se suposa que els fitxers d'entrada usen comes com a separador decimal, si no
        cal tocar la linia que fa el canvi!"""
        
        dades=open(arxiu, 'r')

        # Extraiem la informacio del mapa en un vector
        # info=[ncols,nrows,xllcorner,yllcorner,cellsize,nodata]
        info=[]
        for k in range(6):
            linia = dades.readline()
            liniacomes=linia.replace(',', '.')
            llista = liniacomes.split()
            if k>1 and k<5:
                info.append(float(llista[1]))
            else:
                info.append(int(llista[1]))
        #escrivim el vector a l'objecte
        self.ncols = info[0]
        self.nrows = info[1]
        self.xllcorner = info[2]
        self.yllcorner = info[3]
        self.cellsize = info[4]
        self.nodata = info[5]

        # Generem la matriu amb les dades
        self.matriu=[]
        for i in range(info[1]):

            liniai=[]
            linia=dades.readline()
            liniacomes=linia.replace(',', '.')
            llista=liniacomes.split()

            for j in range (self.ncols):
                cota=float(llista[j])
                liniai.append(cota)
            self.matriu.append(liniai)
            
        dades.close()

    def bolca(self,ConjuntBlocs,mode,TOPO):
        
        "Bolca en una matriu la informacio de trajectories(mode 0) o d'aturades (mode 1)"
        points=open('CADI/Resultats/'+NomSimulacio+'/Blocks_XY.txt','w')
        points.write('x;y;Volum[m3];Runout[m];#Font;#Block\n')
        
        VolumTotal=0.0
        xTotal=0.0
        yTotal=0.0
        for f in range (len(ConjuntBlocs)):

            for i in range(len(ConjuntBlocs[f])):
                
                if mode==0:
                    for j in range(len(ConjuntBlocs[f][i].T)):
                        self.matriu[ConjuntBlocs[f][i].T[j][0]][ConjuntBlocs[f][i].T[j][1]]+=1
                elif mode==1:
                    k=len(ConjuntBlocs[f][i].T)-1
                    if ConjuntBlocs[f][i].frag==0:
                        Ri=[ConjuntBlocs[f][i].T[0][0],ConjuntBlocs[f][i].T[0][1]]
                        Rf=[ConjuntBlocs[f][i].T[k][0],ConjuntBlocs[f][i].T[k][1]]
                        
                        self.matriu[Rf[0]][Rf[1]]+=1

                        x=TOPO[0].xllcorner+Rf[1]*TOPO[0].cellsize+0.5
                        y=TOPO[0].yllcorner+(TOPO[0].nrows-1-Rf[0])*TOPO[0].cellsize+0.5
                        runout=((Rf[0]-Ri[0])**2+(Rf[1]-Ri[1])**2)**0.5
                        points.write(str(x)+';'+str(y)+';'+str(ConjuntBlocs[f][i].Volum)+';'+str(runout)+';'+str(f)+';'+str(i)+'\n')

                        VolumTotal+=ConjuntBlocs[f][i].Volum
                        xTotal+=x*ConjuntBlocs[f][i].Volum
                        yTotal+=y*ConjuntBlocs[f][i].Volum				
						
		
                elif mode==2:
                    for j in range(len(ConjuntBlocs[f][i].E)):
                                               
                        cotapas=round(ConjuntBlocs[f][i].E[j][1]-ConjuntBlocs[f][i].E[j][0],2)
                                                
                        if self.matriu[ConjuntBlocs[f][i].T[j][0]][ConjuntBlocs[f][i].T[j][1]] < cotapas:
                            self.matriu[ConjuntBlocs[f][i].T[j][0]][ConjuntBlocs[f][i].T[j][1]]=cotapas
                else:
                    for j in range(len(ConjuntBlocs[f][i].T)):
						
                        r=0.5
                        m=(4.0/3.0)*math.pi*r*r*r*Densitat
                        v=(ConjuntBlocs[f][i].E[j][5])
                        energia=(0.5*m*v*v)/1000.0 #En kJ!!!!!!

                        self.matriu[ConjuntBlocs[f][i].T[j][0]][ConjuntBlocs[f][i].T[j][1]]+=energia
		
        points.close()

		
    def divideix(self,numero):
        
        "Divideix els valors individuals de la patriu pels de la matriu d'entreda"
        for i in range(self.nrows):
            for j in range(self.ncols):
                if numero.matriu[i][j]>0:
                    self.matriu[i][j]=self.matriu[i][j]/numero.matriu[i][j]
                else:
                    self.matriu[i][j]=0
                    
    def acumula(self,Bloc):
        
        "Modifica el model digital del terreny en funcio d'on s'atura un bloc. En desenvolupament"

        #aproximem aument de cota (opcio B: en funcio del radi)
        densitat=2700
        dH=math.sqrt((Bloc.m*3)/(4*3.141592*densitat))

        #sumem a les coordenades d'aturada el dH
        aturada=Bloc.T[len(Bloc.T)-1]
        self.matriu[aturada[0]][aturada[1]]=self.matriu[aturada[0]][aturada[1]]+10*dH #el 10 va fora (per fer proves)

 	
		
		
print("Process started")
print("Loading DEM...")
MDE=Matrius()
MDE.importa(path_to_DEM)

PASSAGES=Matrius([],MDE.ncols,MDE.nrows,MDE.xllcorner,MDE.yllcorner,MDE.cellsize,0)
PASSAGES.zeros(MDE.nrows,MDE.ncols)

PERCENTIL=Matrius([],MDE.ncols,MDE.nrows,MDE.xllcorner,MDE.yllcorner,MDE.cellsize,0)
PERCENTIL.zeros2(MDE.nrows,MDE.ncols)

PERCENTIL_H=Matrius([],MDE.ncols,MDE.nrows,MDE.xllcorner,MDE.yllcorner,MDE.cellsize,0)
PERCENTIL_H.zeros2(MDE.nrows,MDE.ncols)

fitxers_asc=[]

print("Loading trajectories...")
j=0
with open(path_to_passages) as infile:
	for line in infile:
		if(len(line)>0):
			indexos = line.rstrip().split(';')
			xIndex=int(indexos[1])
			yIndex=int(indexos[0])

			PASSAGES.matriu[xIndex][yIndex]+=1
			PERCENTIL.matriu[xIndex][yIndex].append(float(indexos[2]))
			PERCENTIL_H.matriu[xIndex][yIndex].append(float(indexos[3]))



print("Computing rasters...")
PASSAGES.exporta(fitxerFrequencia)
fitxers_asc.append(fitxerFrequencia)

for p in Percentils_Calcul:
	print("Computing ENERGIES percentile ",str(p))
	P=PERCENTIL.percentil(p)
	fitxerPercentil=Output_folder+"ENERGY_p"+str(p)+".asc"
	P.exporta(fitxerPercentil)
	fitxers_asc.append(fitxerPercentil)

for p in Percentils_Calcul:
	print("Computing Heights percentile ",str(p))
	P=PERCENTIL_H.percentil(p)
	fitxerPercentil_H=Output_folder+"HEIGHT_p"+str(p)+".asc"
	P.exporta(fitxerPercentil_H)
	fitxers_asc.append(fitxerPercentil_H)

    
    
if carregar_resultats:

    for fitxer_asc in fitxers_asc:

        raster_layer = QgsRasterLayer(fitxer_asc, os.path.basename(fitxer_asc), 'gdal')

        # Check if the layer was loaded successfully
        if raster_layer.isValid():
            # Add the layer to the map canvas
            QgsProject.instance().addMapLayer(raster_layer)
            print('ASC layer loaded successfully.')
        else:
            print('Error loading ASC layer.')
        
print("Finshed!")

