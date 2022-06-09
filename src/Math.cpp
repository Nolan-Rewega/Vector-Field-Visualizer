#include"Math.h"

Math::Math(){
    /* legal variables */
    variables = {
        {"x", 3}, {"y", 2}, {"z", 1}, {"w", 0}
    };
    
    /* vector list of common math functions. */
    functions = {
        {"sin",     1},     {"cos",     1},     {"tan",     1}, 
        {"asin",    1},     {"acos",    1},     {"atan",    1},
        {"csc",     1},     {"sec",     1},     {"cot",     1},
        {"acsc",    1},     {"asec",    1},     {"acot",    1},
        {"!",       1},     {"ln",      1},     {"log",     1}
    };
   
    /* priority and evalutation type of operators */
    plus = {1, 1};  minus = {1, 1};
    mult = {2, 1};  div = {2, 1}; 
    power= {3, 0};

    /* the dictionary of valid operators */
    operators = {
        {"+", plus},    {"-", minus},   {"*", mult},
        {"/", div},     {"^", power}
    };
}




void Math::getInput(){
    // -- get user input, this should be done seperatly
    string input;
    cout << "Enter a equation or EoF (CTRL + D)" << endl; 
    cout << "e.g: ( 3 * x + y ^ 2 )" << endl;
    for(int i = 0; i < 3; i++){
        cout << "Enter Equations " << i+1 << ". \n";
        getline(cin, input);
        if(input.size() > 0){
            equations.push_back(input);
        }
    }
    swapped.resize(equations.size(), " ");
    resultArray = (double*)calloc(equations.size(), sizeof(double));
}
   
   
double* Math::parseToPostFix(double x, double y, double z){
    setVarValues(x, y, z);
    for(unsigned long e = 0; e < equations.size(); e++){
        /*
            run dijkstras shunnting yard algorithm on equation e.
            replace current equation with the postfix version
        */
        swapped[e] = swapVariablesWithValues(equations[e]);
        swapped[e] = shunting(swapped[e]);
        resultArray[e] = evalPostfix(swapped[e]);
    }
    return resultArray;
}

string Math::shunting(string eq){
    
    queue<string> output;
    stack<string> op_stack;
    string delim = " "; string token;
    size_t idx = 0;
    
    while( (idx = eq.find(delim)) != std::string::npos){
        // get each individual component
        token = eq.substr(0, idx);
        eq.erase(0, idx + delim.length());
        
        if (isNumber(token)) {output.push(token);}
        else if (functions.find(token) != functions.end()){
            op_stack.push(token);
        }
        else if (operators.find(token) != operators.end()){
            if (!op_stack.empty()){
                string top = op_stack.top();
                while( (operators.count(top) > 0) && ((operators[top][0] > operators[token][0]) || \
                        (operators[top][0] == operators[token][0] && operators[token][1] == 1))  \
                        && top != "(" && !op_stack.empty() ){
                    output.push(top);
                    op_stack.pop();
                    if(!op_stack.empty()){ top = op_stack.top(); }
                }
            }
            op_stack.push(token);
        }
        else if (token == "(") {op_stack.push(token);}
        else if (token == ")"){
            while(op_stack.top() != "(" && !op_stack.empty()){
                output.push(op_stack.top());
                op_stack.pop();
            }
            if (op_stack.top() == "(") {op_stack.pop();}
            if (!op_stack.empty()){
                if (functions.find(op_stack.top()) != functions.end()){
                    output.push(op_stack.top());
                    op_stack.pop();
                }
            }
        }
    }
        
    while (! op_stack.empty()){
        string top = op_stack.top();
        op_stack.pop();
        if (top == ")" || top == "(") { cout << "MISMATCH PARENS! " << endl; }
        else { output.push(top); }
    }
    
    // create the postfix String
    string postfix;
    while(!output.empty()) { 
        postfix = postfix + output.front() + " ";
        output.pop();
    }
    return postfix;
}
        

double Math::evalPostfix(string eq){
    //cout << eq << endl;
    stack<double> num_stack;
    vector<double> args;
    string delim = " ";
    string token; 
    size_t idx;

    while( (idx = eq.find(delim)) != std::string::npos){
        // get each individual component
        token = eq.substr(0, idx);
        eq.erase(0, idx + delim.length());

        if (isNumber(token)) { num_stack.push(stod(token));}   
        else{
            int arg_limit = 0;
            if(operators.find(token) != operators.end()){arg_limit = 2;}
            else { arg_limit = functions[token];}
            for (int i = 0; i < arg_limit; i++ ){
                args.push_back(num_stack.top());
                num_stack.pop();
            }
        num_stack.push(compute(args, token));
        args.clear();
        }

    } 
    return num_stack.top();
};





// ~~~~~~~~~ PRIVATE METHODS ~~~~~~~~~~~~~~

string Math::swapVariablesWithValues(string eq) {
    string copy = eq; string delim = " "; string token;
    size_t idx = 0; size_t pos = 0;
    //cout << "START: " << eq << endl;

    while ((idx = eq.find(delim)) != std::string::npos) {
        // get each individual component
        token = eq.substr(0, idx);
        eq.erase(0, idx + delim.length());

        pos = copy.find(token);
        while (pos != std::string::npos && variables.find(token) != variables.end()) {
            // get each individual component
            string substitute = to_string(variables[token]);
            copy.replace(pos, token.size(), substitute);
            pos = copy.find(token, pos + substitute.size());
        }
    }
    //cout << "SWAPPED: " << copy << endl;
    return copy;
}


// used in evaluation of the postfix expression
double Math::compute(vector<double> args, string op){

    if(op == "+"){return args[1] + args[0];}
    else if(op == "-"){return args[1] - args[0];}
    else if(op == "*"){return args[1] * args[0];}
    else if(op == "/"){return args[1] / args[0];}
    else if(op == "^"){return pow(args[1], args[0]);}
    else if(op == "!"){
        double result = args[0];
        while(args[0] > 1){
            args[0]--;
            result = result * args[0];
        }
        return result;
    }
    
    else if(op == "sin"){return sin( args[0]);}
    else if(op == "cos"){return cos( args[0]);}
    else if(op == "tan"){return tan( args[0]);}
    else if(op == "sec"){return 1/(sin( args[0]));}
    else if(op == "csc"){return 1/(cos( args[0]));}
    else if(op == "cot"){return cos(args[0]) / sin(args[0]);}   
    else if(op == "asin"){return asin( args[0]);}
    else if(op == "acos"){return acos( args[0]);}
    else if(op == "atan"){return atan( args[0]);}
    else if(op == "asec"){return acos(1 / args[0]);}
    else if(op == "acsc"){return asin(1 / args[0]);}
    else if(op == "acot"){return atan(-args[0]) + 3.1415 / 2;}
    else if(op == "log"){return log10(args[0]);}
    else if(op == "ln"){return log(args[0]);}
    else {
        cout << "COMPUTE ERROR" << endl;
        return -1;
    }
}
 
void Math::setVarValues(double x, double y, double z) {
    variables["x"] = x;
    variables["y"] = y;
    variables["z"] = z;
}

/* helper function */
bool Math::isNumber(string token){
    int numeric_flag = 0;
    for(unsigned long i = 0 ; i < token.size(); i++){
        if(isdigit(token[i]) || token == ".") { numeric_flag = 1;}
    }
    return numeric_flag;
}



