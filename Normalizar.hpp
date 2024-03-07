#ifndef Normalizar_hpp
#define Normalizar_hpp
#include <vector>
#include <iostream>
using namespace std;

class Normalizar{
public:
    vector<vector<double> > normalize(const vector<vector<double> >& data);
    void estandarizar_matriz(vector<vector<double> >& matriz, const vector<double>& minValues, const vector<double>& maxValues);
    vector<vector<double> > transponer_matriz(const vector<vector<double> >& matriz);

};

#endif