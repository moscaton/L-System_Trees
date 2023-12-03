#pragma once
#include <stack>
#include <iostream>
#include <string>
#include <vector>


using namespace std;
using namespace glm;


class TurtleState {
public:
	vec3 location;
    vec3 looking;
	
	TurtleState(vec3 LOC, vec3 LOOK) { 
        this->location = LOC; 
        this->looking = LOOK;
    }
};

class Turtle {
private:
	vec3 looking;
	vec3 location;
	stack<TurtleState> stateStack;
public:
	Turtle(){
        location = {0,0,0};
        looking = {0,1,0};
    };
	void turn(vec3 rotation){
        looking += rotation;
        looking = normalize(looking);
        // cout<< "Turtle turned now looking in direction ("<< looking[0]<< ", "<< looking[1]<<", "<< looking[2]<<")"<<endl;
    };
	void pushState(){
        stateStack.push(TurtleState(this->location,this->looking));
    };
	void popState(){
        if (!stateStack.empty()){
            TurtleState prevState = stateStack.top();
            stateStack.pop();
            this->location = prevState.location;
            this->looking = prevState.looking;
        }
    };
    void moveForward(){
        vec3 toAdd = looking * 0.1f;
        vec3 endPoint (location + toAdd );

        // cout<<"Turtle moving from ("<< location[0] << ", " << location[1]<< ", " << location[2];
        // cout<<") to ("<< endPoint[0]<< ", "<< endPoint[1]<< ", " << endPoint[2];
        // cout<< ") . Looking in direction ("<< looking[0]<< ", "<< looking[1]<<", "<< looking[2]<<")"<<endl;
        
        location = endPoint;
    };
/*
 F draw segement
 X means nothing is used only if next iteration happens
 & means roll left by angle around H
 $ means roll right by angle around H
 + means turn left by angle around U
 - means turn right by angle around U
 [ push the current state of the turtle onto the stack
 ] pop the current state of the turtle from the stack
 Some information obtained from http://algorithmicbotany.org/papers/sigcourse.2003/2-1-lsystems.pdf
*/
    vector<vec3> followLSystem(string lsystem, float angleRad){
        int moveCount = 0;
        vector<vec3> branches;

        for (int i = 0 ; i < lsystem.length(); i++){
            char currentChar = lsystem[i];
            if (currentChar == 'F'){
                vec3 start = this->location;
                this->moveForward();
                vec3 end = this->location;
                branches.push_back(vec3(start[0],start[1],start[2]));
                branches.push_back(vec3(end[0],end[1],end[2]));
                moveCount +=1;
            }
            else if (currentChar == '&'){
                vec3 start = this->location;
                this->turn(vec3 (-1.0f* angleRad,0,0));
                vec3 end = this->location;
            }
            else if (currentChar == '$'){
                vec3 start = this->location;
                this->turn(vec3 (angleRad,0,0));
                vec3 end = this->location;
            }
            else if (currentChar == '+'){
                vec3 start = this->location;
                this->turn(vec3 (0,0,angleRad));
                vec3 end = this->location;
            }
            else if (currentChar == '-'){
                vec3 start = this->location;
                this->turn(vec3 (0,0,-1.0f *angleRad));
                vec3 end = this->location;
            }
            else if (currentChar == '['){
                this->pushState();
            }
            else if (currentChar == ']'){
                this->popState();
            }

        }
        // cout<<"Movecount:"<<moveCount<<endl;
        return branches;
    };



    
};