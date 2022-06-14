#include"Math.h"

Math::Math(){
    /* legal variables */
    variables = {
        {"x", 3}, {"y", 2}, {"z", 1}
    };
    
    /* dictionary of common math functions. */
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

    /* dictionary of valid operators */
    operators = {
        {"+", plus},    {"-", minus},   {"*", mult},
        {"/", div},     {"^", power}
    };
}


int Math::getDimensionality() {
    string inputD;

    cout << "Enter the dimensionality of the vector field (2 or 3):  ";
    getline(cin, inputD);
    try { dimensions = stoi(inputD); }
    catch (...) {
        cout << "Error: input is not an integer.";
        exit(-42);
    }

    if (dimensions != 2 && dimensions != 3) {
        cout << "input is neither 2 nor 3.";
        exit(-84);
    }
    cout << endl;
    return dimensions;
}

void Math::getInput() {
    // -- get user input, this should be done seperatly
    string inputEQ;

    equations.clear();

    if (dimensions == 3) {
        cout << "Enter " << dimensions << " vector equations which represent the field: F (x, y, z)" << endl;
        cout << "A example set of equation could be:" << endl;
        cout << "\t sin x " << endl;
        cout << "\t sin y " << endl;
        cout << "\t sin z " << endl;
    }
    else {
        cout << "Enter " << dimensions << " vector equations which represent the field: F (x, y)" << endl;
        cout << "A example set of equation could be:" << endl;
        cout << "\t ( x ^ 2 - y ^ 2 ) - 4 " << endl;
        cout << "\t 2 * x * y " << endl;
    }
    for(int i = 0; i < dimensions; i++){
        cout << "Enter the " << (char)(120 + i) << " Vector component. \n";
        getline(cin, inputEQ);
        if(inputEQ.size() > 0){
            /* run dijkstras shunnting yard algorithm on input. */
            equations.push_back( shunting(inputEQ) );
        }
    }
    cout << endl;

    resultArray = (double*)calloc(dimensions, sizeof(double));
}
   


   
double* Math::parseToPostFix(double x, double y, double z){
    setVarValues(x, y, z);
    for(unsigned long e = 0; e < equations.size(); e++){
        resultArray[e] = evalPostfix( swapVariablesWithValues(equations[e]) );
    }
    return resultArray;
}



string Math::shunting(string eq){
    
    queue<string> output;
    stack<string> op_stack;
    string delim = " "; string token;
    size_t idx = 0;
    
    while( (idx = eq.find(delim)) != std::string::npos){
        // -- Get each individual syntactic element and remove it from the string.
        token = eq.substr(0, idx);
        eq.erase(0, idx + delim.length());
        
        // -- If the current syntactic element (token) is a number or a variable
        // -- then push it onto the output stack.
        if (isNumber(token) || variables.find(token) != variables.end()) {
            output.push(token);
        }

        // -- If the current syntactic element is a function push it on the
        // -- operator stack.
        else if (functions.find(token) != functions.end()){
            op_stack.push(token);
        }

        // -- If the current syntactic element is a (binary) operator
        // -- then push it on the operator stack.
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

        else if (token == "(") {
            op_stack.push(token);
        }

        else if (token == ")"){
            while(op_stack.top() != "(" && !op_stack.empty()){
                output.push(op_stack.top());
                op_stack.pop();
            }

            if (op_stack.top() == "(") {
                op_stack.pop();
            }

            if (!op_stack.empty()){
                if (functions.find(op_stack.top()) != functions.end()){
                    output.push(op_stack.top());
                    op_stack.pop();
                }
            }
        }
    }
    
    // -- Push remaining operators / functions onto the output stack
    while (! op_stack.empty()){
        string top = op_stack.top();
        op_stack.pop();
        if (top == ")" || top == "(") { cout << "MISMATCH PARENS! " << endl; }
        else { output.push(top); }
    }
    
    // -- Create the postfix String.
    string postfix;
    while(!output.empty()) { 
        postfix = postfix + output.front() + " ";
        output.pop();
    }
    return postfix;
}
        

double Math::evalPostfix(string eq){
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
    return copy;
}


// -- Psudo function environment of all implemented math operations and functions.
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



