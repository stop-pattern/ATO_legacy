#include "../header/define.h"
#include "../header/Header.h"
#include "ATO.h"

#define ATO_LAG 500	//ÉâÉO[ms]




void c_ATO::Control(State S, int * panel, int * sound) {
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
		if (LimitSpeed - 0.5 < S.V) {	//å∏ë¨êßå‰
			if (control.B < specific.B) {
				if (rand() % 2) {
					control.B += 1;
				}
			}
			break;
		}
		else if (Limit > S.V) {	//â¡ë¨êßå‰
			if (control.P < specific.P) {
				if (rand() % 2) {
					control.P += 1;
				}
			}
		}
		else {	//ëƒçsêßå‰
			if (isCSC) {	//ATOíËë¨
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


void c_ATO::CSC() {
	if (accelaration > 0.5) {
		control.B += 1;
		control.P = 0;
	}
	else if (accelaration < -0.5) {
		control.B = 0;
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


bool c_ATO::Departure() {
	if (key_A1 == true && (key_A2 == true || key_B1 == true)) {
		if (door == false && Stat.V == 0 && manual.B == 0 && manual.P == 0 && manual.R == 1 && LimitSpeed >= 25) {
			return true;
		}
	}
	return false;
}

void c_ATO::SignalChange() {
	Limit = Reference_Speed[Mode][signal];
}


void c_ATO::ChangeMode(int in) {
	if (Stat.V == 0 && manual.B > 0 && manual.P == 0) {
		if (Mode > 0 && Mode < 3) {
			Mode += in;
			SignalChange();
		}
	}
}
