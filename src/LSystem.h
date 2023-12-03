#pragma once
#include <vector>
#include <string>
using namespace std;
//  F draw segement
//  X means nothing is used only if next iteration happens
//  & means roll left by angle around H
//  $ means roll right by angle around H
//  + means turn left by angle around U
//  - means turn right by angle around U
//  [ push the current state of the turtle onto the stack
//  ] pop the current state of the turtle from the stack
//  Some information obtained from http://algorithmicbotany.org/papers/sigcourse.2003/2-1-lsystems.pdf

string iterateL(string sentence)  { //this iterates the L-system 1 time
	string newSentence;
    srand( time(0) );
	for (unsigned int i = 0; i < sentence.length(); i++) {
		char c = sentence.at(i);
        
        if (c == 'X') {
            int rNum = rand() % 8;
            switch (rNum) {
            case 0:
                newSentence += "F&+[[X]+X]-F[+FX]+X";
                break;
            case 1:
                newSentence += "F$+[[X]+X]-F[-FX]+X";
                break;
            case 2:
                newSentence += "F&-[[X]+X]-F[+FX]-X";
                break;
            case 3:
                newSentence += "F$-[[X]+X]-F[-FX]-X";
                break;
            case 4:
                newSentence += "F&+[[X]-X]+F[+FX]+X";
                break;
            case 5:
                newSentence += "F$+[[X]-X]+F[-FX]+X";
                break;
            case 6:
                newSentence += "F&-[[X]-X]+F[+FX]-X";
                break;
            case 7:
                newSentence += "F$-[[X]-X]+F[-FX]-X";
                break;
            }
        }
		else if (c == 'F'){
			newSentence += "FF";
        }
		else {
            newSentence += c;
        }
    }
	return newSentence;
}

string iterateNTimes(string axiom, int N){ //used to create a longer l-system
    for (int n = 0; n < N; n++) {
		axiom = iterateL(axiom);
	}
    return axiom;
}