#include "../header/define.h"
#include "../header/Header.h"
#include "TASC.h"


void c_TASC::Control(State S, int * panel, int * sound) {
	control.P = 0;
	switch (TASCstatus) {
	case ATC_status::TASC_stopping:
		control.B = 4;
		if (manual.B > control.B) {
			control.B = 0;
			TASCstatus = ATC_status::TASC_waiting;
		}
		break;
	case ATC_status::TASC_control:
	case ATC_status::TASC_brake:

		break;
	case ATC_status::TASC_ON:
		if (S.V != 0) {
			TASCstatus = ATC_status::TASC_waiting;
		}
	case ATC_status::TASC_waiting:
	default:
		break;
	}

}

void c_TASC::setBeacon(int index, Beacon b) {
	if (index >= 0 && index < 5) {
		this->setStatus(true);
		c_TASC::P[index][index / 1000] = b;
	}
	else if (index == -1) {
		c_TASC::passage = b;
	}
}

void c_TASC::setStatus(bool in) {
	isControling = in;
	if (in == false) {
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
