#include "../header/define.h"
#include "../header/Header.h"
#include "ATO.h"

#define ATO_LAG 500	//ƒ‰ƒO[ms]


void c_ATO::Control(State S, int * panel, int * sound) {

	if (ATCstatus & ATC_status::ATO_ON) {
		if (ATCstatus & ATC_status::ATO_stopping) {
			this->control.P = 0;
			this->control.B = 4;
		}

		if (this->Departure()) {
			ATCstatus |= ATC_status::ATO_control;
		}
		else {
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
		}

		if (ATCstatus & ATC_status::ATO_control) {
			if (LimitSpeed + 1 < S.V) {	//Œ¸‘¬§Œä
				if (this->control.B < specific.B) {
					if (rand() % 2) {
						this->control.B += 1;
					}
				}
			}
			else if (this->Limit > S.V) {	//‰Á‘¬§Œä
				if (this->control.P < specific.P) {
					if (rand() % 2) {
						this->control.P += 1;
					}
				}
			}
			else {	//‘Äs§Œä
				if (this->isCSC) {	//ATO’è‘¬
					this->CSC();
				}
				this->control.P = 0;
				this->control.B = 0;
				//isCSC = true;
			}
		}
	}
	else {
		for (int i = 136; i < 150; i++) {
			panel[i] = false;
		}
	}
	
	/*
	if (abs(accelaration) > 10) {
		handle.B = specific.E;	//
	}
	*/
}


void c_ATO::CSC() {
	if (accelaration > 0.5) {
		this->control.B += 1;
		this->control.P = 0;
	}
	else if (accelaration < -0.5) {
		this->control.B = 0;
		this->control.P += 1;
	}
	else {
	}
	if (this->control.B < 0) {
		this->control.B = 0;
	}
	if (this->control.P < 0) {
		this->control.P = 0;
	}
}


bool c_ATO::Departure() {
	if (key_A1 == true && (key_A2 == true || key_B1 == true)) {
		if (door == false && Stat.V == 0 && manual.B == 0 && manual.P == 0 && manual.R == 1 && LimitSpeed >= 25) {
			this->control.P = 0;
			this->control.B = 0;
			return true;
		}
	}
	return false;
}

void c_ATO::SignalChange() {
	this->Limit = this->Reference_Speed[this->Mode][signal];
}


void c_ATO::ChangeMode(int in) {
	if (key_S == true) {
		if (Stat.V == 0 && manual.B > 0 && manual.P == 0) {
			if (this->Mode > 0 && this->Mode < 3) {
				this->Mode += in;
				this->SignalChange();
			}
			else if (this->Mode >= 3) {
				this->Mode = 3;
			}
			else {
				this->Mode = 1;
			}
		}
	}
}

void c_ATO::setPattern(Beacon b) {
	brake = b;
}

void c_ATO::inEmergency(void) {
	ATCstatus &= ~ATC_status::ATO_control;
	ATCstatus &= ~ATC_status::ATO_doing;
	this->control.P = 0;
	this->control.B = 0;
	this->isCSC = false;
}
