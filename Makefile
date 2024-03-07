
all: main.exe

main.exe: main.cpp ParteUno.cpp Normalizar.cpp
	g++ -o main.exe main.cpp ParteUno.cpp Normalizar.cpp

clean: 
	rm *.exe