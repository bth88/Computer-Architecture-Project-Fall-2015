#include "ProjectHeader.h"

#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

//Global Variables:
         
//IM is the instruction memory, where the instructions are stored.
// PC is the program counter, where the next instruction is stored.
const static InstructionMemory IM ;
static ProgramCounters PC ;

//Control Signals:
static int RegDst = 0; //0 Chooses Rd, 1 Chooses Rt
static int Branch = 0; // 0 Branching Disabled, 1 Branching Enabled
static int MemRead = 0; // 0 Memory Reading Disabled, 1 Memory Reading Enabled
static int MemtoReg = 0; // 0 Memory to Register Disabled, 1 Memory to Register Enabled
static int ALUOp = 0; // 0 ALU Control chooses from Instructions[5-0], 1 compares to 0
static int MemWrite = 0; // 0 Memory Writing Disabled, 1 Memory Writing is Enabled
static int ALUSrc = 0; // 0 Chooses Read Data 2, 1 Chooses Sign Extended Address
static int RegWrite = 0; // 0 Register Writing Disabled, 1 Register Writing Enabled
static int Jump = 0; //0 Jumping Disabled, 1 Jumping Enabled

//This is where the control signals are set.
void Control(const char instructionType, int typeTwo, int ALUOpGiven){
     if(instructionType == 'R'){
        cout << "R-type instruction" << endl ;
        Branch,MemRead,MemtoReg,MemWrite,ALUSrc,Jump= 0;
        RegWrite,RegDst = 1;
        ALUOp = ALUOpGiven ;
     }
     //If the instruction is 'I' Type and does not access memory or branch.
     else{if((instructionType == 'I') and (typeTwo == 0)){
        cout << "I-type instruction No Mem Access No Jump" << endl ;
        RegDst,Branch,MemRead,MemtoReg,MemWrite,ALUSrc,Jump = 0;
        RegWrite = 1;
        ALUOp = ALUOpGiven ;
     }
     //If the instruction is 'I' Type and does not access memory but does branch.
     else{if((instructionType == 'I') and (typeTwo == 1)){
        cout << "I-type instruction No Mem Access Yes Jump" << endl ;
        RegDst,MemRead,MemtoReg,MemWrite,ALUSrc,Jump = 0;
        Branch,RegWrite = 1;
        ALUOp = ALUOpGiven ;
     }
     //If the instruction is 'I' Type and accesses Memory to read
     else{if((instructionType == 'I') and (typeTwo == 2)){
        cout << "I-type instruction Mem Access Read" << endl ;
        RegDst,Branch,MemWrite,ALUSrc,Jump = 0;
        RegWrite,MemRead,MemtoReg = 1;
        ALUOp = ALUOpGiven ;
     }
     //If the instruction is 'I' Type and accesses Memory to write
     else{if((instructionType == 'I') and (typeTwo == 3)){
        cout << "I-type instruction Mem Access Write" << endl ;
        RegDst,Branch,MemRead,MemtoReg,ALUSrc,RegWrite,Jump = 0;
        MemWrite = 1;
        ALUOp = ALUOpGiven ;
     }
     else{if(instructionType == 'J'){
        cout << "J-type instruction" << endl ;
        RegDst,Branch,MemRead,MemtoReg,MemWrite,ALUSrc,RegWrite = 0;
        Jump = 1;
        ALUOp = ALUOpGiven ;
     }}}}}}
}

//FetchInstruction returns an Instruction (String) based on the Program Counter.
// It also increments the program counter index by 1. 
Instruction FetchInstruction(){
       Instruction nextInstruction ;
       nextInstruction = IM.Instructions[PC.nextIndex] ;
       PC.nextIndex++ ;
       return nextInstruction ;
       }

// parseInstruction takes in an instruction and returns a parsedInstruction struct
// that contains all ways that the instruction could be parsed.
parsedInstruction parseInstruction(Instruction currentInstruction) {
     string opCode = "0000" ;
     string rs = "000" ;
     string rt = "000" ;
     string rd = "000" ;
     string func = "000" ;
     string sixBitImmediate = "000000" ;
     string eightBitImmediate = "00000000" ;
     string address = "000000000000" ;
     string parsedInstructionArray[8] ;
     for(int k = 0; k < 4; k++){
             opCode[k] = currentInstruction[k] ;
     }
     for(int l = 0; l < 3; l++){
             rs[l] = currentInstruction[l+4] ;
             rt[l] = currentInstruction[l+4+3] ;
             rd[l] = currentInstruction[l+4+3+3] ;
             func[l] = currentInstruction[l+4+3+3+3] ;
     }
     for(int m = 0; m < 6; m++) {
             sixBitImmediate[m] = currentInstruction[m+4+3+3] ;
     }
                      
     for(int n = 0; n < 8; n++){
             eightBitImmediate[n] = currentInstruction[n+4+3+1] ;
     }
     for(int o = 0; o < 12; o++){
             address[o] = currentInstruction[o+4] ;
     }
     parsedInstruction parsed(opCode, rs, rt, rd, func, sixBitImmediate, eightBitImmediate, address) ;
     return parsed ;
             
}
//decodeInstruction takes in one argument of Instruction (String) type.  It then
// parses the instruction and sets the correct control signals.  
parsedInstruction decodeInstruction(Instruction currentInstruction){
     parsedInstruction currParsedInstruction ;
     currParsedInstruction = parseInstruction(currentInstruction) ;
     //ALUOp: 0 = Do Nothing, 1 = Add, 2 = Subtract, 3 = Shift Right, 4 = Shift Left,
     //       5 = OR, 6 = XOR
                    if(currParsedInstruction.opCode == "0001") {
                         cout << "Add Immediate" << endl ;
                         Control('I',0,1) ;
                         }
                    if(currParsedInstruction.opCode == "0010") {
                         cout << "Decrement" << endl ;
                         Control('I',0,2) ;
                         }
                    if(currParsedInstruction.opCode == "0011") {
                         cout << "Increment" << endl ;
                         Control('I',0,1) ;
                         }
                    if(currParsedInstruction.opCode == "0100") {
                         cout << "Jump" << endl ;
                         Control('J',0,0) ;
                         }
                    if(currParsedInstruction.opCode == "0101") {
                         cout << "Jump If Zero" << endl ;
                         Control('J',0,2) ;
                         }
                    if(currParsedInstruction.opCode == "0110") {
                         cout << "Load Word" << endl ;
                         Control('I',2,0) ;
                         }
                    if(currParsedInstruction.opCode == "0111") {
                         cout << "Load Word Immediate" << endl ;
                         Control('I', 0, 1) ;
                         }
                    if(currParsedInstruction.opCode == "1000") {
                         cout << "Shift Left Immediate" << endl ;
                         Control('I', 0, 4) ;
                         }
                    if(currParsedInstruction.opCode == "1001") {
                         cout << "Shift Right Immediate" << endl ;
                         Control('I', 0, 3) ;
                         }
                    if(currParsedInstruction.opCode == "1010") {
                         cout << "Store Word" << endl ;
                         Control('I', 3, 0) ;
                         }
                    if(currParsedInstruction.opCode == "1011") {
                         cout << "Store Word Immediate" << endl ;
                         Control('I', 3, 1) ;
                         }
                    if(currParsedInstruction.opCode == "1100") {
                         cout << "Subtraction Immediate" << endl ;
                         Control('I', 0, 2) ;
                         }
                    if(currParsedInstruction.opCode == "1101") {
                         cout << "Jump Less Than Or Equal" << endl;
                         Control('I', 1, 2) ;
                         }
                    if(currParsedInstruction.opCode == "1111"){
                                      if(currParsedInstruction.func == "000") {
                                           cout << "Add" << endl;
                                           Control('R',0,1) ;
                                           }
                                      if(currParsedInstruction.func == "001") { 
                                           cout << "Or" << endl;
                                           Control('R',0,5) ;
                                           }
                                      if(currParsedInstruction.func == "010") {
                                           cout << "Subtract" << endl;
                                           Control('R',0,2) ;
                                           }
                                      if(currParsedInstruction.func == "011") {
                                           cout << "XOR" << endl; 
                                           Control('R',0,6) ;
                                           }
     return currParsedInstruction ;
}

string Mux(int controlSignal, string *inputArray){
       return inputArray[controlSignal] ;
       }

/*
memControl( rw = 0 for read/1 for write, offset = reg/mem address offset, type = reg/mem, input = int reference variable)
return 1 for good, 0 for false
*/
int memControl(int rw, int type, int offset, int* input, memController mem){
	if(rw == 0){ //read
		if(type == 0){ // Reg
			*input = reg[offset];
			return 1;
		}else if(type == 1){ // Mem
			*input = mem[offset];
			return 1;
		}else{
			return 0;
		}
	}else if(rw == 1){ //write
		if(type == 0){ // Reg
			reg[offset] = *input;
			return 1;
		}else if(type == 1){ // Mem
			mem[offset] = *input;
			return 1;
		}else{
			return 0;
		}
	}
	return 0;


}	

int main(int argc, char **argv) {
    for(int i = 0; i < numInstructions; i++){
            parsedInstruction currParsedInstruction;
            string currInstruction;
            currInstruction = FetchInstruction() ;
            currParsedInstruction = decodeInstruction(currInstruction) ;
    }
    struct memController memory;
	cout << memory.reg[0]<< endl;
    string input ;
    cin >> input ;
    return 0;
}
