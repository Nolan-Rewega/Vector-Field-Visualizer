#include"Graph.h"
#include"Math.h"
#include"Arrow.h"

/* classic cartesean system, 4 quadrants */
Graph::Graph(int rows, int cols, int aisles, Math* mathObj, bool is2D){
    
    ROWS = rows;
    COLS = cols;
    AISLES = (is2D) ? 2: aisles;

    /* Internal bounds of the grid */
     left_x = -5.0f;      right_x = 5.0f;
     bot_y  = -5.0f;      top_y   = 5.0f;
     back_z = -5.0f;      front_z = 5.0f;

    /* discretization values */
    discretization_x = (right_x - left_x) / COLS;    
    discretization_y = (top_y - bot_y) / ROWS;
    discretization_z = (front_z - back_z) / AISLES;
    
    solver = mathObj;


    // -- 24 vertices per Cube Border, 6 components per vertex.
    numberOfVertices = 24;
    vertexDataByteSize = numberOfVertices * 6 * sizeof(GLfloat);
    vertexData = (GLfloat*)calloc(numberOfVertices * 6, sizeof(GLfloat));

    translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
    rotationMatrix = glm::mat4(1.0f);


    // --  Initilization of the Grid.
    fillBorderData();
    fillTextLabels();
    initFieldData();
    updateGraph();
}
 
Graph::~Graph(){
	freeVertexData();
}
 


vector<Arrow*> Graph::getArrows() { return arrow_data;}
vector<TextLabel*> Graph::getText(){ return text; }



void Graph::translateReferenceFrame(glm::vec3 delta){
    left_x += delta.x;      right_x += delta.x;
    bot_y += delta.y;      top_y += delta.y;
    back_z += delta.z;      front_z += delta.z;
    updateGraph();
    fillTextLabels();
    calculateField();
}

void Graph::updateGraph() {

    int c = 0.0f;
    int r = 0.0f;
    int a = 0.0f;
    for (int i = 0; i < field_data_size; i += 3) {
        c = (i / 3) % (COLS-1);
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

        glm::vec3 wrldpos = glm::vec3(-0.8f + (col * 1.6f / COLS), 0.8f - (row * 1.6f / ROWS), 0.8f - (ais * 1.6f / AISLES));

        results = solver->parseToPostFix(field_data[i + 0], field_data[i + 1], field_data[i + 2]);

        Arrow* a = new Arrow(glm::vec3(results[0], results[1], results[2]), 1.4f / COLS, 1.4f / ROWS, 0.1f);
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
    GLfloat size = 0.7f;

    for (int i = 3; i < numberOfVertices * 6.0f; i += 6) {
        vertexData[i + 0] = 1.0f;
        vertexData[i + 1] = 1.0f;
        vertexData[i + 2] = 1.0f;
    }

    // -- This sucks.
    vertexData[0]  = -size; 	vertexData[1]  = -size;    vertexData[2]  =  size;
    vertexData[3]  =  1.0f; 	vertexData[4]  =  0.0f;    vertexData[5]  =  0.0f;
    vertexData[6]  =  size; 	vertexData[7]  = -size;    vertexData[8]  =  size;
    vertexData[9]  =  1.0f; 	vertexData[10] =  0.0f;    vertexData[11] =  0.0f;

    vertexData[12] = -size; 	vertexData[13] = -size;    vertexData[14] = -size;
    vertexData[15] =  0.0f; 	vertexData[16] =  0.0f;    vertexData[17] =  1.0f;
    vertexData[18] = -size; 	vertexData[19] = -size;    vertexData[20] =  size;
    vertexData[21] =  0.0f; 	vertexData[22] =  0.0f;    vertexData[23] =  1.0f;

    vertexData[24] = -size; 	vertexData[25] =  size;    vertexData[26] =  size;
    vertexData[27] =  0.0f; 	vertexData[28] =  1.0f;    vertexData[29] =  0.0f;
    vertexData[30] = -size; 	vertexData[31] = -size;    vertexData[32] =  size;
    vertexData[33] =  0.0f; 	vertexData[34] =  1.0f;    vertexData[35] =  0.0f;

    vertexData[36] = -size; 	vertexData[37] =  size;    vertexData[38] = -size;
    vertexData[42] = -size; 	vertexData[43] =  size;    vertexData[44] =  size;

    vertexData[48] = -size; 	vertexData[49] =  size;    vertexData[50] =  size;
    vertexData[54] =  size; 	vertexData[55] =  size;    vertexData[56] =  size;

    vertexData[60] =  size; 	vertexData[61] =  size;    vertexData[62] =  size;
    vertexData[66] =  size; 	vertexData[67] = -size;    vertexData[68] =  size;

    vertexData[72] =  size; 	vertexData[73] = -size;    vertexData[74] =  size;
    vertexData[78] =  size; 	vertexData[79] = -size;    vertexData[80] = -size;

    vertexData[84] =  size; 	vertexData[85] = -size;    vertexData[86] = -size;
    vertexData[90] =  size; 	vertexData[91] =  size;    vertexData[92] = -size;

    vertexData[96] =  size; 	vertexData[97] =  size;    vertexData[98] =  size;
    vertexData[102] =  size; 	vertexData[103] =  size;    vertexData[104] = -size;

    vertexData[108] =  size; 	vertexData[109] = -size;    vertexData[110] = -size;
    vertexData[114] = -size; 	vertexData[115] = -size;    vertexData[116] = -size;

    vertexData[120] = -size; 	vertexData[121] = -size;    vertexData[122] = -size;
    vertexData[126] = -size; 	vertexData[127] =  size;    vertexData[128] = -size;

    vertexData[132] = -size; 	vertexData[133] =  size;    vertexData[134] = -size;
    vertexData[138] =  size; 	vertexData[139] =  size;    vertexData[140] = -size;

}

void Graph::fillTextLabels(){
    text.clear();

    GLfloat width = 0.1;

    // -- X axis
    text.push_back(new TextLabel(glm::vec3(0.75, -0.7f, 0.7f), glm::vec3(1.0f, 0.0f, 0.0f), 0.05, "X"));
    for (int i = 0; i < COLS + 1; i++) {
        GLfloat x = left_x + (discretization_x * i);

        string xstr = to_string(x);
        string result = xstr.substr(0, xstr.find(".") + 3);

        text.push_back(new TextLabel(glm::vec3(-0.7f + ( i * (1.4f / COLS)), -0.8f, 0.7f), glm::vec3(1.0f, 0.0f, 0.0f), width, result));
    }

    // -- Y axis
    text.push_back(new TextLabel(glm::vec3(-0.7, 0.75f, 0.7f), glm::vec3(0.0f, 1.0f, 0.0f), 0.05, "Y"));
    for (int i = 0; i < ROWS + 1; i++) {
        GLfloat y = bot_y + (discretization_y * i);

        string xstr = to_string(y);
        string result = xstr.substr(0, xstr.find(".") + 3);

        text.push_back(new TextLabel(glm::vec3(-0.8, -0.7f + (i * (1.4f / ROWS)), 0.7f), glm::vec3(0.0f, 1.0f, 0.0f), width, result));
    }

    // -- Z axis
    text.push_back(new TextLabel(glm::vec3(-0.8, -0.7f, -0.75f), glm::vec3(0.0f, 0.0f, 1.0f), 0.05, "Z"));
    for (int i = 0; i < AISLES + 1; i++) {
        GLfloat z = back_z + (discretization_z * i);

        string xstr = to_string(z);
        string result = xstr.substr(0, xstr.find(".") + 3);

        text.push_back(new TextLabel(glm::vec3(-0.8, -0.8f, -0.7f + (i * (1.4f / AISLES))), glm::vec3(0.0f, 0.0f, 1.0f), width, result));
    }
}



