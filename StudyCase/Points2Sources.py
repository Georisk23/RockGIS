#Parametros a configurar
layerName='Sources'
Hz=2.0              # Altura respecto el terreno
Vx=0.01             # Velocidad linial inicial en el eje X
Vy=0.0              # Velocidad linial inicial en el eje Y
Vz=0.0              # Velocidad linial inicial en el eje Z
NumBlocks=10         # Numero de bloques a lanzar de cada fuente
Volume=1            # Volumen de los bloques
# Ruta fichero de salida
arxiu=open("Z:\sources.txt",'w+')

# Inicio del calculo
layer = QgsProject.instance().mapLayersByName(layerName)[0]
num_features = layer.featureCount()
id=1
for feature in layer.getFeatures():
    print(round((id/num_features)*100,2)," %")
    arxiu.write('## \n')
    arxiu.write('ID = '+ str(id) +'\n')
    arxiu.write('Rx = '+ str(feature.geometry().asPoint()[0]) +'\n')
    arxiu.write('Ry = '+ str(feature.geometry().asPoint()[1]) +'\n')
    arxiu.write('Hz = '+ str(Hz) +'\n')
    arxiu.write('Vx = '+ str(Vx) +'\n')
    arxiu.write('Vy = '+ str(Vy) +'\n')
    arxiu.write('Vz = '+ str(Vz) +'\n')
    arxiu.write('NumBlocks = '+ str(NumBlocks) +'\n')
    arxiu.write(str(Volume) +'\n%%\n\n')
    
    id+=1

arxiu.close()


