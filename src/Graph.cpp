#include"Graph.h"
#include"Math.h"
#include"Arrow.h"

/* classic cartesean system, 4 quadrants */
Graph::Graph(int rows, int cols, int aisles, Math* mathObj){
    
    ROWS = rows;
    COLS = cols;
    AISLES = aisles;

    /* Internal bounds of the grid */
     left_x = -3.0f;      right_x = 3.0f;
     bot_y  = -3.0f;      top_y   = 3.0f;
     back_z = -3.0f;      front_z = 3.0f;

    /* discretization values */
    discretization_x = (right_x - left_x) / COLS;    
    discretization_y = (top_y - bot_y) / ROWS;
    discretization_z = (front_z - back_z) / AISLES;
    
    solver = mathObj;


    // -- 8 vertices per Cube Border, 6 components per vertex.
    numberOfVertices = 8;
    vertexDataByteSize = numberOfVertices * 6 * sizeof(GLfloat);
    vertexData = (GLfloat*)calloc(numberOfVertices * 6, sizeof(GLfloat));
    

    // -- 12 Lines per border, 2 Indices per line.
    numberOfDataIndices = 24;
    dataIndicesByteSize = numberOfDataIndices * sizeof(GLushort);
    dataIndices = (GLushort*)calloc(dataIndicesByteSize, sizeof(GLushort));
    GLushort temp[24] =
    {
        0,1,    2,3,    4,5,    6,7,
        0,2,    1,3,    5,7,    4,6,
        0,4,    2,6,    1,5,    3,7
    };
    for (int i = 0; i < numberOfDataIndices; i++) { dataIndices[i] = temp[i]; }



    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
    rotationMatrix = glm::mat4(1.0f);


    // --  Initilization of the Grid.
    fillBorderData();
    initFieldData();
    updateGraph();
}
 
Graph::~Graph(){
	freeVertexData();
}
 



vector<Arrow*> Graph::getArrows() { return arrow_data;}



void Graph::updateGraph() {

    int c = 0.0f;
    int r = 0.0f;
    int a = 0.0f;
    for (int i = 0; i < field_data_size; i += 3) {
        c = (i / 3) % (COLS-1);
        //cout << c << "  " << r << "   " << a << "      " << i << endl;
        field_data[i + 0] = (left_x + ((c + 1) * discretization_x));
        field_data[i + 1] = (top_y - ((r + 1) * discretization_y));
        field_data[i + 2] = (front_z - ((a + 1) * discretization_z));

        if (c == (COLS - 2)) {
            r++;
            if (r == (ROWS - 1)) {
                r = 0;
                a++;
            }
        }
    }
}


void Graph::calculateField(){
    arrow_data.clear();
    double* results = nullptr;

    for (int i = 0; i < field_data_size; i += 3) {
        GLfloat col = (field_data[i + 0] - left_x) / discretization_x;
        GLfloat row = (field_data[i + 1] - top_y)  / ( -discretization_y);
        GLfloat ais = (field_data[i + 2] - front_z) /( -discretization_z);

        //cout << col << "   " <<  row << "   " << ais << endl;
        glm::vec3 wrldpos = glm::vec3(-0.8f + (col * 1.6f / COLS), 0.8f - (row * 1.6f / ROWS), 0.8f - (ais * 1.6f / AISLES));
        //cout << wrldpos.x << "   " << wrldpos.y << "   " << wrldpos.z << endl;

        results = solver->parseToPostFix(field_data[i + 0], field_data[i + 1], field_data[i + 2]);
        //cout << results[0] << "   " << results[1] << "   " << results[2] << endl;

        Arrow* a = new Arrow(glm::vec3(results[0], results[1], results[2]), 1.6f / COLS, 1.6f / ROWS, 0.1f);
        a->translateShape(wrldpos);
        arrow_data.push_back(a);
    }
}






void Graph::initFieldData(){
    field_data_size = (COLS - 1) * (ROWS - 1) * (AISLES - 1) * 3;
    field_data_size_bytes = field_data_size * sizeof(GLfloat);
    field_data = (GLfloat*)calloc(field_data_size, sizeof(GLfloat));
}


void Graph::fillBorderData(){
    GLfloat size = 0.8f;

    for (int i = 3; i < 8.0f * 6.0f; i += 6) {
        vertexData[i + 0] = 1.0f;
        vertexData[i + 1] = 1.0f;
        vertexData[i + 2] = 1.0f;
    }

    vertexData[0]  = -size; 	vertexData[1]  = -size;    vertexData[2]  =  size;
    vertexData[3]  =  0.0f; 	vertexData[4]  =  1.0f;    vertexData[5]  =  0.0f;
    vertexData[6]  =  size; 	vertexData[7]  = -size;    vertexData[8]  =  size;
    vertexData[9]  =  0.0f; 	vertexData[10] =  1.0f;    vertexData[11] =  0.0f;

    vertexData[12] = -size; 	vertexData[13] = -size;    vertexData[14] = -size;
    vertexData[15] =  1.0f; 	vertexData[15] =  0.0f;    vertexData[16] =  0.0f;
    vertexData[18] =  size; 	vertexData[19] = -size;    vertexData[20] = -size;


    vertexData[24] = -size; 	vertexData[25] =  size;    vertexData[26] =  size;
    vertexData[27] =  1.0f; 	vertexData[28] =  0.0f;    vertexData[29] =  0.0f;
    vertexData[30] =  size; 	vertexData[31] =  size;    vertexData[32] =  size;

    vertexData[36] = -size; 	vertexData[37] =  size;    vertexData[38] = -size;
    vertexData[42] =  size; 	vertexData[43] =  size;    vertexData[44] = -size;
}



