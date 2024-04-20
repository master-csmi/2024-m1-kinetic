# Kinetic project

## team member :

InfmathAxel : Demuth Axel

Dorian : Geraldes Pereira Dorian

## Compilation of the project :

to compile the project you need to have boost already installed 

to compile the project you need to do the following step :
```
# create a virtual environnement 
python3 -m venv .venv

# activate the virtual environnement 
source .venv/bin/activate

# instal conan 
pip3 install -r requirements.txt

#configure cmake

cmake --preset default

#Execute the build 

cmake --build --preset default
```
