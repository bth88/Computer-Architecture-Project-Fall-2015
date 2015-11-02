#include "ProjectHeader.h"

#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;


//IM is the instruction memory, where the instructions are stored.
// PC is the program counter, where the next instruction is stored.
const static InstructionMemory IM ;
static ProgramCounters PC ;

//This is where the control signals are set.
void Control(const char instructionType){
     if(instructionType == 'R'){
        cout << "R-type instruction" << endl ;
     }
     else{if(instructionType == 'I'){
        cout << "I-type instruction" << endl ;
     }
     else{if(instructionType == 'J'){
        cout << "J-type instruction" << endl ;
     }}}
}

//FetchInstruction returns an Instruction (String) based on the Program Counter.
// It also increments the program counter index by 1. 
Instruction FetchInstruction(){
       Instruction nextInstruction ;
       nextInstruction = IM.Instructions[PC.nextIndex] ;
       PC.nextIndex++ ;
       return nextInstruction ;
       }
       
//decodeInstruction takes in one argument of Instruction (String) type.  It then
// parses it to figure out what type of instruction it is.
void decodeInstruction(Instruction currentInstruction){
     string opCode = "0000" ;
     string func = "000" ;
     for(int k = 0; k < 4; k++){
             opCode[k] = currentInstruction[k] ;
     }
     for(int l = 0; l < 3; l++){
             func[l] = currentInstruction[l + 13] ;
     }
                    if(opCode == "0001") {
                         cout << "Add Immediate" << endl ;
                         Control('I') ;
                         }
                    if(opCode == "0010") {
                         cout << "Decrement" << endl ;
                         Control('I') ;
                         }
                    if(opCode == "0011") {
                         cout << "Increment" << endl ;
                         Control('I') ;
                         }
                    if(opCode == "0100") {
                         cout << "Jump" << endl ;
                         Control('J') ;
                         }
                    if(opCode == "0101") {
                         cout << "Jump If Zero" << endl ;
                         Control('J') ;
                         }
                    if(opCode == "0110") {
                         cout << "Load Word" << endl ;
                         Control('I') ;
                         }
                    if(opCode == "0111") {
                         cout << "Load Word Immediate" << endl ;
                         Control('I') ;
                         }
                    if(opCode == "1000") {
                         cout << "Shift Left Immediate" << endl ;
                         Control('I') ;
                         }
                    if(opCode == "1001") {
                         cout << "Shift Right Immediate" << endl ;
                         Control('I') ;
                         }
                    if(opCode == "1010") {
                         cout << "Store Word" << endl ;
                         Control('I') ;
                         }
                    if(opCode == "1011") {
                         cout << "Store Word Immediate" << endl ;
                         Control('I') ;
                         }
                    if(opCode == "1100") {
                         cout << "Subtraction Immediate" << endl ;
                         Control('I') ;
                         }
                    if(opCode == "1101") {
                         cout << "Jump Less Than Or Equal" << endl;
                         Control('I') ;
                         }
                    if(opCode == "1111"){
                                      if(func == "000") {
                                           cout << "Add" << endl;
                                           Control('R') ;
                                           }
                                      if(func == "001") { 
                                           cout << "Or" << endl;
                                           Control('R') ;
                                           }
                                      if(func == "010") {
                                           cout << "Subtract" << endl;
                                           Control('R') ;
                                           }
                                      if(func == "011") {
                                           cout << "XOR" << endl; 
                                           Control('R') ;
                                           }
                    }
                    
}

int main(int argc, char **argv) {
    for(int i = 0; i < numInstructions; i++){
            string currInstruction;
            currInstruction = FetchInstruction() ;
            decodeInstruction(currInstruction) ;
    }
    string input ;
    cin >> input ;
    return 0;
}
