#include <iostream>
#include <fstream> 
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include "Normalizar.hpp"
#include "ParteUno.hpp"
using namespace std;

#define SELECTION 0
#define DISTANCE 0
#define STOP 0


int main(){
    string filename = "datasets/Dataset2.csv";

     // Abrir el archivo CSV
    ifstream file(filename);

    // Verificar si el archivo se abrió correctamente
    if (!file.is_open()) {
        cout << "Error al abrir el archivo " << filename << endl;
        return 1;
    }

    vector<vector<double> > data;
    string line;
    while(getline(file, line)){
        stringstream ss(line);
        vector<double> row;
        string value;

        while(getline(ss,value,';')) {
            stringstream ss(value);
            /*
            Configurar la configuración regional 
            del stringstream para que reconozca 
            la coma como separador decimal
            */ 
            ss.imbue(locale(""));
            double val;
            ss >> val;
            row.push_back(val);
        }
        // Solo si tiene etiqueta...
        vector<double> new_row = vector<double>(row.begin(),row.end()-1);
        
        data.push_back(new_row);
    }
    file.close();

    //Normaliza bien
    Normalizar norm;
    vector<vector<double> > normData = norm.normalize(data);

    //Llamar a la función de lloyd
    int k = 2;
    int maxIter = 200;
    ParteUno pt1(SELECTION, DISTANCE, STOP);
    pt1.lloyd_function(normData,k,maxIter);



    return 0;

}



