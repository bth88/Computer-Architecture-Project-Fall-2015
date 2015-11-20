#ifndef PROJECT_H
#define PROJECT_H

#include <stdio.h>
#include <iostream>
#include <string.h>

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
       Instructions[10] = "0101000110100000" ;
       Instructions[11] = "0010110110000001" ;
       Instructions[12] = "0110000100000101" ;
       Instructions[13] = "0111111011111111" ;
       Instructions[14] = "0001111111000001" ;
       Instructions[15] = "1101111110010101" ;
       Instructions[16] = "1001000000000011" ;
       Instructions[17] = "1111001001000001" ;
       Instructions[18] = "1011011011111111" ;
       Instructions[19] = "1000000000001000" ;
       Instructions[20] = "0100000110010000" ;
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
       std::string sixBitImmediate ;
       std::string eightBitImmediate ;
       std::string address ;
       //Empty Constructor:
       parsedInstruction(){opCode = "0000"; rs = "000"; rt = "000"; rd = "000"; func = "000"; sixBitImmediate = "000000"; 
       eightBitImmediate = "00000000"; address = "000000000000";}
       //Overloaded Constructor: 
       parsedInstruction(std::string opCodeGiven, std::string rsGiven, std::string rtGiven, std::string rdGiven, std::string funcGiven,
       std::string sixBitImmediateGiven, std::string eightBitImmediateGiven, std::string addressGiven): opCode(opCodeGiven), rs(rsGiven),
       rt(rtGiven), rd(rdGiven), func(funcGiven), sixBitImmediate(sixBitImmediateGiven), eightBitImmediate(eightBitImmediateGiven),
       address(addressGiven) {}
       };

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
#endif
