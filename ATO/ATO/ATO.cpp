#include "../header/define.h"
#include "../header/Header.h"
#include "ATO.h"

#define ATO_LAG 500	//ラグ[ms]


void c_ATO::Control(State S, int * panel, int * sound) {
		if (ATCstatus & ATC_status::ATO_stopping) {
			this->control.P = 0;
			this->control.B = 4;
		}

		//発車判定
		if (this->Departure()) {
			ATCstatus |= ATC_status::ATO_control;
		}

		//ATO制御
		if (ATCstatus & ATC_status::ATO_control) {
			//減速制御
			if (LimitSpeed + 1 < S.V) {
				if (this->control.B < specific.B) {
					if (rand() % 2) {
						this->control.B += 1;
					}
				}
			}
			//加速制御
			else if (this->Limit > S.V) {
				if (this->control.P < specific.P) {
					if (rand() % 2) {
						this->control.P += 1;
					}
				}
			}
			//惰行制御
			else {
				if (this->isCSC) {	//ATO定速
					this->CSC();
				}
				this->control.P = 0;
				this->control.B = 0;
				//isCSC = true;
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
