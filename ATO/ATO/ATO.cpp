#include "../header/define.h"
#include "../header/Header.h"
#include "ATO.h"

#define ATO_LAG 500	//ラグ[ms]

int ATO::Limit;	//目標速度
int ATO::Mode = 1;	//ATO運転モード
Hand ATO::control;	//ハンドル制御値

bool ATO::isCSC;	//ATO定速
int ATO::button_buf;

const int ATO::Reference_Speed[4][256] = {
	{0,25,45,65,100,0,0,0,0,0,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,120,0,30,0,0,0,45,40,35,30,25,20,15,10,5,0,0,0,25,40,65},
	{0,0,0,0,0,0,0,0,0,0,0,5,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,115,0,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,7,7,12,17,22,27,32,37,42,47,52,57,62,67,72,77,82,87,92,97,102,107,117,0,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,5,5,10,15,20,20,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,0,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

void ATO::Control(State S, int * panel, int * sound) {
	switch (ATCstatus) {
	case ATC_status::ATO_stopping:
		control.B = 4;
		if (manual.B > 4) {
			ATCstatus = ATC_status::ATO_waiting;
		}
	case ATC_status::ATO_ON:
	case ATC_status::ATO_waiting:
		if (Departure() == true) {
			if (button_buf > 0)
			{

			}
			ATCstatus = ATC_status::ATO_driving;
			break;
		}
		control.P = 0;
		control.B = 0;
		panel[67] = manual.B;
		panel[136] = true;
		panel[137] = false;
		panel[138] = false;
		panel[139] = false;
		panel[140] = 0;
		panel[141] = false;
		panel[142] = false;
		panel[143] = false;
		panel[144] = 0;
		panel[145] = 0;
		panel[146] = true;
		panel[147] = false;
		panel[148] = 0;
		panel[149] = 0;
		panel[150] = false;
		break;
	case ATC_status::ATO_driving:
		if (LimitSpeed - 0.5 < S.V) {	//減速制御
			if (control.B < specific.B) {
				if (rand() % 2) {
					control.B += 1;
				}
			}
			break;
		}
		else if (Limit > S.V) {	//加速制御
			if (control.P < specific.P) {
				if (rand() % 2) {
					control.P += 1;
				}
			}
		}
		else {	//惰行制御
			if (isCSC) {	//ATO定速
				CSC();
				break;
			}
			control.P = 0;
			control.B = 0;
			isCSC = true;
			break;
		}
		break;
	default:
		for (int i = 136; i < 150; i++) {
			panel[i] = false;
		}
		break;
	}
}


void ATO::CSC() {
	if (accelaration > 0.5) {
		control.B += 1;
		control.P -= 1;
	}
	else if (accelaration < -0.5) {
		control.B -= 1;
		control.P += 1;
	}
	else {
	}
	if (control.B < 0) {
		control.B = 0;
	}
	if (control.P < 0) {
		control.P = 0;
	}
}


bool ATO::Departure() {
	if (key_A1 == true && (key_A2 == true || key_B1 == true)) {
		if (door == false && Stat.V == 0 && manual.B == 0 && manual.P == 0 && manual.R == 1 && LimitSpeed >= 25) {
			return true;
		}
	}
	return false;
}

void ATO::SignalChange() {
	Limit = Reference_Speed[Mode][signal];
}


void ATO::ChangeMode(int in) {
	if (Stat.V == 0 && manual.B > 0 && manual.P == 0) {
		if (Mode > 0 && Mode < 3) {
			Mode += in;
			SignalChange();
		}
	}
}
