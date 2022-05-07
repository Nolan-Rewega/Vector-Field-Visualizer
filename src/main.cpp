#include<iostream>
#include"Math.h"
#include"Grid.h"
#include"Display.h"

using namespace std;

int main(){
    /* Intializing objects */
    Grid field;
    Display screen;
    
    while(!screen.check_termination()){
        /* data retreval */
        
        /* OpenGL proccesies*/
        screen.background();
        screen.update_vertices(field.vector_data);
        screen.plot_vectors();
        screen.poll_events();
    }
    screen.exit();
    
    //field.printGrid();
    /* Testing Math parser 
    Math test;
    string a1 = test.shunting("3 + 4 * 2 ");
    string a2 = test.shunting("3 + 4 + 2 * 5 ");
    string a3 = test.shunting("3.14 + 4 * 2 ");
    string a4 = test.shunting("3 + 4 * 2 ^ 2 ");
    string a5 = test.shunting("25 - ( 3 * ( 2 + 3 ) / 5 ) ");
    string a6 = test.shunting(" ( ( 4 + 2 ) * ( 2 / 2 ) ) ");
    string a7 = test.shunting("6 / 3 + ( 5 ^ 2 ) ");  
    string a8 = test.shunting("3 + 4 * 2 / ( 1 - 5 ) ^ 2 ^ 3 ");
    string a9 = test.shunting("sin ( 3 * cos ( 4 + 4 ) ) ");
    string a10 = test.shunting("( 4 + 5 ) ! ");
    string a11 = test.shunting("( ln ( 16 ) + 5 ! ) / ( 3 * 6 ) ");
    string a12 = test.shunting("sin ( 3 / 3 * 3.14 ) ");
    
    // testing evalInfix 
    // 95% condident evalInfix works as intended 
    //test.evalPostfix(a1);
    //test.evalPostfix(a2);
    //test.evalPostfix(a3);
    //test.evalPostfix(a4);
    //test.evalPostfix(a5);
    //test.evalPostfix(a6);
    //test.evalPostfix(a7);
    //test.evalPostfix(a8);
    //test.evalPostfix(a9);
    //test.evalPostfix(a10);
    //test.evalPostfix(a11);
    //test.evalPostfix(a12);

    //test.parseToPostFix(10,1,2);
    */
    return 1;
};
