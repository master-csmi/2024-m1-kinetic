# Kinetic project

## team member :

InfmathAxel : Demuth Axel

Dorian : Geraldes Pereira Dorian

## Compilation of the project :



to compile the project you need to do the following step :
```
# create a virtual environnement 
python3 -m venv .venv

# activate the virtual environnement 
source .venv/bin/activate

# instal conan 
pip3 install -r requirements.txt

#For this project Boost libraries need to be installed :
#if its not already intalled you need to do it before the cmake configuration 


#configure cmake

cmake --preset default

#Execute the build 

cmake --build --preset default
```
## Execution of the project :
the different parameters important to change are :

-data 

-dist 

-minp 

-stl 1  (if you want to use an stl file as imput)

to execute the project you can use 
```
./build/default/bin/kinetic 

# some exemple with parameters

./build/default/bin/kinetic -data data/flame.ply -dist 0.3 -minp 50 -regangle 5
./build/default/bin/kinetic -data data/3zones_normal5.ply -dist 0.2 -minp 500
./build/default/bin/kinetic -data data/3zones_normal_apres.ply -dist 0.2 -minp 500
./build/default/bin/kinetic -data data/ACJasmin_Mesh2_test2.ply -dist 0.3 -minp 300
./build/default/bin/kinetic -data data/mesh_ThreeZones.stl -stl 1
```