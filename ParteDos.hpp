#ifndef ParteDos_hpp
#define ParteDos_hpp
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class ParteDos{
private: 

public:

    double euclidian_distance(const vector<double>& x1, const vector<double>& x2);
    double calcular_dispersion(const vector<vector<double>>& cluster, const vector<double>& centroid); 

    //Medida interna
    double calinski_harabasz(const vector<vector<double> >& data ,const vector<vector<double> >& centroides, 
                            const vector<vector<vector<double> > >& clusters);
    
    double davies_bouldin(const vector<vector<vector<double> > >& clusters, const vector<vector<double> >& centroides);

    //Medida externa
    double rand_index(const vector<vector<double> >& cluster1, int etq_c1,const vector<vector<double> >& cluster2, int etq_c2);

};

#endif