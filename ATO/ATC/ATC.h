#ifndef _ATC_
#define _ATC_

#include "../header/ats.h"
#include "../header/define.h"
#include "../header/Header.h"

#define BRAKE_HALF 2000

class c_ATC{
public:
	void Control(State, int *, int *);	//毎フレーム制御
	void setSignal(int);
	Hand control;
private:
	int limit;
	long int brake_cnt;	//ハーフブレーキ用カウンタ
};


#endif // !_ATC_
