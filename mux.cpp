#include <iostream>
#include <string>
using namespace std;

struct mux {
	int src0;
	int src1;
	int src2;
	int src3;
	

}  mux;

int muxControl(int select2, int select1){

	if ((select2 == 0) && (select1 == 0))//00 -> src0
		return mux.src0;
	
	else if ((select2 == 0) && (select1 == 1))//01
		return mux.src1;
	
	else if ((select2 == 1) && (select1 == 0))//10
		return mux.src2;
	
	else if ((select2 == 1) && (select1 == 1))//11
		return mux.src3;

	else
		return 0;// remove laters
}