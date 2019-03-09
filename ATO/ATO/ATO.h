#ifndef _ATO_
#define _ATO_

#include "../header/ats.h"


namespace ATO {
	void Control(State, int *, int *);	//毎フレーム制御
	void CSC();	//ATO定速
	bool Departure(void);	//出発ボタン判定
	void SignalChange();	//現示変更
	void ChangeMode(int);	//運転モード変更

	extern int Limit;	//目標速度
	extern int Mode;	//ATO運転モード
	extern Hand control;	//ハンドル制御値

	extern bool isCSC;	//ATO定速
	extern int button_buf;	//ボタン押下時ラグ判定用

	extern const int Reference_Speed[4][256];	//ATO目標速度
	enum ATOmode {
		ATC追従,
		ATO平常,
		ATO回復,
		ATO遅速,
	};
};

#endif // _ATO_