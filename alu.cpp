
#include <iostream>
#include <string>
#include <string.h>
using namespace std;




int aluOutput(int src1, int src2, int control, int func, int status)
{
	if (func == 000)
		return src1, src2; // do nothing

	else if (func == 001)
		return src1 + src2;// return sum

	else if (func == 010) //2
		return src1 - src2;// return differece

	else if (func == 011)
		return src1 >> src2; // return source 1 shift right by src2 bits

	else if (func == 100)
		return src1 << src2; // return S1 shift left by src2 bits

	else if (func == 101)
		return src1 | src2; // return bitwise OR

	else if (func == 110)
		return src2 ^ src1; // return bitwise XOR


}
