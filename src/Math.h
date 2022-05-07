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
    vector<double> result;

    map <string, double> variables;
    map<string, int> functions;
   
    vector<int> plus, minus, mult, div, power;

    /* the dictionary of valid operators */
    map<string, vector<int>> operators;
    
    /* Methods */
    Math();
    void getInput();
    void setVarValues(double x,double y, double z);
    void parseToPostFix(double x, double y, double z);
    string swapVariablesWithValues(string eq);
    string shunting(string eq);
    double evalPostfix(string eq);
    double compute(vector<double> args, string op);
    bool isNumber(string token);
};
#endif
