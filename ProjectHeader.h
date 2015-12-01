#include "Project.h"

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>
using namespace std;

//Global Variables:

int end; // When this is 1, stop the program.

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
static struct mux lteMux ;
static struct mux ZeroMux ;

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
int aluOutput(int src1, int src2, int aluControl)
{


	if (aluControl == 0)
		return src1; // do nothing

	if (aluControl == 1)
		return src1 + src2;// return sum

	if (aluControl == 2) 
		return src1 - src2;// return differece

	if (aluControl == 3)
		return src1 >> src2; // return source 1 shift right by src2 bits

	if (aluControl == 4)
		return src1 << src2; // return S1 shift left by src2 bits

	if (aluControl == 5)
		return src1 | src2; // return bitwise OR

	if (aluControl == 6)
		return src2 ^ src1; // return bitwise XOR

	else
		return 0;

}
//This is where the control signals are set.
void Control(const char instructionType, int typeTwo, int ALUOpGiven){
     //Control Signals:
     struct ControlSignalsArray ControlSignals ;
     
     //For No Operations...
     if(instructionType == 'N'){
       cout << "Nop" << endl ;
       ControlSignals.PCSrc = 0; 
        ControlSignals.RegDst = 0; 
        ControlSignals.Jump = 0;
        ControlSignals.ALUSrc = 0;
        ControlSignals.MemSrc = 0;
        ControlSignals.ImmSrc = 0;
        ControlSignals.MemtoReg = 0; 
        ControlSignals.RegWrite = 0;
        ControlSignals.MemRead = 0; 
        ControlSignals.MemWrite = 0; 
        }
     //For J type instructions...
     if(instructionType == 'J'){
        cout << "J-type instruction" << endl ;
        ControlSignals.PCSrc = 1; 
        ControlSignals.RegDst = 0; 
        ControlSignals.Jump = 1;
        ControlSignals.ALUSrc = 0;
        ControlSignals.MemSrc = 0;
        ControlSignals.ImmSrc = 0;
        ControlSignals.MemtoReg = 0; 
        ControlSignals.RegWrite = 0;
        ControlSignals.MemRead = 0; 
        ControlSignals.MemWrite = 0; 
        //If it is a Jump Zero instruction...
        if(ALUOpGiven == 2) { ControlSignals.JumpZero = 1 ; ControlSignals.Jump = 0;} 
        ControlSignals.ALUOp = ALUOpGiven ;
     }
     //For R type instructions
     else{if(instructionType == 'R'){
        cout << "R-type instruction" << endl ;
        ControlSignals.PCSrc = 0; 
        ControlSignals.RegDst = 1; 
        ControlSignals.Jump = 0;
        ControlSignals.ALUSrc = 0;
        ControlSignals.MemSrc = 0;
        ControlSignals.ImmSrc = 0;
        ControlSignals.MemtoReg = 0; 
        ControlSignals.RegWrite = 1;
        ControlSignals.MemRead = 0; 
        ControlSignals.MemWrite = 0; 
        ControlSignals.ALUOp = ALUOpGiven ;
     }
     //For I type instructions with No memory access, no Jumps, and the ALU result stored.
     else{if((instructionType == 'I') && (typeTwo == 0)){
        cout << "I-type instruction No Mem Access No Jump Result Stored" << endl ;
        ControlSignals.PCSrc = 0; 
        ControlSignals.RegDst = 0; 
        ControlSignals.Jump = 0;
        ControlSignals.ALUSrc = 1;
        ControlSignals.MemSrc = 0;
        ControlSignals.ImmSrc = 0;
        ControlSignals.MemtoReg = 0; 
        ControlSignals.RegWrite = 1;
        ControlSignals.MemRead = 0; 
        ControlSignals.MemWrite = 0; 
        ControlSignals.ALUOp = ALUOpGiven ;
     }
     //For I type instructions with No memory access, no Jumps, and the Immediate value stored.
     else{if((instructionType == 'I') && (typeTwo == 1)){
        cout << "I-type instruction No Mem Access No Jump Immediate Stored" << endl ;
        ControlSignals.PCSrc = 0; 
        ControlSignals.RegDst = 0; 
        ControlSignals.Jump = 0;
        ControlSignals.ALUSrc = 1;
        ControlSignals.MemSrc = 0;
        ControlSignals.ImmSrc = 1;
        ControlSignals.MemtoReg = 0; 
        ControlSignals.RegWrite = 1;
        ControlSignals.MemRead = 0; 
        ControlSignals.MemWrite = 0; 
        ControlSignals.ALUOp = ALUOpGiven ;
     }
     //For I type instructions with Jump.
     else{if((instructionType == 'I') && (typeTwo == 2)){
        cout << "I-type instruction Jump" << endl ;
        ControlSignals.PCSrc = 1; 
        ControlSignals.RegDst = 0; 
        ControlSignals.Jump = 0;
        ControlSignals.ALUSrc = 0;
        ControlSignals.MemSrc = 0;
        ControlSignals.ImmSrc = 0;
        ControlSignals.MemtoReg = 0; 
        ControlSignals.RegWrite = 0;
        ControlSignals.MemRead = 0; 
        ControlSignals.MemWrite = 0; 
        ControlSignals.ALUOp = ALUOpGiven ;
     }
     //For I type instructions that read from memory
     else{if((instructionType == 'I') && (typeTwo == 3)){
        cout << "I-type instruction Mem Access Read" << endl ;
        ControlSignals.PCSrc = 0; 
        ControlSignals.RegDst = 0; 
        ControlSignals.Jump = 0;
        ControlSignals.ALUSrc = 1;
        ControlSignals.MemSrc = 0;
        ControlSignals.ImmSrc = 0;
        ControlSignals.MemtoReg = 1; 
        ControlSignals.RegWrite = 1;
        ControlSignals.MemRead = 1; 
        ControlSignals.MemWrite = 0; 
        ControlSignals.ALUOp = ALUOpGiven ;
     }
     //For I type instructions that write Register Data
     else{if((instructionType == 'I') && (typeTwo == 4)){
        cout << "I-type instruction Mem Access Write Register Data " << endl ;
        ControlSignals.PCSrc = 0; 
        ControlSignals.RegDst = 0; 
        ControlSignals.Jump = 0;
        ControlSignals.ALUSrc = 1;
        ControlSignals.MemSrc = 0;
        ControlSignals.ImmSrc = 0;
        ControlSignals.MemtoReg = 0; 
        ControlSignals.RegWrite = 0;
        ControlSignals.MemRead = 0; 
        ControlSignals.MemWrite = 1; 
        ControlSignals.ALUOp = ALUOpGiven ;
     }
     //For I type instructions that write Immediate Data
     else{if((instructionType == 'I') && (typeTwo == 5)){
        cout << "I-type instruction Mem Access Write Immediate Data " << endl ;
        ControlSignals.PCSrc = 0; 
        ControlSignals.RegDst = 0; 
        ControlSignals.Jump = 0;
        ControlSignals.ALUSrc = 1;
        ControlSignals.MemSrc = 1;
        ControlSignals.ImmSrc = 0;
        ControlSignals.MemtoReg = 0; 
        ControlSignals.RegWrite = 0;
        ControlSignals.MemRead = 0; 
        ControlSignals.MemWrite = 1; 
        ControlSignals.ALUOp = ALUOpGiven ;
     }
}}}}}}}
     IDEXBuffer.ControlSignals = ControlSignals ; 
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
     string EightBitImmediate = "00000000" ;
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
     
     for(int n = 0; n < 8; n++) {
             EightBitImmediate[n] = currentInstruction[n+4+3+1] ;      
     }
     for(int o = 0; o < 12; o++){
             JumpOffset[o] = currentInstruction[o+4] ;
     }
     parsedInstruction parsed(opCode, rs, rt, rd, func, ImmediateOffset, EightBitImmediate, JumpOffset) ;
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
                         Control('I',3,1) ;
                         }
                    if(currParsedInstruction.opCode == "0111") {
                         cout << "Load Word Immediate" << endl ;
                         Control('I', 1, 0) ;
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
                         Control('I', 4, 0) ;
                         }
                    if(currParsedInstruction.opCode == "1011") {
                         cout << "Store Word Immediate" << endl ;
                         Control('I', 5, 0) ;
                         }
                    if(currParsedInstruction.opCode == "1100") {
                         cout << "Subtraction Immediate" << endl ;
                         Control('I', 0, 2) ;
                         }
                    if(currParsedInstruction.opCode == "1101") {
                         cout << "Jump Less Than Or Equal" << endl;
                         Control('I', 2, 0) ;
                         }
                    if(currParsedInstruction.opCode == "1110") {
                         cout << "No Operation" << endl ;
                         Control('N', 0, 0) ;
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
	if(rw == 0){ //read
		if(type == 0){ // Reg
			*input = mc->reg[offset];
			return 1;
		}else if(type == 1){ // Mem
			*input = mc->mem[offset];
			cout << "mem[offset-1]: " << mc->mem[offset];
			return 1;
		}else{
			return 0;
		}
	}else if(rw == 1){ //write
		if(type == 0){ // Reg
			mc->reg[offset] = *input;
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
int* RegisterFileOperations(int R1, int R2, int RWrite, int WriteData, int write){
     int resultR1 = 0;
     int resultR2 = 0;
     if(write == 0) {
     memControl(0, 0, R1, &resultR1, &mc) ;
     memControl(0, 0, R2, &resultR2, &mc) ;
     }
     if((write == 1) && (MEMWBBuffer.ControlSignals.RegWrite == 1)) {
     cout << "Write Reg: " << RWrite << endl ;
     cout << "WriteData: " << WriteData << endl ;
     memControl(1, 0, RWrite, &WriteData, &mc) ;
     }
     int* results = new int[2]; 
     results[0] = resultR1 ;
     results[1] = resultR2 ;
     return results ;
     }


//The first stage, IF.  It updates the IF/ID buffer.
void InstructionFetchStage(){
     //First, create the mux that chooses the next address.
     PC.nextIndex = muxControl(PCSrcMux, 0, IDEXBuffer.ControlSignals.PCSrc) ;
     Instruction nextInstruction ;

     nextInstruction = IM.Instructions[PC.nextIndex] ;
     if(nextInstruction == "1111111111111111")
     {
         cout << "End of Program!" << endl ;
         end = 1 ;
         nextInstruction = "1110000000000000" ;
         PCSrcMux.src0 = PC.nextIndex ;
     }
     else{
     PCSrcMux.src0 = PC.nextIndex + 1;
     }
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
     resultsArray = RegisterFileOperations(btoi(currParsedInstruction.rs), btoi(currParsedInstruction.rt), 0, 0, 0) ;
     
     //Select which register to write to...
     int WregResult ;
     RegDstMux.src0 = btoi(currParsedInstruction.rt) ;
     RegDstMux.src1 = btoi(currParsedInstruction.rd) ;
     WregResult = muxControl(RegDstMux, IDEXBuffer.ControlSignals.RegDst, 0) ;
     if(IDEXBuffer.ControlSignals.ImmSrc == 1) {WregResult = btoi(currParsedInstruction.rs);} 
     
     //Calculate the jump address...
     int jumpAddressResult = 0 ;
     int offset = 0 ;
     JumpMux.src0 = btoi(currParsedInstruction.ImmediateOffset) ;
     JumpMux.src1 = btoi(currParsedInstruction.JumpOffset) ;
     offset = muxControl(JumpMux, 0, IDEXBuffer.ControlSignals.Jump) ;
     //Less than or = branch.
     if((IDEXBuffer.ControlSignals.Jump == 0) && (IDEXBuffer.ControlSignals.PCSrc == 1)) {
        int value = resultsArray[1] - resultsArray[0] ;
        int lte = 0;
        if(value <= 0) { lte = 1;}
        lteMux.src0 = PC.nextIndex + 1 ;
        lteMux.src1 = offset ;
        offset = muxControl(lteMux, 0, lte);
     }
     //Jump Zero
     if(IDEXBuffer.ControlSignals.JumpZero == 1) {
        ZeroMux.src0 = PC.nextIndex + 1;
        ZeroMux.src1 = btoi(currParsedInstruction.ImmediateOffset) ;
        if(IDEXBuffer.R1 == 0){EXMEMBuffer.ControlSignals.ALUZero = 1 ;}
        offset = muxControl(ZeroMux, 0, EXMEMBuffer.ControlSignals.ALUZero) ;
     }
     jumpAddressResult = offset;
     PCSrcMux.src1 = jumpAddressResult;
     
     //Update the IDEX Buffer...
     IDEXBuffer.R1 = resultsArray[0] ;
     IDEXBuffer.R2 = resultsArray[1] ;
     IDEXBuffer.writeReg = WregResult ; 
     IDEXBuffer.imm = btoi(currParsedInstruction.ImmediateOffset) ;
     IDEXBuffer.EightBitImm = btoi(currParsedInstruction.EightBitImmediate) ;
     IDEXBuffer.jumpAddress = jumpAddressResult ;
     return ;
     }

void ExecutionStage()
{
	ALUSrcMux.src0 = IDEXBuffer.R2;
	ALUSrcMux.src1 = IDEXBuffer.imm;
	int exMuxResult = muxControl(ALUSrcMux,0, IDEXBuffer.ControlSignals.ALUSrc);
	EXMEMBuffer.aluResult = aluOutput(IDEXBuffer.R1, exMuxResult, IDEXBuffer.ControlSignals.ALUOp);
	if(EXMEMBuffer.aluResult == 0) { EXMEMBuffer.ALUZero = 1; }
	else{ EXMEMBuffer.ALUZero = 0;}
	EXMEMBuffer.R2 = IDEXBuffer.R2;
	EXMEMBuffer.imm = IDEXBuffer.imm;
	EXMEMBuffer.EightBitImm = IDEXBuffer.EightBitImm;
	EXMEMBuffer.writeReg = IDEXBuffer.writeReg;
	EXMEMBuffer.ControlSignals = IDEXBuffer.ControlSignals ;
}

void MemoryStage()
{
    int check, src, address = EXMEMBuffer.EightBitImm;
	memSrcMux.src0 = EXMEMBuffer.R2;
	memSrcMux.src1 = EXMEMBuffer.EightBitImm;
	if(EXMEMBuffer.ControlSignals.MemWrite == 1){
        src = muxControl(memSrcMux, 0, EXMEMBuffer.ControlSignals.MemSrc) ;
        int index = address / 16 ;
		check = memControl(1, 1, index, &src, &mc);
	}
	if(EXMEMBuffer.ControlSignals.MemRead == 1){
        //int readIndex = address / 16 ;
		check = memControl(0,1, address, &src, &mc);
	}
	MEMWBBuffer.imm = EXMEMBuffer.imm;
	MEMWBBuffer.EightBitImm = EXMEMBuffer.EightBitImm;
	MEMWBBuffer.aluResult = EXMEMBuffer.aluResult;
	MEMWBBuffer.writeReg = EXMEMBuffer.writeReg;
	MEMWBBuffer.memResult = src;
	MEMWBBuffer.ControlSignals = EXMEMBuffer.ControlSignals ;
}

void WriteBackStage()
{
    int check, src, address = MEMWBBuffer.writeReg;
	Immd.src0 = MEMWBBuffer.aluResult;
	Immd.src1 = MEMWBBuffer.EightBitImm;
	if(MEMWBBuffer.ControlSignals.ImmSrc == 0){
		MemToRegMux.src0 = Immd.src0;
	}else{
		MemToRegMux.src0 = Immd.src1;
	}
	MemToRegMux.src1 = MEMWBBuffer.memResult;
	if(MEMWBBuffer.ControlSignals.MemtoReg == 0){
		src = MemToRegMux.src0;
	}else{
		src = MemToRegMux.src1;
	}

	RegisterFileOperations(0,0,MEMWBBuffer.writeReg, src, 1) ; 
}
//Initialize Memory
void initMem() {
     mc.mem[1] = 257 ;
     mc.mem[2] = 272 ;
     mc.mem[3] = 17 ;
     mc.mem[4] = 240 ;
     mc.mem[5] = 255 ;
}

int main(int argc, char **argv) {
    initMem() ;
    end = 0;
    int testcounter = 0;
    while(end == 0){
            if(testcounter >= 1000) { break;}
            cout << "INSTRUCTION #: " << testcounter << endl; 
            InstructionFetchStage() ;
            InstructionDecodeStage() ;
            ExecutionStage() ;
            MemoryStage() ;
            WriteBackStage() ;
            testcounter++ ;
    
	for(int i = 0; i < 8; i++) {
            cout << mc.reg[i] << endl ;
            }
    for(int j = 0; j < 17; j++) {
            cout << "MemIndex: " << j << " Mem Value: " << mc.mem[j] << endl;
            }
}
    string input ;
    cin >> input ;
    return 0;
}
