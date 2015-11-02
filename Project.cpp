#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;
typedef string Instruction ;
const int instructionSize = 16 ;
const int numInstructions = 26 ;

//This struct has all of the 26 instructions stored in an array named Instructions.
// Each index of the Instructions array contains a 16 bit instruction, that is stored
// as a string.  
struct InstructionMemory{
       Instruction Instructions[numInstructions] ;
       InstructionMemory() {
       Instructions[0] =  "0111000001000000" ;
       Instructions[1] =  "0111001000010000" ;
       Instructions[2] =  "1000001001001000" ;
       Instructions[3] =  "0001001001010000" ;
       Instructions[4] =  "0111010000001111" ;
       Instructions[5] =  "0111011011110000" ;
       Instructions[6] =  "0111100000000000" ;
       Instructions[7] =  "0111101000010000" ;
       Instructions[8] =  "0111110000000101" ;
       Instructions[9] =  "1100110110000000" ;
       Instructions[10] = "0101000110100000" ;
       Instructions[11] = "0010110110000000" ;
       Instructions[12] = "0110100000000101" ;
       Instructions[13] = "0111111011111111" ;
       Instructions[14] = "0001111111000001" ;
       Instructions[15] = "1101110111010101" ;
       Instructions[16] = "1001000000000011" ;
       Instructions[17] = "1111001001000001" ;
       Instructions[18] = "1011000011111111" ;
       Instructions[19] = "1000000000001000" ;
       Instructions[20] = "0100000110010000" ;
       Instructions[21] = "1001010010000010" ;
       Instructions[22] = "1111100100011011" ;
       Instructions[23] = "1011000011111111" ;
       Instructions[24] = "0001101101000010" ;
       Instructions[25] = "0100000010010000" ;
       }
       } InstructionMemory1;

// ProgramCounters is a struct that contains the index for the next instruction.
// The default index is 0, though it can be created with any index.
struct ProgramCounters{
       int nextIndex;
       ProgramCounters() {
       nextIndex = 0;
       }
       ProgramCounters(int nextIndexGiven): nextIndex(nextIndexGiven){}
       } ProgramCounter;

//IM is the instruction memory, where the instructions are stored.
// PC is the program counter, where the next instruction is stored.
const static InstructionMemory IM ;
static ProgramCounters PC ;

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
                         }
                    if(opCode == "0010") {
                         cout << "Decrement" << endl ;
                         }
                    if(opCode == "0011") {
                         cout << "Increment" << endl ;
                         }
                    if(opCode == "0100") {
                         cout << "Jump" << endl ;
                         }
                    if(opCode == "0101") {
                         cout << "Jump If Zero" << endl ;
                         }
                    if(opCode == "0110") {
                         cout << "Load Word" << endl ;
                         }
                    if(opCode == "0111") {
                         cout << "Load Word Immediate" << endl ;
                         }
                    if(opCode == "1000") {
                         cout << "Shift Left Immediate" << endl ;
                         }
                    if(opCode == "1001") {
                         cout << "Shift Right Immediate" << endl ;
                         }
                    if(opCode == "1010") {
                         cout << "Store Word" << endl ;
                         }
                    if(opCode == "1011") {
                         cout << "Store Word Immediate" << endl ;
                         }
                    if(opCode == "1100") {
                         cout << "Subtraction Immediate" << endl ;
                         }
                    if(opCode == "1101") {
                         cout << "Jump Less Than Or Equal" << endl;
                         }
                    if(opCode == "1111"){
                                      if(func == "000") {
                                           cout << "Add" << endl;
                                           }
                                      if(func == "001") { 
                                           cout << "Or" << endl;
                                           }
                                      if(func == "010") {
                                           cout << "Subtract" << endl;
                                           }
                                      if(func == "011") {
                                           cout << "XOR" << endl; 
                                           }
                    }
                    
}
            
int main(int argc, char **argv) {
    for(int i = 0; i < numInstructions; i++){
            string currInstruction;
            currInstruction = FetchInstruction(PC.nextIndex) ;
            decodeInstruction(currInstruction) ;
    }
    string input ;
    cin >> input ;
    return 0;
}
