#include "../header/define.h"
#include "../header/Header.h"
#include "TASC.h"


void c_TASC::Control(State S, int * panel, int * sound) {
	control.P = 0;
	panel[ATC_Panel::TASC_braking] = false;
	if (ATCstatus & ATC_status::TASC_ON) {
		panel[TASC_power] = true;
		panel[TASC_release] = false;
		panel[TASC_braking] = false;
		panel[TASC_controling] = false;
		panel[TASC_noches] = 0;
		panel[TASC_failed] = false;
		panel[TASC_power_M] = false;
		panel[TASC_controling_M] = false;
		panel[TASC_debug] = 0;

		this->Distance = this->Location - S.Z;
		this->Limit = sqrt(this->Distance * DECELERATION_BRAKE);

		if (ATCstatus & ATC_status::TASC_control) {
			ATCstatus |= ATC_status::TASC_doing;
			if (this->Limit * 1.1 < S.V) {
				if (control.B >= 0 && control.B <= specific.B) {
					control.B++;
				}
			}
			else if (this->Limit * 0.9 > S.V) {
				if (control.B >= 0 && control.B <= specific.B) {
					control.B--;
				}
			}
			else {
				ATCstatus &= ~ATC_status::TASC_doing;
			}
			if (S.V < 0.5) {
				ATCstatus |= ATC_status::TASC_stopping;
				ATCstatus &= ~ATC_status::TASC_control;
				ATCstatus &= ~ATC_status::TASC_doing;

			}
		}
		if (ATCstatus & ATC_status::TASC_stopping) {
			control.B = 4;
			if (manual.B > control.B) {
				control.B = 0;
				ATCstatus &= ~ATC_status::TASC_stopping;
			}
		}
	}
	else {
		panel[TASC_power] = false;
		panel[TASC_release] = false;
		panel[TASC_braking] = false;
		panel[TASC_controling] = false;
		panel[TASC_noches] = 0;
		panel[TASC_failed] = false;
		panel[TASC_power_M] = false;
		panel[TASC_controling_M] = false;
		panel[TASC_debug] = 0;
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
		ATCstatus |= ATC_status::TASC_control;
	}
	else {
		ATCstatus &= ~ATC_status::TASC_control;
		ATCstatus &= ~ATC_status::TASC_doing;
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
	ATCstatus &= ~ATC_status::TASC_control;
	ATCstatus &= ~ATC_status::TASC_doing;
	this->control.P = 0;
	this->control.B = 0;
}
