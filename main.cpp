#include <iostream>
#include <fstream> 
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <regex>
#include <filesystem>
#include <ctime>
#include "Normalizar.hpp"
#include "ParteUno.hpp"
#include "ParteDos.hpp"
using namespace std;
namespace fs = filesystem;

#define DIRECTORIO "datasets"
#define MAX_ITER 200


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


void grafica_del_codo(const vector<int>& k_values, const vector<vector<double> >& normData, const vector<int>& variante){
        vector<double> y_values(k_values.size()+1);
        int n = normData.size();
        for(int i = 0; i < k_values.size(); i++){
            ParteUno pt1(variante[0], variante[1], variante[2]);
            pt1.lloyd_function(normData,k_values[i],MAX_ITER);

            vector<vector<vector<double> > > clusters = pt1.get_clusters();
            vector<vector<double> > centroides = pt1.get_centroides();

            double dist_total = 0.0;
            for(int c = 0; c < clusters.size();c++){
                for(int n=0; n < clusters[c].size();n++){
                    dist_total += pt1.euclidian_distance(clusters[c][n],centroides[c]);
                }
            }
            y_values[k_values[i]] = round(dist_total/k_values[i]);
        }

        cout << "Valores de la k: " << endl;
        for(int i = 0; i < k_values.size(); i++) cout << k_values[i] << " ";
        cout << endl;

        cout << "Valores correspondientes a la k: " << endl;
        for(int i = 1; i < y_values.size(); i++) cout << y_values[i] << " ";
        cout << endl;

}

void grafica_del_codo_etq(const vector<int>& k_values, const vector<vector<double> >& normData, const vector<int>& variante){
        vector<double> y_values(k_values.size()+1);
        int n = normData.size();
        for(int i = 0; i < k_values.size(); i++){
            ParteUno pt1(variante[0], variante[1], variante[2]);
            pt1.lloyd_function_etq(normData,k_values[i],MAX_ITER);
            vector<vector<vector<double> > > clusters = pt1.get_clusters();
            vector<vector<double> > centroides = pt1.get_centroides();

            double dist_total = 0.0;
            for(int c = 0; c < clusters.size();c++){
                for(int n = 0; n < clusters[c].size();n++){
                    vector<double> data_sin_eqt = vector<double>(clusters[c][n].begin()+1,clusters[c][n].end());
                    dist_total += pt1.euclidian_distance(data_sin_eqt,centroides[c]);
                }
            }
            y_values[k_values[i]] = round(dist_total/k_values[i]);
        }

        cout << "Valores de la k: " << endl;
        for(int i = 0; i < k_values.size(); i++) cout << k_values[i] << " ";
        cout << endl;

        cout << "Valores correspondientes a la k: " << endl;
        for(int i = 1; i < y_values.size(); i++) cout << y_values[i] << " ";
        cout << endl;

}

void escribe_salida(const vector<int>& variante, double cal_score, double davies_score, double duration){
    string selection, distancia, stop;
    if (variante[0] == 0) selection = "random";
    else selection = "k-means++";

    if (variante[1] == 0) distancia = "euclidean";
    else distancia = "manhattan";

    if (variante[2] == 0) stop = "max_iteraciones";
    else stop = "hasta que los centroides no cambien";

    cout << "Duración del algortimo: " << duration << " segundos" << endl;
    cout << "Con metodo de selecion: " << selection << ", ";
    cout << "con metodo de distancia: " << distancia << " y  metodo de stop: " << stop;
    cout << " se obtiene un calinski_score: " << cal_score;
    cout << " y se obtine un davies score: " << davies_score << endl;
}

void escribe_salida_etq(const vector<int>& variante, double rand_score, double duration){
    string selection, distancia, stop;
    if (variante[0] == 0) selection = "random";
    else selection = "k-means++";

    if (variante[1] == 0) distancia = "euclidean";
    else distancia = "manhattan";

    if (variante[2] == 0) stop = "max_iteraciones";
    else stop = "hasta que los centroides no cambien";

    cout << "Duración del algortimo: " << duration << " segundos" << endl;
    cout << "Con metodo de selecion: " << selection << ", ";
    cout << "con metodo de distancia: " << distancia << " y  metodo de stop: " << stop;
    cout << " se obtiene un avg_rand_score: " << rand_score << endl;
}


int main(){
    int n_datasets = contar_datasets();

    vector<string> rutas_archivos = buscar_dataset_name(n_datasets);
    vector<bool> tiene_etiqueta = buscar_si_tiene_etiqueta(n_datasets);
    vector<int> k_values = {1,2,3,4,5,6,7,8,9,10};
    vector<vector<int> > variantes = {{0,0,0},{0,0,1},{0,1,0},{0,1,1},{1,0,0},{1,0,1},{1,1,0},{1,1,1}};
    vector<int> mejor_k_datset = {0, 7, 2, 2, 3, 4, 3};

    for(int i = 1; i < n_datasets; i++){
        //Condición para evaluar solo el dataset elegido, para hacer pruebas
        if(i != 6) continue;

        string name_dataset = rutas_archivos[i];
        ifstream file(name_dataset);
        if (!file.is_open()) return 1;

        vector<vector<double> > data;
        vector<int> etiquetas;
        string line;
        while(getline(file, line)){
            stringstream ss(line);
            vector<double> row;
            string value;
            while(getline(ss,value,';')) {
                stringstream ss(value);
                //Configurar la configuración regional del stringstream para que reconozca la coma como separador decimal 
                ss.imbue(locale(""));
                double val;
                ss >> val;
                row.push_back(val);
            }
            // Solo si tiene etiqueta
            if (tiene_etiqueta[i]){
                etiquetas.push_back(row[row.size()-1]);
                vector<double> new_row = vector<double>(row.begin(),row.end()-1);
                data.push_back(new_row);
            }
            else data.push_back(row);
        }
        file.close();

        int k = mejor_k_datset[i];
        cout << "Número de clusters: " <<  k << endl;
        cout << endl;

        Normalizar norm;
        vector<vector<double> > normData = norm.normalize(data);

        if (tiene_etiqueta[i]){
            //Añadir las etiquetas a la columna 0 de los puntos
            for(int i = 0; i < normData.size();i++) normData[i].insert(normData[i].begin(),etiquetas[i]);    

            double max_rand_score_total = 0.0;
            double variante_max_score;
            for(int i = 0; i < variantes.size(); i++){
                //Algortimo de clustering
                ParteUno pt1(variantes[i][0],variantes[i][1],variantes[i][2]);
                clock_t start = clock();
                pt1.lloyd_function_etq(normData,k,MAX_ITER);
                
                vector<vector<vector<double> > > clusters = pt1.get_clusters();

                //Calcular la medida externa, Rand Index, para cada par de clusters
                ParteDos pt2;
                double avg_rand_score = 0.0;
                double binomial = (clusters.size() * (clusters.size()-1))/2;
                for(int i = 0; i < clusters.size(); i++){
                    for(int j = i+1; j < clusters.size();j++){
                        double rand_score = pt2.rand_index(clusters[i],i,clusters[j],j);
                        avg_rand_score += rand_score;
                    }
                }
                avg_rand_score = avg_rand_score/binomial;
                clock_t end = clock();

                if(avg_rand_score > max_rand_score_total){
                    max_rand_score_total = avg_rand_score;
                    variante_max_score = i;
                }

                // Calcular la duración
                double duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;

                escribe_salida_etq(variantes[i],avg_rand_score,duration);
            }
            cout << endl;

            //Parte de la gráfica del codo
            grafica_del_codo_etq(k_values,normData,variantes[variante_max_score]);

        }
        else{
            double max_calinksi_score_total = 0.0;
            double variante_max_score;
            for(int i = 0; i < variantes.size(); i++){
                //Algortimo de clustering
                ParteUno pt1(variantes[i][0],variantes[i][1],variantes[i][2]);
                clock_t start = clock();
                pt1.lloyd_function_etq(normData,k,MAX_ITER);
                vector<vector<vector<double> > > clusters = pt1.get_clusters();
                vector<vector<double> > centroides = pt1.get_centroides();


                //Calcular la medida interna
                ParteDos pt2;
                //Se busca maximizar el calinski_score
                double cal_score = pt2.calinski_harabasz(normData,centroides,clusters);
                //Se busca minimizar el davies_bouldin_score
                double davies_score = pt2.davies_bouldin(clusters,centroides);
                clock_t end = clock();

                if(cal_score > max_calinksi_score_total){
                    max_calinksi_score_total = cal_score;
                    variante_max_score = i;
                }

                double duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;
                escribe_salida(variantes[i],cal_score,davies_score,duration);
            }
            cout << endl;
            
            //Parte de la gráfica del codo
            grafica_del_codo(k_values,normData,variantes[variante_max_score]);
        }
    }

    return 0;
}



