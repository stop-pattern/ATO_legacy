#include "../header/define.h"
#include "../header/Header.h"
#include "ATO.h"

#define ATO_LAG 500	//ÉâÉO[ms]


void c_ATO::Control(State S, int * panel, int * sound) {

	switch (ATOstatus) {
	case ATC_status::ATO_stopping:
	case ATC_status::ATO_ON:
	case ATC_status::ATO_waiting:
		control.P = 0;
		ATOstatus == ATC_status::ATO_stopping ? control.B = 4 : control.B = 0;
		if (Departure()) {
			if (button_buf > 0) {

			}
			ATOstatus = ATC_status::ATO_driving;
			break;
		}
		panel[Brake_notches] = manual.B;
		panel[TASC_power] = true;
		panel[TASC_release] = false;
		panel[TASC_braking] = false;
		panel[TASC_controling] = false;
		panel[TASC_noches] = 0;
		panel[TASC_failed] = false;
		panel[TASC_power_M] = false;
		panel[TASC_controling_M] = false;
		panel[ATO_P] = 0;
		panel[ATO_B] = 0;
		panel[ATO_power] = true;
		panel[ATO_controling] = false;
		panel[TASC_debug] = 0;
		panel[ATO_debug] = 0;
		panel[Reservation] = false;
		break;
	case ATC_status::ATO_driving:
		if (LimitSpeed + 1 < S.V) {	//å∏ë¨êßå‰
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
			//isCSC = true;
			break;
		}
		break;

	case ATC_status::TASC_ON:
	case ATC_status::TASC_control:
	case ATC_status::TASC_brake:
	case ATC_status::TASC_waiting:
		panel[Brake_notches] = manual.B;
		panel[TASC_power] = true;
		panel[TASC_release] = false;
		panel[TASC_braking] = false;
		panel[TASC_controling] = false;
		panel[TASC_noches] = 0;
		panel[TASC_failed] = false;
		panel[TASC_power_M] = false;
		panel[TASC_controling_M] = false;
		panel[ATO_P] = 0;
		panel[ATO_B] = 0;
		panel[ATO_power] = false;
		panel[ATO_controling] = false;
		panel[TASC_debug] = 0;
		panel[ATO_debug] = 0;
		panel[Reservation] = false;
		break;
	case ATC_status::OFF:
	case ATC_status::ON:
	default:
		panel[Brake_notches] = 0;
		for (int i = 136; i < 150; i++) {
			panel[i] = false;
		}
		break;
	}

	/*
	if (abs(accelaration) > 10) {
		handle.B = specific.E;	//
	}
	*/
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
			control.P = 0;
			control.B = 0;
			return true;
		}
	}
	return false;
}

void c_ATO::SignalChange() {
	Limit = Reference_Speed[Mode][signal];
}


void c_ATO::ChangeMode(int in) {
	if (key_S == true) {
		if (Stat.V == 0 && manual.B > 0 && manual.P == 0) {
			if (Mode > 0 && Mode < 3) {
				Mode += in;
				SignalChange();
			}
			else if (Mode >= 3) {
				Mode = 3;
			}
			else
			{
				Mode = 1;
			}
		}
	}
}

void c_ATO::setPattern(Beacon b) {
	brake = b;
}

void c_ATO::inEmergency(void){
	ATCstatus = ATC_status::ATO_ON;
	this->control.P = 0;
	this->control.B = 0;
	this->isCSC = false;
}
