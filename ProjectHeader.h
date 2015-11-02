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
#endif
