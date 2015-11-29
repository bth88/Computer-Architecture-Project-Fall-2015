#include "Project.h"

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>
using namespace std;

//Global Variables:
         
//IM is the instruction memory, where the instructions are stored.
// PC is the program counter, where the next instruction is stored.
const static InstructionMemory IM ;
static ProgramCounters PC ;
static struct memController mc ;

//Initialized Buffers
static struct IFIDBuf IFIDBuffer;
static struct IDEXBuf IDEXBuffer;
static struct EXMEMBuf EXMEMBuffer;
static struct MEMWBBuf MEMWBBuffer ;

//Initialized Muxes
static struct mux PCSrcMux ;
static struct mux RegDstMux ;
static struct mux JumpMux ;
static struct mux ALUSrcMux ;
static struct mux memSrcMux ;
static struct mux Immd ;
static struct mux MemToRegMux ;

//Control Signals:
static int PCSrc = 0 ; // 0 Chooses PC + 4, 1 chooses the calculated address.
static int RegDst = 0; //0 Chooses Rd, 1 Chooses Rt
static int MemRead = 0; // 0 Memory Reading Disabled, 1 Memory Reading Enabled
static int MemtoReg = 0; // 0 Memory to Register Disabled, 1 Memory to Register Enabled
static int ALUOp = 0; // 0 ALU Control chooses from Instructions[5-0], 1 compares to 0
static int MemWrite = 0; // 0 Memory Writing Disabled, 1 Memory Writing is Enabled
static int ALUSrc = 0; // 0 Chooses Read Data 2, 1 Chooses Sign Extended Address
static int RegWrite = 0; // 0 Register Writing Disabled, 1 Register Writing Enabled
static int Jump = 0; //0 Jumping Disabled, 1 Jumping Enabled
static int MemSrc = 0 ; // 0 Chooses either immediate or result of ALU for WB stage, 1 chooses result from Memory
static int ImmSrc = 0 ; // 0 Chooses the ALU result, 1 chooses the immediate value.
//The function for Mux Controller...
int muxControl(struct mux currMux, int select2, int select1){

	if ((select2 == 0) && (select1 == 0))//00 -> src0
		return currMux.src0;
	
	else if ((select2 == 0) && (select1 == 1))//01
		return currMux.src1;
	
	else if ((select2 == 1) && (select1 == 0))//10
		return currMux.src2;
	
	else if ((select2 == 1) && (select1 == 1))//11
		return currMux.src3;

	else
		return 0;// remove laters
}
// Takes Binary String and turnes it into an int
int btoi(string bin){
	int num = 0;
	int len = bin.length();
	for(int i = 0; i < len; i++){
		num += (bin[i]-'0') * pow(2,(len - 1 -i));
	}
	return num;
}
int aluOutput(int src1, int src2, int aluControl, string func)
{


	if (func == "000")
		return src1; // do nothing

	if (func == "001")
		return src1 + src2;// return sum

	if (func == "010") //2
		return src1 - src2;// return differece

	if (func == "011")
		return src1 >> src2; // return source 1 shift right by src2 bits

	if (func == "100")
		return src1 << src2; // return S1 shift left by src2 bits

	if (func == "101")
		return src1 | src2; // return bitwise OR

	if (func == "110")
		return src2 ^ src1; // return bitwise XOR

	else
		return 0;

}
//This is where the control signals are set.
void Control(const char instructionType, int typeTwo, int ALUOpGiven){
     if(instructionType == 'R'){
        cout << "R-type instruction" << endl ;
        MemRead,MemtoReg,MemWrite,ALUSrc,Jump, PCSrc= 0;
        RegWrite,RegDst = 1;
        ALUOp = ALUOpGiven ;
     }
     //If the instruction is 'I' Type and does not access memory or branch.
     else{if((instructionType == 'I') and (typeTwo == 0)){
        cout << "I-type instruction No Mem Access No Jump" << endl ;
        RegDst,MemRead,MemtoReg,MemWrite,ALUSrc,Jump = 0;
        RegWrite = 1;
        ALUOp = ALUOpGiven ;
     }
     //If the instruction is 'I' Type and does not access memory but does branch.
     else{if((instructionType == 'I') and (typeTwo == 1)){
        cout << "I-type instruction No Mem Access Yes Jump" << endl ;
        RegDst,MemRead,MemtoReg,MemWrite,ALUSrc,Jump = 0;
        PCSrc,RegWrite = 1;
        ALUOp = ALUOpGiven ;
     }
     //If the instruction is 'I' Type and accesses Memory to read
     else{if((instructionType == 'I') and (typeTwo == 2)){
        cout << "I-type instruction Mem Access Read" << endl ;
        RegDst,PCSrc,MemWrite,ALUSrc,Jump = 0;
        RegWrite,MemRead,MemtoReg = 1;
        ALUOp = ALUOpGiven ;
     }
     //If the instruction is 'I' Type and accesses Memory to write
     else{if((instructionType == 'I') and (typeTwo == 3)){
        cout << "I-type instruction Mem Access Write" << endl ;
        RegDst,PCSrc,MemRead,MemtoReg,ALUSrc,RegWrite,Jump = 0;
        MemWrite = 1;
        ALUOp = ALUOpGiven ;
     }
     else{if(instructionType == 'J'){
        cout << "J-type instruction" << endl ;
        RegDst,PCSrc,MemRead,MemtoReg,MemWrite,ALUSrc,RegWrite = 0;
        Jump = 1;
        ALUOp = ALUOpGiven ;
     }}}}}}
}

// parseInstruction takes in an instruction and returns a parsedInstruction struct
// that contains all ways that the instruction could be parsed.
parsedInstruction parseInstruction(Instruction currentInstruction) {
     string opCode = "0000" ;
     string rs = "000" ;
     string rt = "000" ;
     string rd = "000" ;
     string func = "000" ;
     string ImmediateOffset = "000000" ;
     string JumpOffset = "000000000000" ;
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
             ImmediateOffset[m] = currentInstruction[m+4+3+3] ;
     }
                      
     for(int n = 0; n < 12; n++){
             JumpOffset[n] = currentInstruction[n+4] ;
     }
     parsedInstruction parsed(opCode, rs, rt, rd, func, ImmediateOffset, JumpOffset) ;
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
}



/*
memControl( rw = 0 for read/1 for write, offset = reg/mem address offset, type = reg/mem, input = int reference variable)
return 1 for good, 0 for false
*/
int memControl(int rw, int type, int offset, int* input, memController* mc){
	cout << *input << endl;
	if(rw == 0){ //read
		if(type == 0){ // Reg
			*input = mc->reg[offset];
			return 1;
		}else if(type == 1){ // Mem
			*input = mc->mem[offset];
			return 1;
		}else{
			return 0;
		}
	}else if(rw == 1){ //write
		if(type == 0){ // Reg
			mc->reg[offset] = *input;
			cout << "Register Updated" << endl;
			return 1;
		}else if(type == 1){ // Mem
			mc->mem[offset] = *input;
			return 1;
		}else{
			return 0;
		}
	}
	return 0;
}	
int* RegisterFileOperations(int R1, int R2, int RWrite, int write){
     int resultR1 = 0;
     int resultR2 = 0;
     if(write == 0) {
     memControl(0, 0, R1, &resultR1, &mc) ;
     memControl(0, 0, R2, &resultR2, &mc) ;
     }
     if((write == 1) && (RegWrite == 1)) {
     int* resultRWrite ;
     memControl(1, 0, RWrite, resultRWrite, &mc) ;
     }
     int * results = new int[2]; 
     results[0] = resultR1 ;
     results[1] = resultR2 ;
     return results ;
     }


//The first stage, IF.  It updates the IF/ID buffer.
void InstructionFetchStage(){
     //First, create the mux that chooses the next address.
     PC.nextIndex = muxControl(PCSrcMux, PCSrc, 0) ;
     Instruction nextInstruction ;
     nextInstruction = IM.Instructions[PC.nextIndex] ;
     PCSrcMux.src0 = PC.nextIndex + 1 ;
     IFIDBuffer.currInstruction = nextInstruction ;
     IFIDBuffer.currAddress = PC.nextIndex ;
     return ;
     }

//The second stage, ID.  It updates the ID/EX buffer.
void InstructionDecodeStage(){
     parsedInstruction currParsedInstruction ;
     currParsedInstruction = decodeInstruction(IFIDBuffer.currInstruction) ;
     
     //Find the values for R1 and R2 from the Register File...
     int * resultsArray ;
     resultsArray = RegisterFileOperations(btoi(currParsedInstruction.rs), btoi(currParsedInstruction.rt), 0, 0) ;
     
     //Select which register to write to...
     int WregResult ;
     RegDstMux.src0 = btoi(currParsedInstruction.rt) ;
     RegDstMux.src1 = btoi(currParsedInstruction.rd) ;
     WregResult = muxControl(RegDstMux, RegDst, 0) ;
     
     //Calculate the jump address...
     int jumpAddressResult = 0 ;
     int offset = 0 ;
     JumpMux.src0 = btoi(currParsedInstruction.ImmediateOffset) ;
     JumpMux.src1 = btoi(currParsedInstruction.JumpOffset) ;
     offset = muxControl(JumpMux, Jump, 0) ;
     jumpAddressResult = offset + PC.nextIndex ;
     
     //Update the IDEX Buffer...
     IDEXBuffer.R1 = resultsArray[0] ;
     IDEXBuffer.R2 = resultsArray[1] ;
     IDEXBuffer.writeReg = WregResult ; 
     IDEXBuffer.imm = btoi(currParsedInstruction.ImmediateOffset) ;
     IDEXBuffer.jumpAddress = jumpAddressResult ;
     cout << "R1: " << IDEXBuffer.R1 << endl ;
     cout << "R2: " << IDEXBuffer.R2 << endl ;
     cout << "Wreg: " << IDEXBuffer.writeReg << endl ;
     cout << "imm: " << IDEXBuffer.imm << endl ;
     cout << "jumpAddress: " << IDEXBuffer.jumpAddress << endl ;
     return ;
     }
     
void ExecutionStage()
{
	ALUSrcMux.src0 = IDEXBuffer.R2;
	ALUSrcMux.src1 = IDEXBuffer.imm;
	int exMuxResult = muxControl(ALUSrcMux,0,1);//change argurments from hardcoded.

	EXMEMBuffer.aluResult = aluOutput(IDEXBuffer.R1, exMuxResult, 0, "001");// change last two arguments from hardcoded
	EXMEMBuffer.R2 = IDEXBuffer.R2;
	EXMEMBuffer.imm = IDEXBuffer.imm;
	EXMEMBuffer.writeReg = IDEXBuffer.writeReg;
}

void MemoryStage()
{
    int check, src, address = EXMEM.aluResult;
	memSrcMux.src0 = EXMEM.R2;
	memSrcMux.src1 = EXMEM.imm;
	if(MemWrite == 1){
        src = muxControl(memSrcMux, MemSrc, 0) ;
		check = memControl(1, 1, address, &src, &mc);
	}
	if(MemRead == 1){
		check = memControl(0,1, address, &src, &mc);
	}
	MEMWB.imm = EXMEM.imm;
	MEMWB.aluResult = EXMEM.aluResult;
	MEMWB.writeReg = EXMEM.writeReg;
	MEMWB.memResult = src;
}

void WriteBackStage()
{
    int check, src, address = MEMWBBuffer.writeReg;
	Immd.src0 = MEMWBBuffer.aluResult;
	Immd.src1 = MEMWBBuffer.imm;
	if(ImmSrc == 0){
		MemToRegMux.src0 = Immd.src0;
	}else{
		MemToRegMux.src0 = Immd.src1;
	}
	MemToRegMux.src1 = MEMWBBuffer.memResult;
	if(MemtoReg == 0){
		src = MemToRegMux.src0;
	}else{
		src = MemToRegMux.src1;
	}
	check = memControl(1, 0, address, &src, &mc);
}


int main(int argc, char **argv) {
    for(int i = 0; i < numInstructions; i++){
            parsedInstruction currParsedInstruction;
            InstructionFetchStage() ;
            InstructionDecodeStage() ;
            
    }
	
	int ip = 15;
	if(memControl(1, 0, 0, &ip, &mc) == 1){
		cout << "True" << endl;
	}else{
		cout << "False" << endl;
	}
	cout << mc.reg[0]<< endl;
    string input ;
    cin >> input ;
    return 0;
}
