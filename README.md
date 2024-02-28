# RockGIS

## A rockfall simulation code considering fragmentation
Universitat Politècnica de Catalunya  
Departament d'Enginyeria Civil i Abbiental  

## Model Description
Rockgis takes a lumped mass approach, representing blocks as single points in space with position, linear, and angular velocities. The terrain is represented by a raster file, and height at each point is computed using bilinear interpolation.  
The simulation starts with kinematic conditions and a release point for each block. It follows a parabolic flight trajectory until contact with terrain, applying a rebound model upon impact. If stopping criteria are met, the block halts; otherwise, it checks for fragmentation. If fragmentation occurs, the code generates fragments based on impacting conditions, treating them as new blocks. Fragment trajectories follow power laws, with outgoing trajectories randomly assigned within a defined cone angle. For a detailed technical description, refer to scientific publications available on the project website.  
The main input is a configuration file specifying parameters and paths to input files. The output includes passage counts through raster cells, final positions of stopped blocks, 3D trajectories, and impacts on protective structures. 

## Disclaimer
This code is provided as-is for research purposes. The university does not guarantee its accuracy or suitability for any specific application. Users assume all responsibility for its use and any consequences that may arise.

## Compilation
The code is designed to compile on Linux for both Linux and Windows platforms. Two makefiles are provided accordingly.

## Precompiled Versions
Precompiled versions of the code are available for convenience.

## References
Project Website: [Georisk](https://georisk.upc.edu/en)  
Scientific publications:
- Matas G. (2020) Modelling fragmentation in rockfalls. Tesis doctoral del programa de Ingeniería del Terreno. Dpto. de Ingeniería de Ingeniería Civil y Ambiental (Universitat Politècnica de Catalunya. UPC-BarcelonaTech). [Acces Thesis](https://georisk.upc.edu/es/shared/articles_pdf/Matasetal.2017Preprint.pdf)
- Matas G., Lantada N., Corominas J., Gili J.A., Ruiz-Carulla R., Prades A. (2020) Simulation of full-scale rockfall tests with a fragmentation model. Geosciences, 10 (5), 168, Special Issue "Rock Fall Hazard and Risk Assessment [Acces Article](https://doi.org/10.3390/geosciences10050168)
- Matas G., Lantada N., Corominas J. , Gili J.A., Ruiz-Carulla R., Prades A. (2017).  RockGIS: A GIS-based model for the analysis of fragmentation in rockfalls. Landslides 14(5), 1565-1578. DOI.doi:10.1007/s10346-017-0818-7 [Acces Article](https://georisk.upc.edu/es/shared/articles_pdf/Matasetal.2017Preprint.pdf)

## Tutorial
A video tutorial demonstrating how to use the code is available on project website [Video-Tutorial](https://georisk.upc.edu/en) 

## Aknowledgments
This work has been developed in the framework of the Georisk project, “Advances in rockfall quantitative risk analysis (QRA) incorporating developments in geomatics (GeoRisk)”, PID2019-103974RB-I00, funded by CIN/AEI/10.13039/501100011033, the Ministerio de Ciencia e Innovación and the Agencia Estatal de Investigación of Spain.
