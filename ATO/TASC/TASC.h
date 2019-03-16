#ifndef _TASC_
#define _TASC_

#include "../header/ats.h"

class c_TASC {
public:
	void Control(State, int *, int *);	//毎フレーム制御
	Hand control;	//ハンドル制御値
	int Limit;	//目標速度
	void setBeacon(int, Beacon);	//beacon set
	void setStatus(bool);	//status set
private:
	Beacon P[5][3];	//TASC地上子
	Beacon passage;	//停通判定
};

#endif // !_TASC_