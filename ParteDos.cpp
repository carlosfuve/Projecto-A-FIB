#include "ParteDos.hpp"


double ParteDos::euclidian_distance(const vector<double>& x1, const vector<double>& x2){
    double sum = 0.0;
    for(int i = 0; i < x1.size(); i++){
        sum += pow(x1[i] - x2[i], 2);
    }
    return sqrt(sum);
}

double ParteDos::calcular_dispersion(const vector<vector<double>>& cluster, const vector<double>& centroid){
    double dispersion = 0.0;
    for (const auto& point : cluster) {
        dispersion += euclidian_distance(point, centroid);
    }
    return dispersion / cluster.size();
}

//Medida interna
double ParteDos::calinski_harabasz(const vector<vector<double> >& data ,const vector<vector<double> >& centroides, 
                                    const vector<vector<vector<double> > >& clusters){

    int n = data.size();
    int k = centroides.size();

    //Calculo del centroide global, mediante mediana aritmetica
    vector<double> global_centroide(data[0].size(),0.0);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < data[i].size(); j++){
            global_centroide[j] += data[i][j];
        }
    }
    for(int i = 0; i < global_centroide.size(); i++) global_centroide[i] = global_centroide[i]/n;

    //Calculo de distancias de todos los centroides con respecto al centroide global
    double distance_centroids_global_distance = 0.0;
    for(int i = 0; i < k; i++){
        distance_centroids_global_distance += pow(euclidian_distance(centroides[i],global_centroide),2);
    }

    //Calculo de variancia de cada cluster
    double variance_clusters = 0.0;
    for(int i = 0; i < k; i++){
        for(int j = 0; j < clusters[i].size(); j++){
            for(int m = 0; m < clusters[i][m].size();m++){
                variance_clusters += pow(clusters[i][j][m] - centroides[i][m],2);
            }
        }
    }

    double factor_cluster = distance_centroids_global_distance / variance_clusters;
    double factor = (n-k) / (k-1);
    double calinski_harabasz_score = factor_cluster * factor;
    return calinski_harabasz_score;
}


double ParteDos::davies_bouldin(const vector<vector<vector<double> > >& clusters, const vector<vector<double> >& centroides) {

    int k = clusters.size();

    double davies_bouldin_score = 0.0;
    for(int i = 0; i < k; i++){
        double max_value_cluster = 0.0;
        double dispersion_cluster_i = calcular_dispersion(clusters[i],centroides[i]);

        for(int j = 0; j < k; j++){
            if (i == j) continue;

            double dispersion_cluster_j = calcular_dispersion(clusters[j],centroides[j]);
            double distance_centroides_ij = euclidian_distance(centroides[i],centroides[j]);

            double value = (dispersion_cluster_i + dispersion_cluster_j) / distance_centroides_ij;
            if (value > max_value_cluster) max_value_cluster = value;
        }
        davies_bouldin_score += max_value_cluster;
    }
    
    return davies_bouldin_score/k;
}

//Medida externa
double ParteDos::rand_index(const vector<vector<double> >& cluster1, int etq_c1,
                                const vector<vector<double> >& cluster2, int etq_c2){
                                    
    int n = min(cluster1.size(),cluster2.size());
    double a = 0.0;
    double b = 0.0;
    double binomial = (n * (n-1))/2;
    
    for(int i = 0; i < n; i++){
        int etq_p1 = static_cast<int>(cluster1[i][0]);
        for(int j = i+1; j < n; j++){
            int etq_p2 = static_cast<int>(cluster2[j][0]);

            if (etq_p1 == etq_c1 && etq_p2 == etq_c2) a++;
            else if (etq_p1 != etq_c1 && etq_p2 != etq_c2) b++;
        }
    }
    
    double resultado = (a+b)/binomial;
    resultado = trunc(resultado * 100) / 100;
    return resultado;
}