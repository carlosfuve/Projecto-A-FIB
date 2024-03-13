OPCIONS = -std=c++17

all: main.exe

main.exe: main.cpp ParteUno.cpp Normalizar.cpp ParteDos.cpp
	g++ -o main.exe main.cpp ParteUno.cpp Normalizar.cpp ParteDos.cpp $(OPCIONS)

clean: 
	rm *.o
	rm *.exe