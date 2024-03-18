#include "Normalizar.hpp"


vector<vector<double> > Normalizar::transponer_matriz(const vector<vector<double> >& matriz){
    int filas = matriz.size();
    int columnas = matriz[0].size();
    vector<vector<double> > transpuesta(columnas, vector<double>(filas));

    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            transpuesta[j][i] = matriz[i][j];
        }
    }
    return transpuesta;
}

void Normalizar::estandarizar_matriz(vector<vector<double> >& matriz, const vector<double>& minValues, const vector<double>& maxValues){
    for(int i = 0; i < matriz.size(); i++){
        double rango = maxValues[i] - minValues[i];
        double minVal = minValues[i];
        for(int j = 0; j < matriz[0].size(); j++){
            if (rango == 0) matriz[i][j] = 0.0;
            else matriz[i][j] = (matriz[i][j] - minVal) / rango;
        }
    }
}

vector<vector<double> > Normalizar::normalize(const vector<vector<double> >& data){
    vector<double> minValues(data[0].size());
    vector<double> maxValues(data[0].size());
    vector<vector<double> > transpuesta = transponer_matriz(data);

    for(int i = 0; i < data[0].size(); i++){
        minValues[i] = *min_element(transpuesta[i].begin(),transpuesta[i].end());
        maxValues[i] = *max_element(transpuesta[i].begin(),transpuesta[i].end());
    }

    estandarizar_matriz(transpuesta, minValues, maxValues);
    vector<vector<double> > matrix_normalize = transponer_matriz(transpuesta);
    return matrix_normalize;
}
