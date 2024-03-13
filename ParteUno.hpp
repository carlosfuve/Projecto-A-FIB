#ifndef ParteUno_hpp
#define ParteUno_hpp
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class ParteUno{

private: 
    int selection;
    int stop;
    int distance;

    vector<vector<double> > centr;
    vector<vector<vector<double> > > clusts;

public:
    //Creadora
    ParteUno(int selec, int dist, int st);

    //Get
    vector<vector<double> > get_centroides();
    vector<vector<vector<double> > > get_clusters();
    
    //Distancias
    double euclidian_distance(const vector<double>& x1, const vector<double>& x2);
    double manhattan_distance(const vector<double>& x1, const vector<double>& x2);

    int cluster_cercano(const vector<double>& point, const vector<vector<double> >& centroids);

    //Stop
    bool comparar_centroids(const vector<vector<double> >& antCentroids, const vector<vector<double> >& newCentroids);

    //Selection
    vector<vector<double> > kmeans_plus_plus(const vector<vector<double> >& data, int k);


    void update_centroids(vector<vector<double> >& centroids, const vector<vector<vector<double> > >& clusters);

    //Algortimo
    void lloyd_function(const vector<vector<double> >& data, int k, int maxIterations);
    
};


#endif