#ifndef MATH_H
#define MATH_H

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <cmath>
#include <string>
#include <algorithm>
using namespace std;
/*
    TODO: 
        1.  split this into files, parse, compute, getEquations,
*/

class Math{
public:
    vector<string> equations;
    vector<string> swapped;
    double* resultArray;

    map<string, double> variables;
    map<string, int> functions;
   
    vector<int> plus, minus, mult, div, power;

    /* the dictionary of valid operators */
    map<string, vector<int>> operators;
    
    int dimensions;

    /* Methods */
    Math();
    void getInput();
    int getDimensionality();
    double* parseToPostFix(double x, double y, double z);

    string shunting(string eq);
    double evalPostfix(string eq);

private:
    void setVarValues(double x, double y, double z);
    string swapVariablesWithValues(string eq);
    double compute(vector<double> args, string op);
    bool isNumber(string token);
};
#endif
