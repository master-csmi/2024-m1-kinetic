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
