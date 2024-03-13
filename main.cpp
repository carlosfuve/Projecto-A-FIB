#include <iostream>
#include <fstream> 
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <regex>
#include <filesystem>
#include "Normalizar.hpp"
#include "ParteUno.hpp"
using namespace std;
namespace fs = filesystem;

#define DIRECTORIO "datasets"
#define SELECTION 0
#define DISTANCE 0
#define STOP 0


int contar_datasets() {
    int num_datasets = 1;
    for (const auto& entry : fs::directory_iterator(DIRECTORIO)) {
        if (entry.path().extension() == ".csv") num_datasets++;
    }
    return num_datasets;
}

vector<string> buscar_dataset_name(int num_datasets){
    vector<string> rutas_archivos(num_datasets);
    regex patron_num("\\d+");
    smatch numeros;
    for (const auto& entry : fs::directory_iterator(DIRECTORIO)) {
        if(entry.path().extension() == ".csv"){
            string archivo = entry.path().filename();
            fs::path ruta_completa = fs::path(DIRECTORIO) / archivo;
            regex_search(archivo, numeros, patron_num);
            int index = stoi(numeros[0])%num_datasets;
            rutas_archivos[index] = ruta_completa;
        }
    }
    return rutas_archivos;
}

vector<bool> buscar_si_tiene_etiqueta(int num_datasets){
    vector<bool> tiene_etiqueta(num_datasets,true);
    regex patron_etq("Etiquetat: No");
    smatch etiqueta;

    regex patron_num("\\d+");
    smatch numeros;

    for (const auto& entry : fs::directory_iterator(DIRECTORIO)) {
        if (entry.path().extension() == ".txt") {
            string archivo = entry.path().filename();
            fs::path filename = fs::path(DIRECTORIO) / archivo;

            regex_search(archivo, numeros, patron_num);
            int index_etq = stoi(numeros[0])%num_datasets;

            ifstream file(filename);
            string line;
            while(getline(file, line)){
                stringstream ss(line);
                if (regex_search(line, etiqueta, patron_etq)) {
                    tiene_etiqueta[index_etq] = false;
                    break;
                }
            }
            file.close();
        }
    }
    return tiene_etiqueta;
}




int main(){
    int n_datasets = contar_datasets();

    vector<string> rutas_archivos = buscar_dataset_name(n_datasets);
    vector<bool> tiene_etiqueta = buscar_si_tiene_etiqueta(n_datasets);

    for(int i = 1; i < n_datasets; i++){
        //Condición para evaluar solo el dataset 2, para hacer pruebas
        if(i != 2) continue;

        string name_dataset = rutas_archivos[i];

        ifstream file(name_dataset);
        if (!file.is_open()) return 1;

        vector<vector<double> > data;
        string line;
        while(getline(file, line)){
            stringstream ss(line);
            vector<double> row;
            string value;
            while(getline(ss,value,';')) {
                stringstream ss(value);
                /*
                Configurar la configuración regional del stringstream 
                para que reconozca la coma como separador decimal
                */ 
                ss.imbue(locale(""));
                double val;
                ss >> val;
                row.push_back(val);
            }
            if (tiene_etiqueta[i]){
                //TO DO: guardar etiqueta

                // Solo si tiene etiqueta
                vector<double> new_row = vector<double>(row.begin(),row.end()-1);
                data.push_back(new_row);
            }
            else data.push_back(row);
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

        vector<vector<double> > centroides = pt1.get_centroides();
        vector<vector<vector<double> > > clusters = pt1.get_clusters();
    }

    return 0;

}



