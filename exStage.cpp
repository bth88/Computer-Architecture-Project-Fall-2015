#include "ProjectHeader.h"

#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
using namespace std;

//------------------------

struct mux {
	int src0;
	int src1;
	int src2;
	int src3;
	

}  mux;

int muxControl(int select2, int select1){

	if ((select2 == 0) && (select1 == 0))//00 -> src0
		return mux.src0;
	
	if ((select2 == 0) && (select1 == 1))//01
		return mux.src1;
	
	if ((select2 == 1) && (select1 == 0))//10
		return mux.src2;

	if ((select2 == 1) && (select1 == 1))//11
		return mux.src3;

	else
		return 0;
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

//----------Remove all above this line in main project--------------
struct bufferIDEX {
	int r1;
	int r2;
	int immd;
	int writeReg;
	

}  IDEX;

struct bufferEXMEM{
	int aluResult;
	int r2;
	int immd;
	int writeReg;
	

}  EXMEM;



void ExecuteInstruction()

{
	mux.src0 = IDEX.r2;
	mux.src1 = IDEX.immd;
	int exMuxResult = muxControl(0,0);//change argurments from hardcoded.

	EXMEM.aluResult = aluOutput(IDEX.r1, exMuxResult, 0, "001");// change last two arguments from hardcoded
	EXMEM.r2 = IDEX.r2;
	EXMEM.immd = IDEX.immd;
	EXMEM.writeReg = IDEX.writeReg;


}

//-----------

int main() {

///----testing
	IDEX.r1 = 1;
	IDEX.r2 = 1;
	IDEX.immd = 12;
	IDEX.writeReg = 1;
	ExecuteInstruction();

	cout << IDEX.r1 << endl;
	cout << IDEX.r2 << endl;
	cout << IDEX.immd << endl;
	cout << IDEX.writeReg << endl;
	cout << EXMEM.r2 << endl;
	cout << EXMEM.aluResult << endl;
	cout << EXMEM.immd << endl;
	cout << EXMEM.writeReg << endl;
	


	return 0;

}