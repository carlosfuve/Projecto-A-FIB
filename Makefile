CXX = g++
OPCIONS = -std=c++17
SRC_CPP = src/ParteUno/ParteUno.cpp src/ParteDos/ParteDos.cpp src/Normalizar/Normalizar.cpp
INCLUDES = -I./src/Normalizar -I./src/ParteUno -I./src/ParteDos  
 

all: main.exe

main.exe: main.cpp $(SRC_CPP)
	$(CXX) $(INCLUDES) -o main.exe main.cpp $(SRC_CPP) $(OPCIONS)

clean: 
	rm *.o
	rm *.exe