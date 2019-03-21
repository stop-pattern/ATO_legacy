#include "../header/define.h"
#include "../header/Header.h"
#include "TASC.h"


void c_TASC::Control(State S, int * panel, int * sound) {
	this->control.P = 0;
	panel[ATC_Panel::TASC_braking] = false;
	if (ATCstatus & ATC_Status::TASC_ON) {
		//PÝ’è
		this->Distance = this->Location - S.Z;
		this->Limit = sqrt(this->Distance * DECELERATION_BRAKE);
		if (this->Limit >= MAX_SPEED || isnan(this->Limit)) {
			this->Limit = MAX_SPEED;
		}

		//B”»’è
		if (ATCstatus & ATC_Status::TASC_control) {
			if (this->Limit * 1.1 < S.V) {
				if (this->control.B >= 0 && this->control.B <= specific.B) {
					ATCstatus |= ATC_Status::TASC_doing;
					this->control.B++;
				}
			}
			else if (this->Limit * 0.9 > S.V) {
				if (this->control.B >= 0 && this->control.B <= specific.B) {
					ATCstatus |= ATC_Status::TASC_doing;
					this->control.B--;
				}
			}
			if (abs(S.V) < 0.75) {
				ATCstatus |= ATC_Status::TASC_stopping;
				ATCstatus &= ~ATC_Status::TASC_control;
				ATCstatus &= ~ATC_Status::TASC_doing;
			}
		}


		//“]“®–hŽ~B
		if (ATCstatus & ATC_Status::TASC_stopping) {
			this->control.B = 4;
			if (manual.B > this->control.B) {
				this->control.B = 0;
				ATCstatus &= ~ATC_Status::TASC_stopping;
			}
		}
	}
}

void c_TASC::setBeacon(int index, Beacon b) {
	if (index >= 0 && index < 5) {
		this->setStatus(true);
		c_TASC::P[index][index / 1000] = b;
		Location = Stat.Z + b.Data % 1000;
	}
	else if (index == -1) {
		c_TASC::passage = b;
	}
}

void c_TASC::setStatus(bool in) {
	if (in == true) {
		ATCstatus |= ATC_Status::TASC_control;
	}
	else {
		ATCstatus &= ~ATC_Status::TASC_control;
		ATCstatus &= ~ATC_Status::TASC_doing;
		this->Location = 0;
		for (size_t i = 0; i < 5; i++) {
			for (size_t j = 0; j < 3; j++) {
				P[i][j].Data = 0;
				P[i][j].Num = 0;
				P[i][j].Sig = 0;
				P[i][j].X = 0;
			}
		}
	}
}

void c_TASC::inEmergency(void) {
	ATCstatus &= ~ATC_Status::TASC_control;
	ATCstatus &= ~ATC_Status::TASC_doing;
	ATCstatus &= ~ATC_Status::TASC_stopping;
	this->control.P = 0;
	this->control.B = 0;
}
