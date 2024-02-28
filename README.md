# RockGIS

## A rockfall simulation code considering fragmentation
Universitat Politècnica de Catalunya
Departament d'Enginyeria Civil i Abbiental

## Model Description
Rockgis takes a lumped mass approach, representing blocks as single points in space with position, linear, and angular velocities. The terrain is represented by a raster file, and height at each point is computed using bilinear interpolation.
The simulation starts with kinematic conditions and a release point for each block. It follows a parabolic flight trajectory until contact with terrain, applying a rebound model upon impact. If stopping criteria are met, the block halts; otherwise, it checks for fragmentation. If fragmentation occurs, the code generates fragments based on impacting conditions, treating them as new blocks. Fragment trajectories follow power laws, with outgoing trajectories randomly assigned within a defined cone angle. For a detailed technical description, refer to scientific publications available on the project website.
The main input is a configuration file specifying parameters and paths to input files. The output includes passage counts through raster cells, final positions of stopped blocks, 3D trajectories, and impacts on protective structures. Further details on file structure are provided in the case study.

## Disclaimer
This code is provided as-is for research purposes. The university does not guarantee its accuracy or suitability for any specific application. Users assume all responsibility for its use and any consequences that may arise.

## Compilation
The code is designed to compile on Linux for both Linux and Windows platforms. Two makefiles are provided accordingly.

## Precompiled Versions
Precompiled versions of the code are available for convenience.

## References

    Project Website: https://georisk.upc.edu/en
    Scientific Articles: [List of Articles]

## Tutorial
A video tutorial demonstrating how to use the code is available.

## Aknowledgments
This work has been developed in the framework of the Georisk project, “Advances in rockfall quantitative risk analysis (QRA) incorporating developments in geomatics (GeoRisk)”, PID2019-103974RB-I00, funded by CIN/AEI/10.13039/501100011033, the Ministerio de Ciencia e Innovación and the Agencia Estatal de Investigación of Spain.
