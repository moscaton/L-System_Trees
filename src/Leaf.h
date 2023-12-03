#pragma once
#include <GL/glew.h>    
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#define GLEW_STATIC 1
#include <iostream>
#include <vector>

#include "Branch.h"


using namespace std;
using namespace glm;
class Leaf 
{
public:
    Leaf( int totalSize, vector<float> BV);
    ~Leaf();
    int shaderProgram;
    void drawLeaf(mat4 mvp);
    
    const char* getVertexShader();
    const char* getFragmentShader();
    void compileAndLinkShaders();
    void createRenderingData(int totalSize);
private:
    unsigned int VAO, VBO, EBO;
    mat4 rotMatrix;
    vec3 color;
    vector<float> leafVertices;
};