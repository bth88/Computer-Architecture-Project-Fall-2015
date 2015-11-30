#ifndef PROJECT_H
#define PROJECT_H

#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;
typedef std::string Instruction ;
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
       Instructions[10] = "0101000000011010" ;
       Instructions[11] = "0010110110000001" ;
       Instructions[12] = "0110000100000101" ;
       Instructions[13] = "0111111011111111" ;
       Instructions[14] = "0001111111000001" ;
       Instructions[15] = "1101111110010101" ;
       Instructions[16] = "1001000000000011" ;
       Instructions[17] = "1111001001000001" ;
       Instructions[18] = "1011011011111111" ;
       Instructions[19] = "1000000000001000" ;
       Instructions[20] = "0100000000011001" ;
       Instructions[21] = "1001010010000010" ;
       Instructions[22] = "1111011100100011" ;
       Instructions[23] = "1011011011111111" ;
       Instructions[24] = "0001101101000010" ;
       Instructions[25] = "0100000010010000" ;
       }
       };

// ProgramCounters is a struct that contains the index for the next instruction.
// The default index is 0, though it can be created with any index.
struct ProgramCounters{
       int nextIndex;
       ProgramCounters() {
       nextIndex = 0;
       }
       ProgramCounters(int nextIndexGiven): nextIndex(nextIndexGiven){}
       };

// parsedInstruction is a struct that contains all of the possible parsed pieces of an instruction.
// It is up to the calling function to choose the correct pieces to use for the current instruction.
// For example, a load word immediate instruction would use the eight bit immediate instead of six bit
// immediate, whereas a subtract immediate instruction would use the six bit immediate.  
struct parsedInstruction{
       std::string opCode ;
       std::string rs ;
       std::string rt ;
       std::string rd ;
       std::string func ;
       std::string ImmediateOffset ;
       std::string EightBitImmediate ;
       std::string JumpOffset ;
       //Empty Constructor:
       parsedInstruction(){opCode = "0000"; rs = "000"; rt = "000"; rd = "000"; func = "000"; ImmediateOffset = "000000";
       EightBitImmediate = "00000000"; JumpOffset = "000000000000";}
       //Overloaded Constructor: 
       parsedInstruction(std::string opCodeGiven, std::string rsGiven, std::string rtGiven, std::string rdGiven, std::string funcGiven,
       std::string ImmediateOffsetGiven, std::string EightBitImmediateGiven, std::string JumpOffsetGiven): opCode(opCodeGiven), rs(rsGiven),
       rt(rtGiven), rd(rdGiven), func(funcGiven), ImmediateOffset(ImmediateOffsetGiven), EightBitImmediate(EightBitImmediateGiven), JumpOffset(JumpOffsetGiven) {}
       };

//Struct for memController
struct memController{
		int reg[8];
		int mem[512];
		memController(){
			for( int i = 0; i < 9; i++){
				reg[i] = 0x00;
			}
			for(int i = 0; i < 513; i++){
				mem[i] = 0x00;
			}
		}
};

//Struct for Mux.
struct mux {
	int src0;
	int src1;
	int src2;
	int src3;
	//Empty Constructor
	mux() {src0 = 0; src1 = 0; src2 = 0; src3 = 0;}
    // Overloaded Constructor for int
    mux(int mysrc0, int mysrc1, int mysrc2, int mysrc3) : src0(mysrc0), src1(mysrc1), src2(mysrc2), src3(mysrc3) {}

}  mux;

//Struct for control signals.  This is so they can easily be passed between stages.
struct ControlSignalsArray {
     int PCSrc; // 0 Chooses PC + 4, 1 chooses the calculated address.
     int RegDst; //0 Chooses Rd, 1 Chooses Rt
     int MemRead; // 0 Memory Reading Disabled, 1 Memory Reading Enabled
     int MemtoReg; // 0 Memory to Register Disabled, 1 Memory to Register Enabled
     int ALUOp; // 0 ALU Control chooses from Instructions[5-0], 1 compares to 0
     int MemWrite; // 0 Memory Writing Disabled, 1 Memory Writing is Enabled
     int ALUSrc; // 0 Chooses Read Data 2, 1 Chooses Sign Extended Address
     int RegWrite; // 0 Register Writing Disabled, 1 Register Writing Enabled
     int Jump; //0 Jumping Disabled, 1 Jumping Enabled
     int MemSrc; // 0 Chooses either immediate or result of ALU for WB stage, 1 chooses result from Memory
     int ImmSrc; // 0 Chooses the ALU result, 1 chooses the immediate value.
     int ALUZero; // 0 if the result from the ALU is nonzero, 1 if the result is 0.
     int JumpZero ; //If 0, the instruction is not a jump zero instruction.  If 1 it is.  
     ControlSignalsArray(){
              PCSrc = 0 ;
              RegDst = 0;
              MemRead = 0;
              MemtoReg = 0;
              ALUOp = 0;
              MemWrite = 0; 
              ALUSrc = 0; 
              RegWrite = 0; 
              Jump = 0; 
              MemSrc = 0 ; 
              ImmSrc = 0 ; 
              ALUZero = 0 ;
              JumpZero = 0 ;
     }
     };
//Struct for IFID buffer
struct IFIDBuf {
       Instruction currInstruction ;
       int currAddress ;
       //Empty Constructor
       IFIDBuf() {currInstruction = ""; currAddress = 0;}
       };
       
//Struct for IDEX Buffer
struct IDEXBuf {
       int R1 ;
       int R2 ;
       int writeReg ;
       int imm ; 
       int EightBitImm ;
       int jumpAddress ;
       struct ControlSignalsArray ControlSignals;
       //Empty Constructor
       IDEXBuf() {R1 = 0; R2 = 0; writeReg = 0; imm = 0; EightBitImm = 0; jumpAddress = 0;}
       }; 

//Struct for EXMEM Buffer
struct EXMEMBuf{
	int aluResult;
	int R2;
	int imm;
	int EightBitImm;
	int writeReg;
	struct ControlSignalsArray ControlSignals;
    //Empty Constructor
    EXMEMBuf() {aluResult = 0; R2 = 0; imm = 0; EightBitImm = 0; writeReg = 0;}
};

//Struct for MEMWB Buffer
struct MEMWBBuf{
	int imm;
	int EightBitImm;
	int aluResult;
	int memResult;
	int writeReg;
	struct ControlSignalsArray ControlSignals ;
	MEMWBBuf() {imm = 0; EightBitImm = 0; aluResult = 0; memResult = 0; writeReg = 0;}
};
#endif

