#include "ParteUno.hpp"

ParteUno::ParteUno(int selec, int dist, int st) : distance(dist), stop(st), selection(selec) {}

//Get
vector<vector<double> > ParteUno::get_centroides(){
    return this -> centr;
}

vector<vector<vector<double> > > ParteUno::get_clusters(){
    return this -> clusts;
}

//Distancias
double ParteUno::euclidian_distance(const vector<double>& x1, const vector<double>& x2){
    double sum = 0.0;
    for(int i = 0; i < x1.size(); i++){
        sum += pow(x1[i] - x2[i], 2);
    }
    return sqrt(sum);
}

double ParteUno::manhattan_distance(const vector<double>& x1, const vector<double>& x2){
    double sum = 0.0;
    for(int i = 0; i < x1.size(); i++){
        sum += abs(x1[i] - x2[i]);
    }
    return sum;
}

int ParteUno::cluster_cercano(const vector<double>& point, const vector<vector<double> >& centroids){
    int indexCentroid = 0;
    double minDistance;
    if (this -> distance == 0) minDistance = euclidian_distance(point, centroids[0]);
    else minDistance = manhattan_distance(point,centroids[0]);
    for(int j = 1; j < centroids.size(); j++){
        double posMinDistance;
        if(this -> distance == 0) posMinDistance = euclidian_distance(point, centroids[j]);
        else posMinDistance = manhattan_distance(point,centroids[j]);

        if (posMinDistance < minDistance){
            indexCentroid = j;
            minDistance = posMinDistance;
        }
    }
    return indexCentroid;
}

//Stop 
bool ParteUno::comparar_centroids(const vector<vector<double> >& antCentroids, const vector<vector<double> >& newCentroids){
    for(int i = 0; i < antCentroids.size(); i++){
        for(int j = 0; j < antCentroids[i].size(); j++){
            if(antCentroids[i][j] != newCentroids[i][j]) return false;
        }
    }
    return true;
}

//Selection
vector<vector<double> > ParteUno::kmeans_plus_plus(const vector<vector<double> >& data, int k){ 
   //Se elige un punto aleatorio como el primer centroide
    vector<vector<double> > centroids;
    centroids.push_back(data[rand()%data.size()]);

    while(centroids.size() < k){
        double totalDistance = 0.0;
        //Crea un vector con el número máximo que puede ser representado por un double
        vector<double> minDistance(data.size(), numeric_limits<double>::max());

        //Se calcula la distancia minima entre los puntos y los centroides seleccionados
        for(int i = 0; i < data.size(); i++){
            for(int j = 0; j < centroids.size(); j++){
                double distance = pow(euclidian_distance(data[i],centroids[j]),2);
                minDistance[i] = min(minDistance[i],distance);
            }
            totalDistance += minDistance[i];
        }

        /*Seleccionar siguiente centroide con probabilidad ponderada por distancias,
        se multiplica la distancia total entre un valor entre 0 y 1
        para posteriormente seleccionar el punto que sumando todos sus anteriores supera a la distancia prob*/
        double prob = ((double) rand() / RAND_MAX) * totalDistance;
        double sum = 0.0;
        int nexCentroid = -1;
        for(int i = 0; i < data.size(); i++){
            sum += minDistance[i];
            if (sum >= prob){
                nexCentroid = i;
                break;
            }
        }
        
        if (nexCentroid != -1) centroids.push_back(data[nexCentroid]);

    }
    return centroids;
}



void ParteUno::update_centroids(vector<vector<double> >& centroids, const vector<vector<vector<double> > >& clusters){
    for(int i = 0; i < clusters.size(); i++){
        vector<double> sumCentroids(centroids[i].size(),0);
        //Para cada punto del cluster
        for(int j = 0; j < clusters[i].size(); j++){
            for(int t = 0; t < clusters[i][j].size(); t++) sumCentroids[t] += clusters[i][j][t];
        }

        //Actualizar d coordenadas
        if(!clusters[i].empty()){
            for(int d = 0; d < centroids[i].size(); d++){
                centroids[i][d] = sumCentroids[d] / clusters[i].size();
            }
        }
    }
}

//Algortimo
void ParteUno::lloyd_function(const vector<vector<double> >& data, int k, int maxIterations){

    vector<vector<double> > centroids;
    if (this -> selection == 0) for(int i = 0; i < k; i++) centroids.push_back(data[rand()%data.size()]);
    else centroids = kmeans_plus_plus(data,k);

    if(this -> stop == 0){
        for(int iter = 0; iter < maxIterations; iter++){
            //Cada iteración se crea un cluster nuevo
            vector<vector<vector<double> > > clusters(k);
            for(int n = 0; n < data.size(); n++){
                int indexCluster = cluster_cercano(data[n], centroids); 
                clusters[indexCluster].push_back(data[n]);
            }

            //Actualizar centroides mediante la media, valor medio en el conjunto de datos
            update_centroids(centroids,clusters);

            if(iter == maxIterations-1) {
                this -> centr = centroids;
                this -> clusts = clusters;
            }
        }
    }
    else {
        vector<vector<double> > antCentroids;
        do {
            antCentroids = centroids;

            vector<vector<vector<double> > > clusters(k);
            for(int n = 0; n < data.size(); n++){
                int indexCluster = cluster_cercano(data[n], centroids); 
                clusters[indexCluster].push_back(data[n]);
            }

            update_centroids(centroids,clusters);

            this -> clusts = clusters;

        }while (!comparar_centroids(antCentroids,centroids));

        this -> centr = antCentroids;
    }
}