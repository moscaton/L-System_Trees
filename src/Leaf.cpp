#include "Leaf.h"

Leaf::Leaf(int totalSize, vector<float> BV){
    leafVertices = BV;
    color = {0.07f, 0.2f, 0.0f};
    rotMatrix = mat4(1.0);

    compileAndLinkShaders();
    createRenderingData(totalSize);
};
Leaf::~Leaf() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    leafVertices.clear();
};

// static vector<Vertex> createVertices(){

// }

void Leaf::drawLeaf(mat4 mvp)
{   
    this->rotMatrix = mvp;

    int numCOORD = leafVertices.size();

    int totalSize = numCOORD*3;
    
    float vertexArray[totalSize];
    for (int i = 0; i<totalSize; i++){
        vertexArray[i] = leafVertices[i];
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, totalSize, vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(shaderProgram);
    
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "rotMatrix"), 1, GL_FALSE, &rotMatrix[0][0]);

    glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &color[0]);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (totalSize/3)/2, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

};
const char* Leaf::getVertexShader()
{
    return
    "#version 330 core  \n"
        "layout (location = 0) in vec3 aPos;\n"
        ""
        "uniform mat4 rotMatrix;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = rotMatrix * vec4(aPos.x, aPos.y, aPos.z, 1);\n"
        "\n"
        "}\n";
};
const char* Leaf::getFragmentShader()
{
    return
    "#version 330 core  \n"
        "out vec4 FragColor;"
        "uniform vec3 color;\n"
        ""
        "void main()"
        "{"
        "   FragColor = vec4(color, 1.0f);"
        "}";
};

void Leaf::compileAndLinkShaders()
    {
    
    const char* vertexShaderSource = getVertexShader();
    const char* fragmentShaderSource = getFragmentShader();
    
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
};

void Leaf::createRenderingData(int totalSize)
{
    unsigned int indices [totalSize/2];
    for ( int i = 0; i<(totalSize/2)-1; i+=6){
        indices[i] = i;
        indices[i+1] = i+1;
        indices[i+2] = i+3;
        indices[i+3] = i+1;
        indices[i+4] = i+2;
        indices[i+5] = i+3;  
    };
    
    glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, totalSize * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex,position));
	glEnableVertexAttribArray(0);
    
    glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);
	

    
        
};