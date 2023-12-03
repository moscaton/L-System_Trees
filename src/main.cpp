#define GLEW_STATIC 1  

#include <GL/glew.h>    
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <stack>
#include <iostream>
#include <vector>
#include <string>


#include "LSystem.h"
#include "Branch.h"
#include "Turtle.h"
#include "Leaf.h"


using namespace std;
using namespace glm;

const GLint WIDTH = 900;
const GLint HEIGHT = 700;

void processInput(GLFWwindow *window, vec3& cameraPos, vec3 &cameraFront, vec3 &cameraUp, float& rotDelta)
{
    const float cameraSpeed = 1.5f ; //can adjust

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    //Zoom in/out
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    //Up/Down:
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos[1] += 0.5f;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos[1] -= 0.5f;
	}
    //Left/Right
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    //Rotations
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		rotDelta += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		rotDelta -= 0.1f;
	}

}
mat4 getCameraRotation(vec3 cameraPos, float rotDelta) {
	const float rotationSpeed= 45.0f;   //can adjust
    float angle = rotDelta * rotationSpeed;

	cameraPos = cameraPos[2] * vec3(cos(radians(angle)), 1, sin(radians(angle)));
	mat4 view = lookAt(cameraPos, vec3(0.0f, 2.0f, 0.0f), vec3(0, 1, 0));
	return view;
}


int main()
{
    //Initial declarations:
    float angle = (3.14f / 180) * 25;   //can adjust
    string axiom = "F[X][X][X][X]";

    string lSystem = axiom;
    stack<string> stackLSystems;
    stackLSystems.push(axiom);
    Turtle myTurt;

    //OpenGL startup:
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    //Create Window and rendering context using GLFW
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Nicolas_MB_371_Project", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

    //Set background color
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    
    //Camera variables:
	float lastFrame = 0.0f;
	float rotDelta = 0.0f;
    
    vec3 cameraPos = vec3(0.0f,0.0f,5.0f);
    vec3 cameraTarget = vec3(0.0f,1.0f,0.0f);
    vec3 cameraFront = normalize(cameraTarget - cameraPos);
    vec3 cameraUp = vec3(0.0f, 1.0f,0.0f);

    mat4 view;
    view = lookAt(cameraPos,cameraTarget, cameraUp);
    
    mat4 projection = perspective(radians(45.0f), ((float)bufferWidth/(float)bufferHeight), 2.0f, 100.0f);
    mat4 MVP = projection * view;


    //create branches:
    vector<vec3> branchVertices = myTurt.followLSystem(lSystem, angle);
    int numCOORD = branchVertices.size();
    int totalSize = numCOORD*3;
    vector<float> vA (totalSize);
    int j=0;
    for (int i = 0; i<totalSize-3; i= i + 3){
        vA[i] = branchVertices[j][0];
        vA[i+1] = branchVertices[j][1];
        vA[i+2] = branchVertices[j][2];
        j++;
    };
    
    Branch* branch =  new Branch(totalSize, vA);

    //Ground:
    int numCOORDGround = 4;
    int totalSizeGround = numCOORDGround*3;
    vector<float> groundVA1 (totalSizeGround);
    vector<float> groundVA2 (totalSizeGround);
    // j=0;
    float verticesGround1[9] = {
      2.0f, 0.0f,-2.0f,
      2.0f, 0.0f, 2.0f,
     -2.0f,-0.0f, 2.0f,
    }; 
    float verticesGround2[9] = {
      2.0f, 0.0f,-2.0f,
     -2.0f, 0.0f,-2.0f,
     -2.0f,-0.0f, 2.0f,
    }; 
    for (int i = 0; i<totalSizeGround; i ++){
        groundVA1[i] = verticesGround1[i];
        groundVA2[i] = verticesGround2[i];
    };

    Leaf* ground1 =  new Leaf(totalSizeGround, groundVA1);
    Leaf* ground2 =  new Leaf(totalSizeGround, groundVA2);

    //create nicer line:
    glEnable(GL_LINE_SMOOTH);

    glViewport(0, 0, bufferWidth, bufferHeight);

    int maxIters = 0;

    while(!glfwWindowShouldClose(window))
    {
        //check input:
        glClear(GL_COLOR_BUFFER_BIT);
        
        processInput(window, cameraPos,cameraFront,cameraUp, rotDelta);
        
        //reset view
        view = lookAt(cameraPos,cameraPos + cameraFront, cameraUp );
        MVP = projection * view * getCameraRotation(cameraPos, rotDelta);
        
        //Draw grass tile    
        ground1->drawLeaf(MVP);
        ground2->drawLeaf(MVP);

        //Draw the tree
        branch->drawBranch(MVP); 

        //Swap buffers
        glfwSwapBuffers(window);

        //Detect inputs
        glfwPollEvents();    

        //Proces inputs that affect the system:
        //increment system to grow the plant
        if ((glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) && maxIters<7) {
            Turtle newTurt;
            delete branch;
            lSystem = iterateL(lSystem);
            maxIters+=1;
            if (maxIters<5){
                stackLSystems.push(lSystem);
            }
            vector<vec3> branchVertices = newTurt.followLSystem(lSystem, angle);
            int numCOORD = branchVertices.size();
            int totalSize = numCOORD*3;
            cout<<"Lines to draw: "<<numCOORD<<endl;
            vector<float> vA (totalSize);
            int j=0;
            for (int i = 0; i<totalSize-3; i= i + 3){
                vA[i] = branchVertices[j][0];
                vA[i+1] = branchVertices[j][1];
                vA[i+2] = branchVertices[j][2];
                j++;
            };
            branch =  new Branch(totalSize, vA);

        }
        //decrement the system to shrink plant
        if ((glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) && maxIters>0) {
            Turtle newTurt;
            delete branch;
            lSystem = stackLSystems.top();
            stackLSystems.pop();
            vector<vec3> branchVertices = newTurt.followLSystem(lSystem, angle);
            int numCOORD = branchVertices.size();
            int totalSize = numCOORD*3;
            cout<<"Lines to draw: "<<numCOORD<<endl;
            vector<float> vA (totalSize);
            int j=0;
            for (int i = 0; i<totalSize-3; i= i + 3){
                vA[i] = branchVertices[j][0];
                vA[i+1] = branchVertices[j][1];
                vA[i+2] = branchVertices[j][2];
                j++;
            };
            maxIters -=1;
            branch =  new Branch(totalSize, vA);

        }
        //reset everything
        if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)) {
            Turtle newTurt;
            delete branch;
            lSystem = axiom;
            while (!stackLSystems.empty() ){
                stackLSystems.pop();
            }
            vector<vec3> branchVertices = newTurt.followLSystem(lSystem, angle);
            int numCOORD = branchVertices.size();
            int totalSize = numCOORD*3;
            cout<<totalSize<<endl;
            vector<float> vA (totalSize);
            int j=0;
            for (int i = 0; i<totalSize-3; i= i + 3){
                vA[i] = branchVertices[j][0];
                vA[i+1] = branchVertices[j][1];
                vA[i+2] = branchVertices[j][2];
                j++;
            };
            maxIters =0;
            branch =  new Branch(totalSize, vA);

        }
        //reset camera to a views
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS){
            cameraPos = vec3(0.0f,3.0f,5.0f);
            cameraTarget = vec3(0.0f,1.0f,0.0f);
            cameraFront = normalize(cameraTarget - cameraPos);
            cameraUp = vec3(0.0f, 1.0f,0.0f);
        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
            cameraPos = vec3(0.0f,0.0f,5.0f);
            cameraTarget = vec3(0.0f,1.0f,0.0f);
            cameraFront = normalize(cameraTarget - cameraPos);
            cameraUp = vec3(0.0f, 1.0f,0.0f);
        }

    }
    // delete leaf;
    delete ground1;
    delete ground2;
    delete branch;
    glDisable(GL_LINE_SMOOTH);
    glfwTerminate();
    return 0;
}