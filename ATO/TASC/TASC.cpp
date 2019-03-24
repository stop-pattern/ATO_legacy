#include "../header/define.h"
#include "../header/Header.h"
#include "TASC.h"


void c_TASC::Control(State S, int * panel, int * sound) {
	this->control.P = 0;
	panel[static_cast<int>(ATC_Panel::TASC_braking)] = false;
	if (ATCstatus & static_cast<int>(ATC_Status::TASC_ON)) {
		//PÝ’è
		this->Distance = this->Location - S.Z;
		this->Limit = sqrt(this->Distance * DECELERATION_BRAKE);
		if (this->Limit >= MAX_SPEED || isnan(this->Limit)) {
			this->Limit = MAX_SPEED;
		}

		//B”»’è
		if (ATCstatus & static_cast<int>(ATC_Status::TASC_control)) {
			if (this->control.B >= 0 && this->control.B <= specific.B) {
				if (this->Limit * 0.8 < S.V) {	//TASC“®ì”»’è
					ATCstatus |= static_cast<int>(ATC_Status::TASC_doing);

					if (this->control.B > 0) {
						if (this->Limit < S.V) {
							if (rand() % 3) this->control.B++;
						}
						else {
							if (rand() % 3) this->control.B--;
						}
					}
					else {
						//B”»’è
						if (this->Limit * 0.9 < S.V) {
							if (rand() % 2) this->control.B++;
						}
						else if (this->Limit * 1.1 > S.V) {
							if (rand() % 2) this->control.B--;
						}
					}

					//”ÍˆÍŠO’ù³
					if (this->control.B < 0) {
						this->control.B = 0;
					}
					else if (this->control.B >= specific.E) {
						this->control.B = specific.B;
					}
				}
				else {
					this->control.B = 0;
				}
			}
			if (abs(S.V) < 0.75) {
				ATCstatus |= static_cast<int>(ATC_Status::TASC_stopping);
				ATCstatus &= ~static_cast<int>(ATC_Status::TASC_control);
				ATCstatus &= ~static_cast<int>(ATC_Status::TASC_doing);
			}
		}

		if (ATCstatus & static_cast<int>(ATC_Status::TASC_stopping)) {
		}

		//“]“®–hŽ~B
		if (ATCstatus & static_cast<int>(ATC_Status::TASC_stopping)) {
			this->control.B = 4;
			if (manual.B > this->control.B) {
				this->control.B = 0;
				ATCstatus &= ~static_cast<int>(ATC_Status::TASC_stopping);
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
		ATCstatus |= static_cast<int>(ATC_Status::TASC_control);
	}
	else {
		ATCstatus &= ~static_cast<int>(ATC_Status::TASC_control);
		ATCstatus &= ~static_cast<int>(ATC_Status::TASC_doing);
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
	ATCstatus &= ~static_cast<int>(ATC_Status::TASC_control);
	ATCstatus &= ~static_cast<int>(ATC_Status::TASC_doing);
	ATCstatus &= ~static_cast<int>(ATC_Status::TASC_stopping);
	this->control.P = 0;
	this->control.B = 0;
}
