#include"Math.h"

Math::Math(){
    /* legal variables*/
    variables = {
        {"x", 3}, {"y", 2}, {"z", 1}, {"w", 0}
    };
    
    /* vector list of common math functions. */
    functions = {
        {"sin", 1},     {"cos", 1},     {"tan", 1}, 
        {"sin^-1", 1},  {"cos^-1", 1},  {"tan^-1", 1},
        {"csc", 1},     {"sec", 1},     {"coh", 1},
        {"csc^-1", 1},  {"sec^-1", 1},  {"coh^-1", 1},
        {"!", 1},       {"ln", 1},      {"log", 1}
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
    cout << "e.g ( 3 * x + y ^ 2 )" << endl;
    for(int i = 0; i < 2; i++){
        cout << "Enter Equations " << i+1 << ". \n";
        getline(cin, input);
        if(input.size() > 0){
            equations.push_back(input);
        }
    }
    swapped.resize(equations.size(), " ");
    result.resize(equations.size(), 0.0);
}
   
   
void Math::parseToPostFix(double x, double y, double z){
    for(unsigned long e = 0; e < equations.size(); e++){
        /*
            run dijkstras shunnting yard algorithm on equation e.
            replace current equation with the postfix version
        */
        setVarValues(x ,y ,z);
        swapped[e] = swapVariablesWithValues(equations[e]);
        swapped[e] = shunting(swapped[e]);
        result[e] = evalPostfix(swapped[e]);
    }
}


void Math::setVarValues(double x, double y, double z){
    variables["x"] = x;
    variables["y"] = y;
    variables["z"] = z;
}


string Math::swapVariablesWithValues(string eq){
    string copy = eq; string delim = " "; string token;
    size_t idx = 0; size_t pos = 0;
    cout << "START: "<< eq << endl;
    
    while( (idx = eq.find(delim)) != std::string::npos){
        // get each individual component
        token = eq.substr(0, idx);
        eq.erase(0, idx + delim.length());
        
        pos = copy.find(token);
        while(pos != std::string::npos && variables.find(token) != variables.end()){
            // get each individual component
            string substitute = to_string(variables[token]);
            copy.replace(pos, token.size(), substitute);
            pos = copy.find(token, pos + substitute.size());
        }
    }
    cout << "SWAPPED: " << copy << endl;
    return copy;
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
    
    // create the infix String
    string infix;
    while(!output.empty()) { 
        infix = infix + output.front() + " ";
        output.pop();
    }
    return infix;
}
        

double Math::evalPostfix(string eq){
    stack<double> num_stack;
    vector<double> args;
    string delim = " ";
    string token; size_t idx;

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
    cout << "RESULT (evalpostfix): " << num_stack.top() << endl;
    return num_stack.top();
};

// used in evaluation of the postfix expression
double Math::compute(vector<double> args, string op){
    double value_one = args[0];
    double value_two = args[1];
    if(op == "+"){return value_two + value_one;}
    else if(op == "-"){return value_two - value_one;}
    else if(op == "*"){return value_two * value_one;}
    else if(op == "/"){return value_two / value_one;}
    else if(op == "^"){return pow(value_two, value_one);}
    else if(op == "!"){
        double result = value_one;
        while(value_one > 1){
            value_one--;
            result = result * value_one;
        }
        return result;
    }
    
    else if(op == "sin"){return sin( value_one);}
    else if(op == "cos"){return cos( value_one);}
    else if(op == "tan"){return tan( value_one);}
    else if(op == "sec"){return 1/(sin( value_one));}
    else if(op == "csc"){return 1/(cos( value_one));}
    else if(op == "coh"){return cos(value_one) / sin(value_one);}   
    else if(op == "sin^-1"){return asin( value_one);}
    else if(op == "cos^-1"){return acos( value_one);}
    else if(op == "tan^-1"){return atan( value_one);}
    else if(op == "log"){return log10(value_one);}
    else if(op == "ln"){return log(value_one);}
    else {
        cout << "COMPUTE ERROR" << endl;
        return -1;
    }
}
    
/* helper function */
bool Math::isNumber(string token){
    int numeric_flag = 0;
    for(unsigned long i = 0 ; i < token.size(); i++){
        if(isdigit(token[i]) || token == ".") { numeric_flag = 1;}
    }
    return numeric_flag;
}



